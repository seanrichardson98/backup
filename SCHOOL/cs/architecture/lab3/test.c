#include<stdio.h>
#include"bitlib.h"

int main() {
    int x = 0x76543210;
    int n = 3;
    printf("%04x\n", get_byte(x,n));
    printf("%04x\n",(x<<1));

}
