# System call & CPU Scheduling

## Part1 - Sleep() system call

## Implementation

* `/code/userprog/syscall.h`
* `/code/test/start.s`
* `/code/userprog/exception.cc`
* `/code/threads/alarm.h`
* `/code/threads/alarm.cc`



### Test code

In `/code/test` create 2 test program, `test_sleep1.c` sleep for 50000 and `test_sleep2.c` sleep for 100000, then the "1" is printed 2 times the "2" shows up 1 time. 

test_sleep1.c

```c
#include "syscall.h"

main() {
	int i;
	for(i = 0; i < 10; i++) {
		Sleep(50000);
		PrintInt(1);
	}
}
```

test_sleep2.c

```c
#include "syscall.h"

main() {
	int i;
	for(i = 0; i < 10; i++) {
		Sleep(2 * 50000);
		PrintInt(2);
	}
}
```

#### Makefile

```
...
all: halt shell matmult sort test1 test2 test_sleep1 test_sleep2
...
test_sleep1: test_sleep1.o start.o
	$(LD) $(LDFLAGS) start.o test_sleep1.o -o test_sleep1.coff 
	../bin/coff2noff test_sleep1.coff test_sleep1

test_sleep2: test_sleep2.o start.o
	$(LD) $(LDFLAGS) start.o test_sleep2.o -o test_sleep2.coff 
	../bin/coff2noff test_sleep2.coff test_sleep2
```

### Output

```
yc@ubuntuNachos:~/nachos-4.0/code/userprog$ ./nachos -e ../test/test_sleep1 -e ../test/test_sleep2
Total threads number is 2
Thread ../test/test_sleep1 is executing.
Thread ../test/test_sleep2 is executing.
Sleep Time: 50000 (ms)
Alarm::WaitUntil go sleep
Sleep Time: 100000 (ms)
Alarm::WaitUntil go sleep
Bedroom::MorningCall Thread woken
Print integer:1
Sleep Time: 50000 (ms)
Alarm::WaitUntil go sleep
Bedroom::MorningCall Thread woken
Bedroom::MorningCall Thread woken
Print integer:1
Sleep Time: 50000 (ms)
Alarm::WaitUntil go sleep
Print integer:2
Sleep Time: 100000 (ms)
Alarm::WaitUntil go sleep
Bedroom::MorningCall Thread woken
Print integer:1
Sleep Time: 50000 (ms)
Alarm::WaitUntil go sleep
Bedroom::MorningCall Thread woken
Bedroom::MorningCall Thread woken
Print integer:1
Sleep Time: 50000 (ms)
Alarm::WaitUntil go sleep
Print integer:2
Sleep Time: 100000 (ms)
Alarm::WaitUntil go sleep
Bedroom::MorningCall Thread woken
Print integer:1
Sleep Time: 50000 (ms)
Alarm::WaitUntil go sleep
Bedroom::MorningCall Thread woken
Bedroom::MorningCall Thread woken
Print integer:1
Sleep Time: 50000 (ms)
Alarm::WaitUntil go sleep
Print integer:2
Sleep Time: 100000 (ms)
Alarm::WaitUntil go sleep
Bedroom::MorningCall Thread woken
Print integer:1
Sleep Time: 50000 (ms)
Alarm::WaitUntil go sleep
Bedroom::MorningCall Thread woken
Bedroom::MorningCall Thread woken
Print integer:1
Sleep Time: 50000 (ms)
Alarm::WaitUntil go sleep
Print integer:2
Sleep Time: 100000 (ms)
Alarm::WaitUntil go sleep
Bedroom::MorningCall Thread woken
Print integer:1
Sleep Time: 50000 (ms)
Alarm::WaitUntil go sleep
Bedroom::MorningCall Thread woken
Bedroom::MorningCall Thread woken
Print integer:1
return value:0
Print integer:2
return value:0
No threads ready or runnable, and no pending interrupts.
Assuming the program completed.
Machine halting!

Ticks: total 50000100, idle 49999415, system 300, user 385
Disk I/O: reads 0, writes 0
Console I/O: reads 0, writes 0
Paging: faults 0
Network I/O: packets received 0, sent 0
```

## Part2 - CPU Scheduling

Implement scheduling policies

* Round robin (Default)
* Fisrt come first serve
* Shortest job first
* Priority

### code

* `/code/threads/thread.h`
* `/code/threads/thread.cc`
* `/code/threads/kernel.cc`
* `/code/threads/kernel.h`
* `/code/threads/main.cc`
* `/code/threads/scheduler.h`
* `/code/threads/scheduler.cc`
* `/code/userprog/userkernel.h`
* `/cod/userprog/userkernel.cc`
* `/code/network/netkernel.h`
* `/code/network/netkernel.cc`

### Output

