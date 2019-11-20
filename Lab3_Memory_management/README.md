# Memory Management 

> [Homework slides](https://hackmd.io/@xlYUTygoRkyuQQlwXuWDWQ/ByRRNL6iH#/)

## Problem

Originally, if you execute the `matmult` and `sort`, NachOS will run out of memory.

![](https://i.imgur.com/5ERPT2T.png)

```c
/* matmult.c 
 *    Test program to do matrix multiplication on large arrays.
 *
 *    Intended to stress virtual memory system.
 *
 *    Ideally, we could read the matrices off of the file system,
 *	and store the result back to the file system!
 */

#include "syscall.h"

#define Dim 	20	/* sum total of the arrays doesn't fit in 
			 * physical memory 
			 */

int A[Dim][Dim];
int B[Dim][Dim];
int C[Dim][Dim];

int
main()
{
    int i, j, k;

    for (i = 0; i < Dim; i++)		/* first initialize the matrices */
	for (j = 0; j < Dim; j++) {
	     A[i][j] = i;
	     B[i][j] = j;
	     C[i][j] = 0;
	}

    for (i = 0; i < Dim; i++)		/* then multiply them together */
	for (j = 0; j < Dim; j++)
            for (k = 0; k < Dim; k++)
		 C[i][j] += A[i][k] * B[k][j];

    Exit(C[Dim-1][Dim-1]);		/* and then we're done */
}
```

Note: modify the original `sort.c` to bubble sort
```c
/* sort.c 
 *    Test program to sort a large number of integers.
 *
 *    Intention is to stress virtual memory system.
 *
 *    Ideally, we could read the unsorted array off of the file system,
 *	and store the result back to the file system!
 */

#include "syscall.h"

int A[1024];	/* size of physical memory; with code, we'll run out of space!*/

int
main()
{

    int i, j, tmp;

    /* first initialize the array, in reverse sorted order */
    for (i = 0; i < 1024; i++)      
        A[i] = 1024 - i;


    /* then sort! */
    for (i = 0; i < 1023; i++) {
        for (j = 0; j < (1023 - i); j++) {
            if (A[j] > A[j + 1]) {  /* out of order -> need to swap ! */
                tmp = A[j];
                A[j] = A[j + 1];
                A[j + 1] = tmp;
            }
        }
    }
    Exit(A[0]);		/* and then we're done -- should be 1! */
}
```

## Code

* `/userprog/userkernel.h`
    * Add a `SynchDisk` object to create a swap area.
* `/userprog/userkernel.cc`
    * In the `UserProgKernel` constructor, allocate swap area dynamically.
* `/machine/machine.h`
    * Define some public veriables to record which main memory frame has been used, ID, sector and frame number.
    * Futhermore, we need to record some informations for the page replacement algorithm implementation.
* `/userprog/addrspace.cc`
    1. Uncomment the assertion which will terminate the user program when number of page is greater then number of physical page then we can provide the virtual memory serivce.
    2. AddrSpace::Load
        * Use the main memory first, if it isn't enough store the remain pages to virtual memory which can be accessed by `kernel->Swap_area->WriteSector`
        * Mark them as invalid then the trap of page fault occur, NachOS will search it from virtual memory space.
    4. AddrSpace::SaveState
        * Save the process state when context switch and the page table is loaded.
* `/machine/translate.h`
    1. `LRU_times` is used to count which page is least used one in the main memory.
* `/machine/translate.cc`



###### tags: `Operating System` `NachOS`