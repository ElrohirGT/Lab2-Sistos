// Flavio Galán 22386

#include "unistd.h"
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>

void unmap_memory(void *addr, size_t length) {
  int result = munmap(addr, length);
  if (result != 0) {
    fprintf(stderr, "Failed to unmap shared memory!\n");
    exit(1);
  }
}

void unlink_memory(char *name) {
  int result = shm_unlink(name);
  if (result != 0) {
    fprintf(stderr, "Failed to unlink shared memory!\n");
    exit(1);
  }
}

int main(int argc, char **argv) {
  printf("Parsing args...");
  if (argc != 3) {
    fprintf(stderr, "Invalid quantity of arguments supplied! Count: %d\n",
            argc);
    return 1;
  }

  char *invalidChar;
  int n = strtol(argv[1], &invalidChar, 10);
  if (!(n != '\0' && *invalidChar == '\0')) {
    fprintf(stderr, "The first argument must be an integer!\n");
    return 1;
  }
  printf("DONE!\n");

  char *x = argv[2];
  int process_pipe[2];
  char *shared_mem_title = "/Lab2-Sistos-EJ5";
  const int mem_size = 255;

  printf("%s: Opening shared memory...", x);
  int descriptor =
      shm_open(shared_mem_title, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
  if (descriptor == -1) {
    char *inner_error = "";
    int should_quit = 1;
    switch (errno) {
    case EACCES:
      inner_error = "Permission denied!";
      break;
    case EEXIST:
      // Operation failed because the shared memory file already exists!
      // Creating connection without using truncate.
      printf("%s: Memory was already opened!", x);
      descriptor = shm_open(shared_mem_title, O_RDWR, S_IRUSR | S_IWUSR);
      should_quit = 0;
      break;
    default:
      inner_error = "Unknown error";
      break;
    }

    if (should_quit) {
      fprintf(stderr, "%s: Failed to open the shared memory space! Inner: %s\n",
              x, inner_error);
      return 1;
    }
  } else {
    if (ftruncate(descriptor, mem_size) == -1) {
      fprintf(stderr, "%s: Failed to truncate shared memory file!\n", x);
      return 1;
    }
  }
  printf("%s: DONE!\n", x);

  printf("%s: Mapping shared memory to process memory...", x);
  void *mem_pointer = mmap(NULL, mem_size, PROT_READ | PROT_WRITE,
                           MAP_SHARED_VALIDATE, descriptor, 0);
  if (mem_pointer == MAP_FAILED) {
    fprintf(stderr, "%s: Failed to map shared memory to process memory!\n", x);
    unlink_memory(shared_mem_title);
    return 1;
  }
  printf("%s: DONE!\n", x);
  printf("%s: Pointer created using: %p\n", x, mem_pointer);

  printf("%s: Opening pipe...", x);
  int result = pipe(process_pipe);
  if (result == -1) {
    char *inner_error = "";
    switch (errno) {
    case EFAULT:
      inner_error = "pipedf is not valid!";
      break;
    case EINVAL:
      inner_error = "invalid value in flags!";
      break;
    case EMFILE:
      inner_error = "The per-process limit on the number of open file "
                    "descriptors has been reached.";
      break;
    case ENFILE:
      inner_error = "The system-wide limit on the total number of open files "
                    "has been reached.";
      break;
    default:
      inner_error = "Unknown error";
      break;
    }

    fprintf(stderr, "%s: Failed to initialize pipe! Inner: %s\n", x,
            inner_error);

    unmap_memory(mem_pointer, mem_size);
    unlink_memory(shared_mem_title);

    return 1;
  }
  printf("%s: DONE!\n", x);

  int pid = fork();
  if (pid == -1) {
    fprintf(stderr, "%s: Failed to create child process!\n", x);

    unmap_memory(mem_pointer, mem_size);
    unlink_memory(shared_mem_title);

    return 1;
  } else if (pid == 0) {
    // We're on the child
    close(process_pipe[1]);
    char buff[1];
    for (int i = 0; i < mem_size; i++) {
      int read_bytes = read(process_pipe[0], buff, 1);

      if (read_bytes == -1) {
        fprintf(stderr, "%s: Failed to read from pipe!\n", x);
        close(process_pipe[0]);
        return 1;
      } else if (read_bytes == 0) {
        break;
      } else {
        ((char *)mem_pointer)[i] = buff[0];
      }
    }

    close(process_pipe[0]);
  } else {
    // We're on the father
    close(process_pipe[0]);

    int divisible_count = 0;
    for (int i = 0; i < mem_size; i++) {
      if (i % n == 0) {
        write(process_pipe[1], x, 1);
        divisible_count += 1;
      }
    }
    close(process_pipe[1]);
    wait(NULL);

    printf("%s: Printing shared memory: `", x);
    printf("%s", (char *)mem_pointer);
    printf("`\n%s: Only %d are divisible!\n", x, divisible_count);

    unmap_memory(mem_pointer, mem_size);
    // This is expected to fail on the second program...
    unlink_memory(shared_mem_title);
    printf("%s: Unlinked memory!\n", x);
  }
}
