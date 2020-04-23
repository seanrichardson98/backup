#include <stdio.h>

int factorial(int num) {
    if (num == 0) return 1;
    return num*factorial(num-1);
}
int main() {
    printf("%d\n",factorial(5));
}
