# usage: python3 ProcessTracerCmd.py <timeout_ms> <max_memory_mb> <path_to_executable> <args>

import sys
from sandbox.ProcessTracer import ProcessTracer
from sandbox.ProcessTracer import TracedProcState
import subprocess
import json

def output_json(tracer: ProcessTracer) -> dict:
    output = {}
    # TracedProcState.SUCCESS -> AC
    # TracedProcState.EXEC_ERROR -> RE
    # TracedProcState.TIMEOUT -> TLE
    # TracedProcState.MEMORY_ERROR -> MLE
    if tracer.status == TracedProcState.SUCCESS:
        output["status"] = "AC"
    elif tracer.status == TracedProcState.EXEC_ERROR:
        output["status"] = "RE"
    elif tracer.status == TracedProcState.TIMEOUT:
        output["status"] = "TLE"
    elif tracer.status == TracedProcState.MEMORY_ERROR:
        output["status"] = "MLE"
    else:
        output["status"] = "UNKNOWN"
    
    output["stdout"] = tracer.stdout.decode("utf-8")
    output["stderr"] = tracer.stderr.decode("utf-8")
    output["time_ms"] = tracer.exec_time_ms
    output["memory_mb"] = tracer.max_memory_used_mb

    return output

if __name__ == "__main__":
    if len(sys.argv) < 4:
        print("Usage: python3 ProcessTracerCmd.py <timeout_ms> <max_memory_mb> <path_to_executable> <args>")
        sys.exit(1)

    timeout_ms = int(sys.argv[1])
    max_memory_mb = int(sys.argv[2])
    pathname = sys.argv[3]
    args = sys.argv[4:]

    tracer = ProcessTracer(pathname, args, timeout_ms, max_memory_mb)
    tracer.spawn()

    output = output_json(tracer)

    # print(output)

    print(json.dumps(output, indent=4))

    sys.exit(0)
