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
    parser.add_argument("problem_dir_path", type=str, help="The path to the problem directory.")
    parser.add_argument("mode", choices=["light", "heavy", "both"], help="The mode of execution.")
    parser.add_argument("submitted_code_paths", nargs="+", help="Paths to the submitted code files.")
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
    with open(test_settings_path, "r", encoding='utf8') as f:
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
        tty=True,         # -t
        detach=True,      # -d
    )

    # 実行したいテストのリストを取得
    test_list: list = []
    if mode == "light":
        test_list = test_settings.get("lightTestList", [])
    elif mode == "heavy":
        test_list = test_settings.get("heavyTestList", [])
    else:
        test_list = test_settings.get("lightTestList", []) + test_settings.get("heavyTestList", [])
    
    # 各テストケースに対してビルドを行う
    for one_test in test_list:
        compile_commands: list = one_test.get("compileCommand", [])
        for compile_command in compile_commands:
            container.exec_run(compile_command, workdir="/home/guest/test_files")
    
    # ユーザが提出したプログラムのread/write命令により、コンテナ内の正解ソースコードを表示されることを防ぐため、
    # コンテナ内での.cおよび.cppファイルを削除
    container.exec_run("rm -r *.c *.cpp", workdir="/home/guest/test_files")

    # 各テストケースに対して実行を行う
    for one_test in test_list:
        executable: str = one_test.get("executable")
        test_cases: list = one_test.get("testCases", [])
        for test_case in test_cases:
            input_file: str = test_case.get("inputFile")
            output_file: str = test_case.get("outputFile")
            command: str = f"python3 ../ProcessTracerCmd.py ./{executable} < {input_file}"
            # result = 


    temp_dir.cleanup()
    

    
    
