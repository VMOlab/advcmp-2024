#include <stdio.h>

int main() {
  int x = 1;
  int y = x + 2; // 3
  int z = y + 2; // 5

  if (y < 2) { // false
    z = 1;
  }

  // x = 1, y = 3, z = 5
  printf("%d %d %d\n", x, y, z);

  return 0;
}