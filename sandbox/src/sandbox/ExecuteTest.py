"""
usage: python3 ExecuteTest.py PROBLEM_DIR_PATH mode submitted_file_path1 submitted_file_path2 ...
"""

import argparse
import sys
import os
import docker
import json
import shutil
import tempfile


def main():
    parser = argparse.ArgumentParser(description="Execute test cases for a problem.")
    parser.add_argument(
        "problem_dir_path", type=str, help="The path to the problem directory."
    )
    parser.add_argument(
        "mode", choices=["light", "heavy", "both"], help="The mode of execution."
    )
    parser.add_argument(
        "submitted_code_paths", nargs="+", help="Paths to the submitted code files."
    )
    args = parser.parse_args()

    problem_dir_path: str = args.problem_dir_path
    mode: str = args.mode
    submitted_code_paths: list[str] = args.submitted_code_paths

    if not os.path.exists(problem_dir_path):
        print(f"Error: Problem directory '{problem_dir_path}' not found.")
        sys.exit(1)

    if not os.path.isdir(problem_dir_path):
        print(f"Error: '{problem_dir_path}' is not a directory.")
        sys.exit(1)

    # もしproblem_dir_pathの中にtest_settings.jsonがなかったらエラー
    test_settings_path = os.path.join(problem_dir_path, "test_settings.json")
    if not os.path.exists(test_settings_path):
        print(f"Error: '{test_settings_path}' not found.")
        sys.exit(1)

    # もしproblem_dir_pathの中にtest_settings.jsonがあったら読み込む
    with open(test_settings_path, "r", encoding="utf8") as f:
        test_settings = json.load(f)

    # test_settings["requiredSourceFiles"]のリストにあるファイル名が全てsubmitted_file_pathsのbasenameに含まれているか確認
    required_source_files: list[str] = test_settings.get("requiredSourceFiles", [])
    for required_source_file in required_source_files:
        if not any(
            os.path.basename(submitted_code_path) == required_source_file
            for submitted_code_path in submitted_code_paths
        ):
            print(f"Error: '{required_source_file}' not found in submitted files.")
            sys.exit(1)

    # sandboxディレクトリ(一時的)を作成
    temp_dir = tempfile.TemporaryDirectory()

    # sandboxディレクトリの中にproblemディレクトリをコピー
    shutil.copytree(problem_dir_path, temp_dir)

    # sandboxディレクトリの中に提出されたファイルをコピー
    # 同名のファイルがあったら上書き
    for submitted_code_path in submitted_code_paths:
        shutil.copy(submitted_code_path, temp_dir)

    # sandboxコンテナを作成
    client = docker.from_env()
    container = client.containers.run(
        "sandbox",
        volumes={temp_dir.name: {"bind": "/home/guest/test_files", "mode": "rw"}},
        stdin_open=True,  # -i
        tty=True,  # -t
        detach=True,  # -d
    )

    # 実行したいテストのリストを取得
    test_list: list = []
    if mode == "light":
        test_list = test_settings.get("lightTestList", [])
    elif mode == "heavy":
        test_list = test_settings.get("heavyTestList", [])
    else:
        test_list = test_settings.get("lightTestList", []) + test_settings.get(
            "heavyTestList", []
        )
    
    compile_logs: list = []

    # 各テストケースに対してビルドを行う
    for one_test in test_list:
        compile_commands: list = one_test.get("compileCommand", [])
        for compile_command in compile_commands:
            result = container.exec_run(compile_command, workdir="/home/guest/test_files", demux=True)
            compile_commands = result.output[0].decode("utf-8")
            compile_warnings = result.output[1].decode("utf-8")
            compile_logs.append({
                "command": compile_command,
                "warnings": compile_warnings
            })

    # ユーザが提出したプログラムのread/write命令により、コンテナ内の正解ソースコードを表示されることを防ぐため、
    # コンテナ内での.cおよび.cppファイルを削除
    container.exec_run("rm -r *.c *.cpp", workdir="/home/guest/test_files")

    all_tests_results:list = []

    # 各テストケースに対して実行を行う
    for one_test in test_list:
        one_test_results:list = []
        executable: str = one_test.get("executable")
        test_cases: list = one_test.get("testCases", [])
        # 制限時間、メモリ制限を取得
        timeout_ms: int = one_test.get("timeOut")
        max_memory_mb: int = one_test.get("memoryLimit")
        # メモリーリークチェックするかどうか
        check_memory_leak: bool = one_test.get("memoryLeakCheck", bool)
        for test_case in test_cases:
            input_file: str = test_case.get("inputFile")
            command: str = (
                f"python3 ../sandbox/ProcessTracerCmd.py {timeout_ms} {max_memory_mb} ./{executable} < {input_file}"
            )
            result = container.exec_run(
                command, workdir="/home/guest/test_files", demux=True
            )
            # 結果を取得
            # {'status': 'AC', 'stdout': '...', 'stderr': '...', 'time_ms': 0.759, 'memory_mb': 13.24522}
            exec_result: dict = json.loads(result.output[0].decode("utf-8"))
            stdout_data: str = exec_result.get("stdout")
            stderr_data: str = exec_result.get("stderr")
            # 期待される出力を取得
            expected_stdout: str = ""
            with open(test_case.get("outputFile"), "r", encoding="utf8") as f:
                expected_stdout = f.read()
            expected_stderr: str = ""
            with open(test_case.get("stderrFile"), "r", encoding="utf8") as f:
                expected_stderr = f.read()
            # 結果を判定
            exec_status = "AC"
            if exec_result.get("status") == "AC":
                if stdout_data == expected_stdout and stderr_data == expected_stderr:
                    exec_status = "AC"
                else:
                    exec_status = "WA"
            elif exec_result.get("status") == "RE":
                exec_status = "RE"
            elif exec_result.get("status") == "TLE":
                exec_status = "TLE"
            elif exec_result.get("status") == "MLE":
                exec_status = "MLE"
            else:
                exec_status = "UNKNOWN"
            
            ### TODO: valgrindを用いたメモリーリークチェックを行う
            one_test_results.append({
                "status": exec_status,
                "time": exec_result.get("time_ms"),
                "memory": exec_result.get("memory_mb"),
                "stdout": stdout_data,
                "stderr": stderr_data,
                "expected_stdout": expected_stdout,
                "expected_stderr": expected_stderr
            })
        
        all_tests_results.append(one_test_results)

    temp_dir.cleanup()
