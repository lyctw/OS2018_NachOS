# Thread Management

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



## Solution

`code/userprog/addrspace.h`

`code/userprog/addrspace.cc`