```
yc@ubuntuNachos:~/nachos-4.0/code/threads$ ./nachos FCFS
*** thread 0 looped 0 times
*** thread 1 looped 0 times
*** thread 0 looped 1 times
*** thread 1 looped 1 times
*** thread 0 looped 2 times
*** thread 1 looped 2 times
*** thread 0 looped 3 times
*** thread 1 looped 3 times
*** thread 1 looped 4 times
*** thread 0 looped 4 times
B: remaining 8
B: remaining 7
B: remaining 6
B: remaining 5
B: remaining 4
B: remaining 3
B: remaining 2
B: remaining 1
C: remaining 6
C: remaining 5
C: remaining 4
C: remaining 3
C: remaining 2
C: remaining 1
C: remaining 0
A: remaining 2
A: remaining 1
A: remaining 0
B: remaining 0
D: remaining 2
D: remaining 1
D: remaining 0
No threads ready or runnable, and no pending interrupts.
Assuming the program completed.
Machine halting!

Ticks: total 2800, idle 120, system 2680, user 0
Disk I/O: reads 0, writes 0
Console I/O: reads 0, writes 0
Paging: faults 0
Network I/O: packets received 0, sent 0
```

```
yc@ubuntuNachos:~/nachos-4.0/code/threads$ ./nachos RR
*** thread 0 looped 0 times
*** thread 1 looped 0 times
*** thread 0 looped 1 times
*** thread 1 looped 1 times
*** thread 0 looped 2 times
*** thread 1 looped 2 times
*** thread 0 looped 3 times
*** thread 1 looped 3 times
*** thread 1 looped 4 times
*** thread 0 looped 4 times
B: remaining 8
B: remaining 7
B: remaining 6
B: remaining 5
B: remaining 4
B: remaining 3
B: remaining 2
B: remaining 1
C: remaining 6
C: remaining 5
C: remaining 4
C: remaining 3
C: remaining 2
C: remaining 1
C: remaining 0
A: remaining 2
A: remaining 1
A: remaining 0
B: remaining 0
D: remaining 2
D: remaining 1
D: remaining 0
No threads ready or runnable, and no pending interrupts.
Assuming the program completed.
Machine halting!

Ticks: total 2800, idle 120, system 2680, user 0
Disk I/O: reads 0, writes 0
Console I/O: reads 0, writes 0
Paging: faults 0
Network I/O: packets received 0, sent 0
```

```
yc@ubuntuNachos:~/nachos-4.0/code/threads$ ./nachos SJF
*** thread 0 looped 0 times
*** thread 1 looped 0 times
*** thread 0 looped 1 times
*** thread 1 looped 1 times
*** thread 0 looped 2 times
*** thread 1 looped 2 times
*** thread 0 looped 3 times
*** thread 1 looped 3 times
*** thread 1 looped 4 times
*** thread 0 looped 4 times
A: remaining 2
A: remaining 1
A: remaining 0
D: remaining 2
D: remaining 1
D: remaining 0
C: remaining 6
C: remaining 5
C: remaining 4
C: remaining 3
C: remaining 2
C: remaining 1
C: remaining 0
B: remaining 8
B: remaining 7
B: remaining 6
B: remaining 5
B: remaining 4
B: remaining 3
B: remaining 2
B: remaining 1
B: remaining 0
No threads ready or runnable, and no pending interrupts.
Assuming the program completed.
Machine halting!

Ticks: total 2800, idle 120, system 2680, user 0
Disk I/O: reads 0, writes 0
Console I/O: reads 0, writes 0
Paging: faults 0
Network I/O: packets received 0, sent 0
```

```
yc@ubuntuNachos:~/nachos-4.0/code/threads$ ./nachos PRIORITY
*** thread 0 looped 0 times
*** thread 1 looped 0 times
*** thread 0 looped 1 times
*** thread 1 looped 1 times
*** thread 0 looped 2 times
*** thread 1 looped 2 times
*** thread 0 looped 3 times
*** thread 1 looped 3 times
*** thread 1 looped 4 times
*** thread 0 looped 4 times
B: remaining 8
B: remaining 7
B: remaining 6
B: remaining 5
B: remaining 4
B: remaining 3
B: remaining 2
B: remaining 1
B: remaining 0
D: remaining 2
D: remaining 1
D: remaining 0
C: remaining 6
C: remaining 5
C: remaining 4
C: remaining 3
C: remaining 2
C: remaining 1
C: remaining 0
A: remaining 2
A: remaining 1
A: remaining 0
No threads ready or runnable, and no pending interrupts.
Assuming the program completed.
Machine halting!

Ticks: total 2800, idle 120, system 2680, user 0
Disk I/O: reads 0, writes 0
Console I/O: reads 0, writes 0
Paging: faults 0
Network I/O: packets received 0, sent 0
```