#include <stdio.h>
#include "tinycalc.h"

/* put your application code in this file. */

/*TODO
  - allow for building operations off of memory
  - Let nonsense mean 0
*/


int main(void) {

    printf("\nWelcome to TinyCalc!\n\n Enter an operation <+, - , *, /, ^>");
    printf(" followed by operand\n\n Enter 'q' or 'Q' to quit.\n\n");
    printf(" Enter 'm' or 'M' followed by location (0-4) to load a previous\n");
    printf(" result from memory.");
    printf("\n>");
 
    //Initialize
    tc_memory_t mem;
    mem.most_recent = 0;
    int i;
    for (i = 0; i < TC_MEM_SZ; i++) {
        mem.vals[i] = 0;
    }
    char op;
    double num = 0;
    double last;
    int mem_buffer = 0;
    double mem_num;

    while (1) {
        last = mem_read(mem, 0);
        read_command(&op, &num);
        if (op == 'm' || op == 'M') {
            mem_num = mem_read(mem,num);
            mem_buffer = 1;
            printf("%.2lf\n>",mem_num);
        }
        else if (op == 'q' || op == 'Q') {
            return 1;
        }
        else {
            if (mem_buffer) {last = mem_num; mem_buffer = 0;}
            execute_calculation(op, num, &last);
            mem_save(&mem,last);
            printf("%.2lf\n>",last);
        }
        num = 0;
    }
    return(0);
}
