// Flavio Galan 22386
#include <stdio.h>
#include <unistd.h>

int main() {
  int fatherFork = fork();
  if (fatherFork == 0) {
    for (int i = 0; i < 14000000; i++) {
      printf("Conteo %d\n", i);
    }
  } else {
    while (1) {
    }
  }
}
