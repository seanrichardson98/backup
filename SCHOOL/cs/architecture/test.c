#include<stdio.h>

int main() {
    unsigned long l = (0X55555555 & (~(0XFFFF << 8))) | (0X9889 << 8);
    printf("%lu\n",l);
}
/*
0x00055234 & 0x1fff
1234
 55234555
ffe000fff
054000555

*/
