#include <stdio.h>

void sort(long *data, long count);

/**
 * Print the elements of the input array, one per line.
 * @param data  array of 64-bit integers
 * @param count  number of elements in the input array
 */
void print(long *data, long count)
{
    for (long i = 0; i < count; ++i) {
        printf("%ld: %10ld %016lx\n", i, data[i], data[i]);
    }
}

#define SZ 10

int main()
{
    long array[SZ] = {
        0x0FFF,
        0x0F20,
        0xFFFFFFFFFFFFFF45,
        0xFFFFFFFFFFFFFFFF,
        0x01,
        0x03,
        0x04,
        0x0A,
        0x09,
        0x11
    };

    sort(array, SZ);
    print(array, SZ);
}
