#include<stdio.h>

int legal(int *list) {
    int i = 0;
    int j = 0;

    int check[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    printf("here2\n");
    for (i = 0; i < 3; i++) {
            printf("%d\n",i);
        for (j = i+1; j < 3; j++) {
            printf("%d,%d\n",i,j);
            int index = list[j]-list[i]-1;
            check[index] = 1;
            check[(13-index)] = 1;
        }
    }

    for (i = 0; i < 13; i++) {
        printf("%d\n",check[i]);
        //if (i == 0) return 0;
    }
    return 1;
}

int main() {
    int list[4] = {1,2,4};
    printf("here1\n");
    printf("%d\n",legal(list));
}

