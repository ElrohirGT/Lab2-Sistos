#include "sys/wait.h"
#include "unistd.h"
#include <errno.h>
#include <stdio.h>

int main() {
  int pid = fork();
  if (pid == -1) {
    fprintf(stderr, "Failed to create child process! Code: %d\n", errno);
    return -1;
  } else if (pid == 0) {
    // We're on the child
    if (execl("p", ".", "10", "a") == -1) {
      fprintf(stderr, "Failed to run program `a`!\n");
      return -1;
    }
  } else {
    // We're on the father
    if (execl("p", ".", "5", "b") == -1) {
      fprintf(stderr, "Failed to run program `b`!\n");
      return -1;
    }
    wait(NULL);
  }
}
