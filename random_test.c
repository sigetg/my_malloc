#define _XOPEN_SOURCE 500 // needed for sbrk() on cslab

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "my_malloc.h"

/* convert (unsigned long) d to string s in base b */
void int2str(unsigned long d, int base, char s[], int start_pos, int end_pos)
{
    while (start_pos >= end_pos && d != 0)
    {
        int digit = (d % base);
        d = d/base;
        if (digit < 10)
        {
            s[start_pos--] = '0' + digit;
        }
        else
        {
            digit -= 10;
            s[start_pos--] = 'a' + digit;
        }
    }
    while (start_pos >= end_pos)
    {
        s[start_pos--] = ' ';
    }
}

/* do equivalent of printf("allocate %i ", size); */
void msg1(int size)
{
    char str[18] = "allocate ";
    int2str(size, 10, str, 16, 9);
    str[17] = ',';
    write(STDOUT_FILENO, str, 18);
}

/* do equivalent of either:
 *   printf("got  ptr=%p malloc_size=%lu\n", ptr, ptr-8);
 * or:
 *   printf("free ptr=%p malloc_size=%lu\n", ptr, ptr-8);
 * */
void msg2(char *ptr, int free)
{
    char str[23] = "got  ptr=0x";
    if (free)
    {
        str[0] = 'f';
        str[1] = 'r';
        str[2] = 'e';
        str[3] = 'e';
    }
    int2str((unsigned long)ptr, 16, str, 21, 11);
    str[22] = ',';
    write(STDOUT_FILENO, str, 23);

    char str2[21] = "malloc_size=";
    size_t *p = (size_t *)(ptr-8);
    int2str(*p, 10, str2, 19, 12);
    str2[20] = '\n';
    write(STDOUT_FILENO, str2, 21);
}

void usage()
{
    printf("usage: random_test NUM_ADDRS MAX_SIZE NUM_ITERATIONS\n");
    exit(1);
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        usage();
    }

    // assume all arguments are ints 
    int num_addrs = atoi(argv[1]);
    int max_size = atoi(argv[2]);
    int num_iters = atoi(argv[3]);
    if (num_addrs <= 0 || max_size <= 0 || num_iters <= 0)
    {
        usage();
    }

    srandom(time(NULL));

    char *ptrs[num_addrs];
    for (int i=0; i<num_addrs; ++i)
    {
        size_t size = (random() % max_size) + 1;
        msg1(size*sizeof(char));
        ptrs[i] = (char *)malloc(size*sizeof(char));

        msg2(ptrs[i], 0);
    }

    for (int iter=0; iter<num_iters; ++iter)
    {
        for (int i=0; i<num_addrs; ++i)
        {
            int free_ptr = random() % 2;
            if (free_ptr)
            {
                msg2(ptrs[i], 1);
                free(ptrs[i]);
                ptrs[i] = NULL;
            }
        }

        for (int i=0; i<num_addrs; ++i)
        {
            if (ptrs[i] == NULL)
            {
                size_t size = (random() % max_size) + 1;
                msg1(size*sizeof(char));
                ptrs[i] = (char *)malloc(size*sizeof(char));

                msg2(ptrs[i], 0);
            }
        }
    }

    return 0;
}
