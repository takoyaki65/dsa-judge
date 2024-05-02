import time
import subprocess
import resource
from enum import Enum
import psutil

class TracedProcState(Enum):
    NOT_STARTED = 0
    RUNNING = 1
    SUCCESS = 2
    TIMEOUT = 3  # Process exceeded timeout
    MEMORY_ERROR = 4  # Process exceeded memory limit
    EXEC_ERROR = 5  # Process returned non-zero exit code


class ProcessTracer:
    timeout_ms: int = 1000
    max_memory_mb: int = 100
    stdout_data: str = b""
    stderr_data: str = b""
    exec_time_ms: int = 0
    max_memory_used_mb: int = 0
    pathname: str = ""
    args: list[str] = []
    process: psutil.Popen = None
    status: TracedProcState = TracedProcState.NOT_STARTED
    exitcode: int = 0

    def __init__(
        self,
        pathname: str,
        args: list[str],
        timeout_ms: int = 1000,
        max_memory_mb: int = 100,
    ):
        self.pathname = pathname
        self.args = args
        self.timeout_ms = timeout_ms
        self.max_memory_mb = max_memory_mb

    def spawn(self) -> None:
        """
        Spawns a new process and traces its execution.

        This method starts a new process using the specified `pathname` and `args`.
        It then continuously checks the process's execution time and memory usage
        to ensure it does not exceed the specified timeout and memory limits.

        Raises:
            TimeoutError: If the process exceeds the specified timeout.
            MemoryError: If the process exceeds the specified memory limit.
            subprocess.CalledProcessError: If the process exits with a non-zero exit code.

        Returns:
            None
        """
        print(f"timeout: {self.timeout_ms}")
        print(f"max_memory: {self.max_memory_mb}")
        print(f"pathname: {self.pathname}")
        print(f"args: {self.args}")
        self.process = psutil.Popen(
            [self.pathname] + self.args, stdout=subprocess.PIPE, stderr=subprocess.PIPE
        )
        self.status = TracedProcState.RUNNING

        try:
            while self.process.is_running():
                try:
                    # print("Waiting for process to finish")
                    self.exitcode = self.process.wait(timeout=self.timeout_ms / 10.0)
                    # If TimoutExpired is not raised, the process has finished
                    break
                except psutil.TimeoutExpired:
                    # print("Process is still running")
                    pass

                # Check timeout
                cpu_times = self.process.cpu_times()
                self.exec_time_ms = (
                    cpu_times.user + cpu_times.system + cpu_times.iowait  # in seconds
                ) * 1000  # in milliseconds
                # print(f"Execution time: {self.exec_time_ms}")
                if self.exec_time_ms > self.timeout_ms:
                    self.process.terminate()
                    self.status = TracedProcState.TIMEOUT
                    raise TimeoutError(
                        f"Process exceeded timeout of {self.timeout_ms} ms"
                    )

                # Check memory usage
                memory_info: psutil.Process.memory_info = self.process.memory_info()
                memory_used_mb: float = memory_info.rss / 1024.0 / 1024.0
                self.max_memory_used_mb = max(memory_used_mb, self.max_memory_used_mb)
                # print(f"Memory used: {self.max_memory_used_mb}")
                if self.max_memory_used_mb > self.max_memory_mb:
                    self.process.terminate()
                    self.status = TracedProcState.MEMORY_ERROR
                    raise MemoryError(
                        f"Process exceeded memory limit of {self.max_memory_mb} MB"
                    )

            usage = resource.getrusage(resource.RUSAGE_CHILDREN)
            self.max_memory_used_mb = max(
                self.max_memory_used_mb, usage.ru_maxrss / 1024.0
            )
            self.stdout_data, self.stderr_data = self.process.communicate()
            self.exec_time_ms = (usage.ru_utime + usage.ru_stime) * 1000
        
        except (TimeoutError, MemoryError):
            self.process.terminate()
            self.exitcode = self.process.wait()
            self.stdout_data, self.stderr_data = self.process.communicate()
            raise

        if self.exitcode != 0:
            self.status = TracedProcState.EXEC_ERROR
            raise subprocess.CalledProcessError(
                self.exitcode,
                self.pathname,
                output=self.stdout_data,
                stderr=self.stderr_data,
            )
        if self.exitcode == 0:
            self.status = TracedProcState.SUCCESS

    @property
    def stdout(self):
        """
        Returns the stdout data of the process.

        Returns:
          str: The stdout data of the process.
        """
        return self.stdout_data

    @property
    def stderr(self):
        """
        Returns the stderr data.

        Returns:
            str: The stderr data.
        """
        return self.stderr_data
