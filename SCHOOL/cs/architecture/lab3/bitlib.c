#include<stdio.h>
/*
 * bitwise_nor  - implement ~(x|y) using only ~ and &.
 *  Example biwise_nor(6, 5) = 0xFFFFFFF8
 *  Max. # of operators: 8
 */
int bitwise_nor(int x, int y) {
  return (~x & ~y);
}


/* 
 * bitwise_xor - implement x xor y using only ~ and &.
 * Example bitwise_xor(4,5) = 0x1
 * Max # of operators: 14
 */
int bitwise_xor(int x, int y) {
  return ~(~(x|y)|~(~x|~y));
}

/*
 * eval_not_equal - implement x!=y.
 *  using only !, ~, &, |, ^, +, << and >>
 *   returns 0 if x == y, otherwise 1
 * Examples: eval_not_equal(2,2) = 0, eval_not_equal(3,4) = 1
 * Max # of operators: 6
 */
int eval_not_equal(int x, int y) {
 return !!(x^y);
}


/*
 * get_byte - extract byte n from word x.
 *  using only !, ~, &, |, ^, +, << and >>
 * do not use any constant values of more than 1 byte in size
 * assume ordering as 0=least significant byte,  
 * 3 = most significant byte.
 * Example: get_byte(0x12345678, 1) = 0x56
 * Max # of operators: 6
*/
int get_byte(int x, int n) {
  return ((x >> (n << 3)) & 0xff);
}


/*
* copy_lsbit - set all bits of result value to the least significant bit of x
*  using only !, ~, &, |, ^, +, << and >>
* do not use any constant values of more than 1 byte in size
*  Examples: copy_lsbit(5) = 0xFFFFFFFF, copy_lsbit(8) = 0x0 
*  Max # of operators: 5
*/
int copy_lsbit(int x) {
  return !(x&0x1)+~0x0;
}

/*
 * bit_count - returns the # of bits in x that are 1.
 *  using only !, ~, &, |, ^, +, << and >>
 * do not use any constant values of more than 1 byte in size
 * examples: bit_count(2) = 1, bit_count(10) = 2, bit_count(7) = 3
 * Max # of operators: 40
 */
int bit_count(int x) {
  int h1 = ((0x55 << 8) + 0x55);
  int m1 = (h1 << 16) + h1;
  int h2 = ((0x33 << 8) + 0x33);
  int m2 = (h2 << 16) + h2;
  int h4 = ((0x0f << 8) + 0x0f);
  int m4 = (h4 << 16) + h4;
  int m8 = (0xff << 16) + 0xff;
  int m16 = ((0xff << 8) + 0xff);

  x = (x & m1) + ((x >> 1) & m1);
  x = (x & m2) + ((x >> 2) & m2);
  x = (x & m4) + ((x >> 4) & m4);
  x = (x & m8) + ((x >> 8) & m8);
  x = (x & m16)+ ((x >> 16)& m8);

  return x;
}


