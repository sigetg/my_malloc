#define _XOPEN_SOURCE 500 // needed for sbrk() on cslab

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "my_malloc.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("usage: simple_test1 NUM_ADDRS\n");
        return 1;
    }

    // assume first argument is a number
    int num_addrs = atoi(argv[1]);

    printf("sbrk(0) = %p\n", sbrk(0));

    size_t size;
    char *addr;
    size_t *size_addr;

    for (int i=0; i<num_addrs; ++i)
    {
        size = sizeof(char) * 4 * (i+1);
        addr = (char *)malloc(size);

        size_addr = (size_t *)(addr-8);

        printf("addr(%4lu bytes) = %p, addr[-8] = %lu\n", size, addr, *size_addr);
    }

    printf("sbrk(0) = %p\n", sbrk(0));

    return 0;
}
