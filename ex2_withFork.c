#include <stdio.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main() {
  clock_t start = clock();
  int fatherFork = fork();

  if (fatherFork == 0) {
    // Estamos en el proceso hijo
    int sonFork = fork();

    if (sonFork == 0) {
      // Estamos en el proceso nieto
      for (int i = 0; i < 1000000; i++) {
        printf("Iteration: %d\n", i);
      }
    } else {
      // Estamos en el proceso hijo
      for (int i = 0; i < 1000000; i++) {
        printf("Iteration: %d\n", i);
      }
      wait(NULL);
    }
  } else {
    for (int i = 0; i < 1000000; i++) {
      printf("Iteration: %d\n", i);
    }
    wait(NULL);

    clock_t end = clock();

    double diff = (double)end - (double)start;
    printf("Se tardo: %f", diff);
  }
}
