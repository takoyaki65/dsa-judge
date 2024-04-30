#include <chrono>
#include <complex.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
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

int get_memory(pid_t pid) {
  char path[256];
  snprintf(path, 255, "/proc/%d/status", pid);
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    std::cerr << "Failed to open file." << std::endl;
    return 0;
  }

  int memory = 0;
  int length = 0;

  char line[256];
  while (fgets(line, 255, file) != NULL) {
    if (strncmp(line, "VmHWM:", 6) == 0) {
      length = strlen(line);
      sscanf(line, "VmHWM: %d kB", &memory);
      break;
    }
  }

  fclose(file);
  return memory;
}

int main(int argc, char *argv[]) {
  int opt;
  int timeout = 1;
  int max_memory = 32 * 1024;

  while ((opt = getopt(argc, argv, "t:m:")) != -1) {
    switch (opt) {
    case 't':
      timeout = atoi(optarg);
      break;
    case 'm':
      max_memory = atoi(optarg) * 1024; // Convert MB to KB
      break;
    default:
      std::cerr << "Usage: " << argv[0]
                << " [-t timeout] [-m max_memory_mb] <command> [args...]"
                << std::endl;
      return 1;
    }
  }

  if (optind >= argc) {
    std::cerr << "Usage: " << argv[0]
              << " [-t timeout] [-m max_memory_mb] <command> [args...]"
              << std::endl;
    return 1;
  }

  printf("timeout: %d\n", timeout);
  printf("max_memory: %d\n", max_memory);

  pid_t pid = fork();
  if (pid == -1) {
    std::cerr << "Fork failed." << std::endl;
    return 1;
  }

  if (pid == 0) {
    // Child process
    std::cout << "Child PID: " << getpid() << std::endl;
    execvp(argv[optind], &argv[optind]);
    std::cerr << "Exec failed." << std::endl;
    _exit(EXIT_FAILURE);
  } else {
    rusage usage;
    auto start = std::chrono::steady_clock::now();

    while (true) {
      // Parent process
      pid_t ret = waitpid(pid, nullptr, WNOHANG);
      if (ret == -1) {
        std::cerr << "Waitpid failed." << std::endl;
        return 1;
      } else if (ret == pid) {
        // Child process has terminated
        getrusage(RUSAGE_CHILDREN, &usage);
        std::cout << "Command finished." << std::endl;
        print_usage_stats(usage);
        if (usage.ru_maxrss > max_memory)
          std::cout << "Memory limit exceeded." << std::endl;
        break;
      } else {
        // Child process is still running
        // get resource usage from /proc/$pid/status file
        int memory = get_memory(pid);

        // Check timeout
        if (timeout > 0) {
          auto end = std::chrono::steady_clock::now();
          auto elapsed =
              std::chrono::duration_cast<std::chrono::seconds>(end - start)
                  .count();
          // printf("elapsed: %ld\n", elapsed);
          if (elapsed >= timeout) {
            std::cout << "Timeout limit exceeded." << std::endl;
            kill(pid, SIGTERM);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            if (waitpid(pid, nullptr, WNOHANG) == 0) {
              std::cout << "Killing process." << std::endl;
              kill(pid, SIGKILL);
            }
            getrusage(RUSAGE_CHILDREN, &usage);
            print_usage_stats(usage);
            break;
          }
        }

        // Check memory limit
        if (max_memory > 0 && memory > max_memory) {
          std::cout << "Memory limit exceeded." << std::endl;
          kill(pid, SIGKILL);
          getrusage(RUSAGE_CHILDREN, &usage);
          usage.ru_maxrss = memory;
          print_usage_stats(usage);
          break;
        }

        auto end = std::chrono::steady_clock::now();
        auto elapsed =
            std::chrono::duration_cast<std::chrono::seconds>(end - start)
                .count();
        if (elapsed >= 1) {
          std::this_thread::sleep_for(std::chrono::seconds(1));
        } else {
          std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
      }
    }
  }
}