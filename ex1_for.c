// Flavio Galan 22386
#include <stdio.h>
#include <unistd.h>

int main() {
  for (int i = 0; i < 4; i++) {
    printf("Iteration: %d\n", i);
    // 0 1 2 3
    // 1 2 4 8
    fork();
  }

  // En esta parte hay 16 procesos!
}
