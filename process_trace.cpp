#include <chrono>
#include <cstdlib>
#include <iostream>
#include <sys/resource.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

void print_usage_stats(const rusage &rusage) {
  std::cout << "User CPU time used: " << rusage.ru_utime.tv_sec << "."
            << rusage.ru_utime.tv_usec << " seconds" << std::endl;
  std::cout << "System CPU time used: " << rusage.ru_stime.tv_sec << "."
            << rusage.ru_stime.tv_usec << " seconds" << std::endl;
  std::cout << "Maximum resident set size: " << rusage.ru_maxrss << " kB"
            << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <command> [args...]" << std::endl;
    return 1;
  }

  pid_t pid = fork();
  if (pid == -1) {
    std::cerr << "Fork failed." << std::endl;
    return 1;
  }

  if (pid == 0) {
    // Child process
    std::cout << "Child PID: " << getpid() << std::endl;
    execvp(argv[1], argv + 1);
    std::cerr << "Exec failed." << std::endl;
    _exit(EXIT_FAILURE);
  } else {
    rusage usage;
    auto start = std::chrono::steady_clock::now();
    while (true) {
      // Parent process
      pid_t ret = waitpid(pid, nullptr, 0);
      if (ret == -1) {
        std::cerr << "Waitpid failed." << std::endl;
        return 1;
      } else if (ret == pid) {
        // Child process has terminated
        getrusage(RUSAGE_CHILDREN, &usage);
        std::cout << "Command finished." << std::endl;
        print_usage_stats(usage);
        break;
      } else {
        // Child process is still running
        getrusage(RUSAGE_CHILDREN, &usage);
        std::cout << "Command still running." << std::endl;
        print_usage_stats(usage);

        auto end = std::chrono::steady_clock::now();
        auto elapsed =
            std::chrono::duration_cast<std::chrono::seconds>(end - start)
                .count();
        if (elapsed >= 1) {
          start = end;
          std::this_thread::sleep_for(std::chrono::seconds(1));
        }
      }
    }
  }
}