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

## Virtual Memory

### Demand Paging

Demand paging follows that pages should only be brought into memory if the executing process demands them. This is often referred to as lazy evaluation as only those pages demanded by the process are swapped from secondary storage to main memory. 
Commonly, to achieve this process a page table implementation is used. The page table maps logical memory to physical memory. The page table uses a bitwise operator to mark if a page is valid or invalid. A valid page is one that currently resides in main memory. An invalid page is one that currently resides in secondary memory. When a process tries to access a page, the following steps are generally followed:

1. Attempt to access page.
    * If page is valid (in memory) then continue processing instruction as normal.
    * If page is invalid then a page-fault trap occurs.
2. Check if the memory reference is a valid reference to a location on secondary memory. If not, the process is terminated (illegal memory access). Otherwise, we have to page in the required page.
3. Schedule disk operation to read the desired page into main memory.
Restart the instruction that was interrupted by the operating system trap.

<!-- ![](https://i.imgur.com/gHsVvGu.png) -->

![](https://i.imgur.com/uw0wPw8.png)

### Page Replacemant

Page replacement happens when a requested page is not in memory (page fault) and a free frame cannot be used to satisfy the allocation. Therefore, we need to choose and swap out a *victim page (replaced page)* to the secondary memory (e.g. Disk).

![](https://i.imgur.com/wmgzsJE.png)


Page Replacemant Algo.

1. FIFO
2. LRU
3. LFU
4. MFU


## Code

* `/filesys/synchdisk.h`
    * Using semaphore to synchronize disk memory
* `/filesys/synchdisk.cc`
* `/userprog/userkernel.h`
    * Add a `SynchDisk` object to create a swap area.
    * `ReplaceRule` to decide which page replacement algo is used. 
    * Define **swapTable**, record the counter of virtual page, we will select a victim page based on the counter which is updated every memory access and page fault occur (see ``/machine/translate.cc``). 
* `/userprog/userkernel.cc`
    * `UserProgKernel::Initialize`, allocate swap memory area dynamically.
    * `UserProgKernel::UserProgKernel` command interface
        1. `-fifo`
        2. `-lru`
        3. `-lfu`
        4. `-mfu`
* `/machine/machine.h`
    * Define **frameTable** structure
* `/machine/machine.cc`
    * `Machine::Machine` allocate free physical pages and frameTable
* `/userprog/addrspace.cc`
    1. Uncomment the assertion which will terminate the user program when number of page is greater then number of physical page then we can provide the virtual memory serivce.
    2. AddrSpace::Load
        * Load a user program into memory from a file.
* `/machine/translate.h`
    1. `TranslationEntry` - Define **pageTable**
* `/machine/translate.cc`
    1. `Machine::Translate` will return `ExceptionType`, **translate a virtual address into a physical address**, check valid bit (if invalid, return `PageFaultException`) and update swapTable
    2. `TranslationEntry::init()` - Initialize the pageTable and map between the virtual and physical page


###### tags: `Operating System` `NachOS`