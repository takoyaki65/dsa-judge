# usage: python3 ProcessTracerCmd.py <timeout_ms> <max_memory_mb> <path_to_executable> <args>

import sys
import ProcessTracer
import subprocess

if __name__ == "__main__":
    if len(sys.argv) < 4:
        print("Usage: python3 ProcessTracerCmd.py <timeout_ms> <max_memory_mb> <path_to_executable> <args>")
        sys.exit(1)

    timeout_ms = int(sys.argv[1])
    max_memory_mb = int(sys.argv[2])
    pathname = sys.argv[3]
    args = sys.argv[4:]

    tracer = ProcessTracer.ProcessTracer(pathname, args, timeout_ms, max_memory_mb)
    try:
        tracer.spawn()
        print(f"Status: {tracer.status}")
        print(f"Stdout: {tracer.stdout_data}")
        print(f"Stderr: {tracer.stderr_data}")
        print(f"Executed in {tracer.exec_time_ms} ms")
        print(f"Memory used: {tracer.max_memory_used_mb} MB")
    except (TimeoutError, MemoryError, subprocess.CalledProcessError) as e:
        print(f"Status: {tracer.status}")
        print(f"Error: {str(e)}")
        print(f"Stdout: {tracer.stdout_data}")
        print(f"Stderr: {tracer.stderr_data}")
        print(f"Executed in {tracer.exec_time_ms} ms")
        print(f"Memory used: {tracer.max_memory_used_mb} MB")

    sys.exit(0)
