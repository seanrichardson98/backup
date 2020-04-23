#include "tinycalc.h"
#include <stdio.h>

/* put your function implementations in here. */

int check_command(char op) {
    char valid[9] = {'q','Q','m','M','+','-','*','/','^'};
    int i;
    for (i = 0; i < 9; i++) {
        if (op == valid[i]) return TC_COMMAND_OK;
    }
    return TC_COMMAND_INVALID;
}

int read_command(char *op, double *num) {
    int valid = 0;
    while (!valid) {
        *op = getchar();
        valid = check_command(*op);
    }
    if (*op == 'q' || *op == 'Q') return 1;

    scanf(" %lf",num);
    
    return 0;
}

void execute_calculation(char f_operator, double f_operand, double *p_result) {
    if (f_operator == '+') {
        *p_result = *p_result + f_operand;
    }
    else if (f_operator == '-') {
        *p_result = *p_result - f_operand;
    }
    else if (f_operator == '*') {
        *p_result = *p_result * f_operand;
    }
    else if (f_operator == '/') {
        *p_result = *p_result / f_operand;
    }
    else if (f_operator == '^' && f_operand >= 0) {
        int i;
        double temp = 1;
        for (i = 0; i < f_operand; i++) {
            temp *= *p_result;
        }
        *p_result = temp;
    }
    else {}
}

double mem_read(tc_memory_t mem, int index) {
    if (index >= TC_MEM_SZ) index = 0;
    return mem.vals[(mem.most_recent+(TC_MEM_SZ)-index)%TC_MEM_SZ];
}

void mem_save(tc_memory_t *mem, double val) {
    (*mem).most_recent = ((*mem).most_recent+1)%5;
    (*mem).vals[(*mem).most_recent] = val;
}
