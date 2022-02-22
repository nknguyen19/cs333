/* sort.c
 *    Test program to sort a large number of integers.
 *
 *    Intention is to stress virtual memory system.
 *
 *    Ideally, we could read the unsorted array off of the file system,
 *	and store the result back to the file system!
 */

/*
#define UNIX
#define UNIX_DEBUG
*/

#ifdef UNIX
#include <stdio.h>
#define Exit exit
#else
#include "syscall.h"
#endif /* UNIX */

#define SIZE 100

int A[SIZE]; /* size of physical memory; with code, we'll run out of space!*/

int main()
{
    int i, j, n, tmp;
    char type;

    PrintString("Please enter the size of array (0 < n <= 100): ");
    n = ReadNum();

    while (n > 100 || n < 1)
    {
        PrintString("Invalid size of array. Please enter again a number in range [1, 100]: ");
        n = ReadNum();
    }

    PrintString("Please enter the array: ");
    for (i = 0; i < n; i++)
    {
        A[i] = ReadNum();
    }

    PrintString("Please enter type of order (0: increasing, 1: decreasing): ");
    type = ReadChar();

    while (type != '0' && type != '1')
    {
        PrintString("Invalid type. Please enter again (0: increasing, 1: decreasing): ");
        type = ReadChar();
    }

    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if ((type == '0' && A[j] > A[j + 1]) || (type == '1' && A[j] < A[j + 1]))
            {
                tmp = A[j];
                A[j] = A[j + 1];
                A[j + 1] = tmp;
            }
        }
    }

    PrintString("The sorted array is: ");
    for (i = 0; i < n; i++)
    {
        PrintNum(A[i]);
        PrintChar(' ');
    }

    PrintChar('\n');
    Halt();
}
