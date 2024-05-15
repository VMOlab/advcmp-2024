#include <stdio.h>

int main() {
    const int x = 12;
    const int y = 20;

    int i, j;

    for (j = 0; j < y; ++j) {
        for (i = 0; i < x; ++i) {
            if (i > j)
                continue;
            
            printf("*");
        }
        printf("\n");
    }

    return 0;
}