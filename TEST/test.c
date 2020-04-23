#include <stdio.h>
#include <stdlib.h>

void show_bytes(char* start, size_t len) {
    int i;
    for (i = 0; i < len; i++)
        printf(" %.2x", start[i]);
    printf("\n");
}

void print_arr(int *p, int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%d\t",p[i]);
    }
}

int main() {
    int i;
    int arr[10] = {0,0,0,0,0,0,0,0,0,0};
    for(i = 0; i < 1; i++) {
        int l[2];
        int *p = l;
        int **q = &p;
        int r = abs(**q)%10;
        arr[r] = arr[r] + 1;
    }
    print_arr(arr,10); printf("\n");
}
