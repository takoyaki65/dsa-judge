"""
module for tracing and monitoring the execution of a process.
"""

import subprocess
import resource
from enum import Enum
import psutil


class TracedProcState(Enum):
    """
    Represents the state of a traced process.

    Attributes:
        NOT_STARTED (int): The process has not started yet.
        RUNNING (int): The process is currently running.
        SUCCESS (int): The process completed successfully.
        TIMEOUT (int): The process exceeded the timeout limit.
        MEMORY_ERROR (int): The process exceeded the memory limit.
        EXEC_ERROR (int): The process returned a non-zero exit code.
    """

    NOT_STARTED = 0
    RUNNING = 1
    SUCCESS = 2
    TIMEOUT = 3  # Process exceeded timeout
    MEMORY_ERROR = 4  # Process exceeded memory limit
    EXEC_ERROR = 5  # Process returned non-zero exit code


class ProcessTracer:
    """
    A class for tracing and monitoring the execution of a process.

    Attributes:
        timeout_ms (int): The timeout value in milliseconds.
        max_memory_mb (int): The maximum memory limit in megabytes.
        stdout_data (str): The stdout data of the process.
        stderr_data (str): The stderr data of the process.
        exec_time_ms (int): The execution time of the process in milliseconds.
        max_memory_used_mb (int): The maximum memory used by the process in megabytes.
        pathname (str): The path to the executable file.
        args (list[str]): The command-line arguments for the process.
        process (psutil.Popen): The process object representing the running process.
        status (TracedProcState): The current status of the process.
        exitcode (int): The exit code of the process.
    """

    _timeout_ms: int = 1000
    _max_memory_mb: int = 100
    _stdout_data: str = b""
    _stderr_data: str = b""
    _exec_time_ms: float = 0
    _max_memory_used_mb: float = 0
    _pathname: str = ""
    _args: list[str] = []
    _process: psutil.Popen = None
    _status: TracedProcState = TracedProcState.NOT_STARTED
    _exitcode: int = 0

    def __init__(
        self,
        pathname: str,
        args: list[str],
        timeout_ms: int = 1000,
        max_memory_mb: int = 100,
    ):
        """
        Initializes a new ProcessTracer object.

        Args:
            pathname (str): The path to the executable file.
            args (list[str]): The command-line arguments for the process.
            timeout_ms (int): The timeout value in milliseconds (default: 1000).
            max_memory_mb (int): The maximum memory limit in megabytes (default: 100).
        """
        self._pathname = pathname
        self._args = args
        self._timeout_ms = timeout_ms
        self._max_memory_mb = max_memory_mb

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
        # print(f"timeout: {self._timeout_ms}")
        # print(f"max_memory: {self._max_memory_mb}")
        # print(f"pathname: {self._pathname}")
        # print(f"args: {self._args}")
        self._process = psutil.Popen(
            [self._pathname] + self._args,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
        self._status = TracedProcState.RUNNING

        while self._process.is_running():
            try:
                # print("Waiting for process to finish")
                self._exitcode = self._process.wait(timeout=self._timeout_ms / 10.0)
                # If TimoutExpired is not raised, the process has finished
                break
            except psutil.TimeoutExpired:
                # print("Process is still running")
                pass

            # Check timeout
            cpu_times = self._process.cpu_times()
            self._exec_time_ms = (
                cpu_times.user + cpu_times.system + cpu_times.iowait  # in seconds
            ) * 1000  # in milliseconds
            # print(f"Execution time: {self.exec_time_ms}")
            if self._exec_time_ms > self._timeout_ms:
                self._process.terminate()
                self._exitcode = self._process.wait()
                self._status = TracedProcState.TIMEOUT
                break

            # Check memory usage
            memory_info: psutil.Process.memory_info = self._process.memory_info()
            memory_used_mb: float = memory_info.rss / 1024.0 / 1024.0
            self._max_memory_used_mb = max(memory_used_mb, self._max_memory_used_mb)
            # print(f"Memory used: {self.max_memory_used_mb}")
            if self._max_memory_used_mb > self._max_memory_mb:
                self._process.terminate()
                self.exitcode = self._process.wait()
                self._status = TracedProcState.MEMORY_ERROR
                break

        usage = resource.getrusage(resource.RUSAGE_CHILDREN)
        self._max_memory_used_mb = max(
            self._max_memory_used_mb, usage.ru_maxrss / 1024.0
        )
        self._stdout_data, self._stderr_data = self._process.communicate()
        self._exec_time_ms = (usage.ru_utime + usage.ru_stime) * 1000

        if self._exitcode != 0:
            self._status = TracedProcState.EXEC_ERROR

        if self._exitcode == 0:
            self._status = TracedProcState.SUCCESS
            if self._max_memory_used_mb > self._max_memory_mb:
                self._status = TracedProcState.MEMORY_ERROR

    @property
    def stdout(self):
        """
        Returns the stdout data of the process.

        Returns:
          str: The stdout data of the process.
        """
        return self._stdout_data

    @property
    def stderr(self):
        """
        Returns the stderr data.

        Returns:
            str: The stderr data.
        """
        return self._stderr_data

    @property
    def status(self):
        """
        Returns the status of the process.

        Returns:
            TracedProcState: The status of the process.
        """
        return self._status

    @property
    def exitcode(self):
        """
        Returns the exit code of the process.

        Returns:
            int: The exit code of the process.
        """
        return self._exitcode

    @property
    def exec_time_ms(self):
        """
        Returns the execution time of the process in milliseconds.

        Returns:
            float: The execution time of the process in milliseconds.
        """
        return self._exec_time_ms

    @property
    def max_memory_used_mb(self):
        """
        Returns the maximum memory used by the process in megabytes.

        Returns:
            float: The maximum memory used by the process in megabytes.
        """
        return self._max_memory_used_mb
