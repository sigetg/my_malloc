#define _XOPEN_SOURCE 500 // needed for sbrk() on cslab

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "my_malloc.h"

void do_test(size_t size)
{
    char *addr;
    size_t *size_addr;

    addr = (char *)malloc(size*sizeof(char));
    size_addr = (size_t *)(addr - 8);
    printf("addr(%4lu bytes) = %p, addr[-8] = %lu\n", size, addr, *size_addr);

    free(addr);

    printf("sbrk(0) = %p\n", sbrk(0));
}

int main()
{
    printf("sbrk(0) = %p\n", sbrk(0));

    do_test(4084);
    do_test(4);
    do_test(4);
    do_test(4068);
    do_test(4);

    return 0;
}
