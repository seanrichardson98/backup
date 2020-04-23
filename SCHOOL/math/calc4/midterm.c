#include<stdio.h>
#include<math.h>

int main() {
    double ak = 1;
    double mult;
    int k;
    double sum = 0;
    double x = .9;
    for (k = 1; k < 100; k+=2) {
        printf("%lf + ",ak);
        mult = 1.0*(k*(k+1)-6)/((k+1)*(k+2));
        //printf("mult: %lf", mult);
        ak = ak*mult;
        sum += ak*pow(x,k);
    }
    printf("\nsum:%lf\n",sum);
}
