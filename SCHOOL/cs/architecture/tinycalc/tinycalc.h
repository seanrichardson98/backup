 /*------------------------------tinycalc.h -----------------------------------------//
 // *** DON'T EDIT THIS FILE.  YOUR SOLUTION WILL BE COMPILED USING THIS VERSION OF  //
 // tinycalc.h FOR GRADING.                                                          //
 //----------------------------------------------------------------------------------*/

#define TC_COMMAND_QUIT 0
#define TC_COMMAND_OK 1
#define TC_COMMAND_INVALID 0

#define TC_MEM_SZ 5

typedef struct _tc_mem {
    double vals[TC_MEM_SZ];
    int most_recent;
} tc_memory_t;

//--------------------------- read_command --------------------------------//
// reads an operator (char) and an operand (double) from stdin. function   //
// continues to read from stdin until a valid operator is entered.         //
//                                                                         //
// when q or Q are typed for the operator, read_command will return        //
// TC_COMMAND_QUIT and leave the values of the function parameters         //
// unchanged.  otherwise it will return TC_COMMAND_OK and update the       //
// values of the function parameters with the values read from stdin.      //
//-------------------------------------------------------------------------//
int read_command(char *operator, double *operand);

//----------------------------check_command -------------------------------//
// verifies if an operand is supported by TinyCalc.                        //
//                                                                         //
// returns TC_COMMAND_OK if the operand or command (m,M,q,Q) is supported  //
// otherwise returns TC_COMMAND_INVALID.                                   //
//-------------------------------------------------------------------------//
int check_command(char operator);

//------------------------execute_calculation------------------------------//
// performs the arithmetic operation: [operand2] [operator] [operand1]     //
// and saves the result in operand1.                                       //
//-------------------------------------------------------------------------//
void execute_calculation(char operator, double operand2, double *operand1);

//---------------------------read_mem -------------------------------------//
// returns the result of the nth most recent calculation.                  //
//-------------------------------------------------------------------------//
double mem_read(tc_memory_t memory, int position);

//--------------------------mem_save --------------------------------------//
// saves a value as the most recent calcuation in memory.                  //
//-------------------------------------------------------------------------//
void mem_save(tc_memory_t *, double);
