#include <stdio.h>

int main() {
    int x = 99;
    int y = 50;
    float s = 0.0;

    printf("Valor de x final 99, valor de y final 50\n");

    for (int i = 1; i <= x && y > 0; i += 2, y--) {
        s += (float)i / y;
    }

    printf("Soma: %.2f\n", s);

    return 0;
}
