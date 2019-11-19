# Thread Management

> [Homework slides](https://hackmd.io/@xlYUTygoRkyuQQlwXuWDWQ/HJHT7nSvB#/)

## The issue to be solved...

Executing 2 program 

```shell
./nachos -e ../test/test1 -e ../test/test2
```

<details><summary>User application</summary>

test1.c
```c
#include "syscall.h"
main()
{
	int	n;
	for (n=9;n>5;n--)
		PrintInt(n);
}
``` 

test2.c
```c
#include "syscall.h"

main()
{
    int n;
    for (n=20;n<=25;n++)
    	PrintInt(n);
}

```

</details>



Output

```
Total threads number is 2
Thread ../test/test1 is executing.
Thread ../test/test2 is executing.
Print integer:9
Print integer:8
Print integer:7
Print integer:20
Print integer:21
Print integer:22
Print integer:23
Print integer:24
Print integer:6
Print integer:7
Print integer:8
Print integer:9
Print integer:10
Print integer:12
Print integer:13
Print integer:14
Print integer:15
Print integer:16
Print integer:16
Print integer:17
Print integer:18
Print integer:19
Print integer:20
Print integer:17
Print integer:18
Print integer:19
Print integer:20
Print integer:21
Print integer:21
Print integer:23
Print integer:24
Print integer:25
return value:0
Print integer:26
return value:0
No threads ready or runnable, and no pending interrupts.
Assuming the program completed.
Machine halting!

Ticks: total 800, idle 67, system 120, user 613
Disk I/O: reads 0, writes 0
Console I/O: reads 0, writes 0
Paging: faults 0
Network I/O: packets received 0, sent 0
```

## Problem


Initializing the address space in AddrSpace::AddrSpace

We should modify `addrspace.cc`, Since the initial code only provides one address space, and you must allocate a new address space for each user program.

### Linear Page Tables
With linear tables, the MMU splits a virtual address into page number and page offset components. The page number is used to index into an array of page table entries. The actual physical address is the concatenation of the page frame number in the page table entry and the page offset of the virtual address.

To use linear page tables, one simply initializes variable machine->pageTable to point to the page table used to perform translations. In general, each user process will have its own private page table. Thus, a process switch requires updating the pageTable variable. In a real machine, pageTable would correspond to a special register that would be saved and restored as part of the SWITCH() operation. The machine variable pageTableSize indicates the actual size of the page table.

Page table entries consist of the physical page frame number for the corresponding virtual page, a flag indicating whether the entry is currently valid (set by the OS, inspected by hardware), a flag indicating whether the page may be written (set by OS, inspected by hardware), a bit indicating whether the page has been referenced (set by the hardware, inspected and cleared by OS) and a dirty bit (set by hardware, inspected and cleared by OS).

The Nachos machine has NumPhysPages of physical memory starting at location mainMemory. Thus, page 0 starts at machine->mainMemory, while page N starts at $mainMemory + N * PageSize$



## Solution

`code/userprog/addrspace.h`

`code/userprog/addrspace.cc`

Provide an initial TLB

```c
pageTable = new TranslationEntry[numPages];

    for(unsigned int i = 0, j = 0; i < numPages; i++) {

        pageTable[i].virtualPage = i;
        while(j < NumPhysPages && AddrSpace::usedPhyPage[j] == true)
            j++;

        AddrSpace::usedPhyPage[j] = true;
        pageTable[i].physicalPage = j;
        pageTable[i].valid = true;
        pageTable[i].use = false;
        pageTable[i].dirty = false;
        pageTable[i].readOnly = false;

}
```

Calculate main memory address

```c
int idx = noffH.code.virtualAddr / PageSize;
int page_base   = pageTable[idx].physicalPage * PageSize;
int page_offset = noffH.code.virtualAddr % PageSize;
int main_mem_addr = page_base + page_offset;

executable->ReadAt(
    &(kernel->machine->mainMemory[main_mem_addr]), 
    noffH.code.size, 
    noffH.code.inFileAddr
);
```


## Result

Output

```
Total threads number is 2
Thread ../test/test1 is executing.
Thread ../test/test2 is executing.
Print integer:9
Print integer:8
Print integer:7
Print integer:20
Print integer:21
Print integer:22
Print integer:23
Print integer:24
Print integer:6
return value:0
Print integer:25
return value:0
No threads ready or runnable, and no pending interrupts.
Assuming the program completed.
Machine halting!

Ticks: total 300, idle 8, system 70, user 222
Disk I/O: reads 0, writes 0
Console I/O: reads 0, writes 0
Paging: faults 0
Network I/O: packets received 0, sent 0



## Reference

* http://people.cs.uchicago.edu/~odonnell/OData/Courses/CS230/NACHOS/code-userspace.html
