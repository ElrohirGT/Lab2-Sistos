#include <stdio.h>
#include <time.h>

int main() {
  clock_t start;
  clock_t end;

  start = clock();

  for (int i = 0; i < 1000000; i++) {
    printf("Iteration: %d\n", i);
  }
  for (int i = 0; i < 1000000; i++) {
    printf("Iteration: %d\n", i);
  }
  for (int i = 0; i < 1000000; i++) {
    printf("Iteration: %d\n", i);
  }

  end = clock();

  double diff = (double)end - (double)start;

  printf("Se tardo: %f", diff);
}
