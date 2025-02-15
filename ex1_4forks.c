// Flavio Galan 22386
#include <stdio.h>
#include <unistd.h>

int main() {
  printf("FATHER: %d\n", getpid());
  fork(); // 2
  printf("CHILD: %d\n", getpid());
  fork(); // 4
  printf("GRAND CHILD: %d\n", getpid());
  fork(); // 8
  printf("GRAND GRAND CHILD: %d\n", getpid());
  fork(); // 16
  printf("GRAND GRAND GRAND CHILD: %d\n", getpid());
}
