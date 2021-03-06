// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"

class Sleeper: public CallBackObj
{
public:
	Sleeper(Thread* _t):t(_t) {}

	void CallBack() { kernel->scheduler->ReadyToRun(t); }
private:
	Thread* t;
};

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which)
{
	int	type = kernel->machine->ReadRegister(2);
	int	val;

    switch (which) {
	case SyscallException:
	    switch(type) {
		case SC_Halt:
		    DEBUG(dbgAddr, "Shutdown, initiated by user program.\n");
   		    kernel->interrupt->Halt();
		    break;
		case SC_PrintInt:
			val=kernel->machine->ReadRegister(4);
			cout << "Print integer:" <<val << endl;
			return;
		case SC_Sleep:
			val = kernel->machine->ReadRegister(4);
			if(val <= 0)
				{ cout << "No effect for non-positive sleep time! (" << val << ")" << endl; return; }
			cout << "Sleep time: " << val << endl;
			kernel->alarm->WaitUntil(val);
			return;
/*		case SC_Exec:
			DEBUG(dbgAddr, "Exec\n");
			val = kernel->machine->ReadRegister(4);
			kernel->StringCopy(tmpStr, retVal, 1024);
			cout << "Exec: " << val << endl;
			val = kernel->Exec(val);
			kernel->machine->WriteRegister(2, val);
			return;
*/		case SC_Exit:
			DEBUG(dbgAddr, "Program exit\n");
			val=kernel->machine->ReadRegister(4);
			cout << "return value:" << val << endl;
			kernel->currentThread->Finish();
			break;
		default:
		    cerr << "Unexpected system call " << type << "\n";
 		    break;
	    }
	    break;

	case PageFaultException:
	{
		++kernel->stats->numPageFaults;
		int badVAddr = kernel->machine->ReadRegister(BadVAddrReg);
		//cout << "Deal with the page fault with VAddr " << badVAddr << endl;
		//cout << "(Begin) Deal with the page fault with VAddr " << badVAddr << endl;
		int vpn = badVAddr / PageSize;
		if(kernel->machine->pageTable[vpn].physicalPage >= 0 &&
		   kernel->machine->pageTable[vpn].physicalPage < NumPhysPages &&
		   kernel->machine->frameTable[kernel->machine->pageTable[vpn].physicalPage].pageTable != 0 &&
		   kernel->machine->frameTable[kernel->machine->pageTable[vpn].physicalPage].pageTable->dirty)
		{
			//cout << "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG " << kernel->currentThread << " " << kernel->machine->pageTable[vpn].physicalPage << endl;
			//cout << "This page is writing" << endl;
			//kernel->interrupt->Schedule(new Sleeper(kernel->currentThread), kernel->interrupt->nextTime(), DiskInt);
			//kernel->interrupt->OneTick();
			//return;
			//return;
			//kernel->alarm->WaitUntil(5);
		}
		int p; //page to swap in
		Thread* t = 0;
		if(kernel->machine->freePhysicalPage->empty())
		{
			//pick a victim
			p = 0;
			for(; p < NumPhysPages && (kernel->machine->lastFrame == p || kernel->machine->frameTable[p].inIO); ++p);
			if(p == NumPhysPages)
				return;//Abort();
			int minC = kernel->swapTable[kernel->machine->frameTable[p].pageTable->virtualPage];
			for(int i = p + 1; i < NumPhysPages; ++i)
				if((kernel->machine->lastFrame != i && !(kernel->machine->frameTable[i].inIO)) && minC > kernel->swapTable[kernel->machine->frameTable[i].pageTable->virtualPage])
					{ p = i; minC = kernel->swapTable[kernel->machine->frameTable[i].pageTable->virtualPage]; }
			//cout << "No free frame, pick frame " << p << endl;
			//ASSERT(kernel->machine->frameTable[p].inIO == FALSE);
			kernel->machine->frameTable[p].pageTable->valid = FALSE;
			//t = kernel->machine->frameTable[p].t;
			//cout << "sleep " << t << endl;
			//t->Sleep(FALSE);
/*
			if(kernel->scheduler->check(t))
			{
				kernel->scheduler->remove(t);
				t = 0;
			}
*/
			if(kernel->machine->frameTable[p].pageTable->dirty)
			{
				kernel->machine->frameTable[p].inIO = TRUE;
				//cout << "Write virtual page " << kernel->machine->frameTable[p].pageTable->virtualPage << " into swap" << endl;
				kernel->swapMemory->WriteSector(kernel->machine->frameTable[p].pageTable->virtualPage,
				                                kernel->machine->mainMemory + p * PageSize);
				kernel->machine->frameTable[p].inIO = FALSE;
				//kernel->machine->frameTable[p].pageTable->dirty = FALSE;
			}
            //kernel->swapTable[kernel->machine->frameTable[p]->virtualPage] = 0;
		}
		else
		{
			p = kernel->machine->freePhysicalPage->pop();
			//cout << "Free frame " << p << endl;
		}

		//ASSERT(kernel->machine->lastFrame != p);
//cout << (void*)kernel->machine->mainMemory << endl;
//cout << "haha begin " << p << endl;
//cout << &p << endl;
//int* pp = new int(p);
//cout << pp << endl;
//cout << (void*)(kernel->machine->mainMemory + p * PageSize) << endl;
//cout << p << endl;
		//ASSERT(kernel->machine->frameTable[p].inIO == FALSE);
		kernel->machine->frameTable[p].inIO = TRUE;
		//cout << "Read swap sector " << kernel->machine->pageTable[vpn].virtualPage << " into frame " << p << endl;
		kernel->swapMemory->ReadSector(kernel->machine->pageTable[vpn].virtualPage,
		                               kernel->machine->mainMemory + p * PageSize/*, &p*/);
		kernel->machine->frameTable[p].inIO = FALSE;
//cout << "hehe" << endl;
//cout << (void*)kernel->machine->mainMemory << endl;
//cout << "haha end " << p << endl;
//cout << *pp << endl;
//cout << pp << endl;
//cout << "hehe" << endl;
//cout << "hehe" << p << endl;
		kernel->machine->pageTable[vpn].physicalPage = p;
//cout << kernel->machine->pageTable << endl;
//cout << vpn << endl;
//cout << "hoho" << p << endl;
		kernel->machine->frameTable[p].pageTable = kernel->machine->pageTable + vpn;
//cout << kernel->machine->frameTable[p].pageTable->virtualPage << endl;
		kernel->machine->frameTable[p].pageTable->valid = TRUE;
		kernel->machine->frameTable[p].t = kernel->currentThread;
//cout << kernel->machine->frameTable[p].t << endl;
//cout << "hehe" << endl;
/*
		if(t != 0 && !kernel->scheduler->check(t))
		{
			IntStatus oldLevel = kernel->interrupt->SetLevel(IntOff);
			kernel->scheduler->ReadyToRun(t);
			kernel->interrupt->SetLevel(oldLevel);
		}
*/
		kernel->machine->lastFrame = p;
		if(kernel->replaceRule == FIFO)
			kernel->swapTable[kernel->machine->pageTable[vpn].virtualPage] = ++kernel->counter;
//cout << kernel->machine->frameTable[31] << endl;
		//ASSERT(kernel->machine->frameTable[p].pageTable != 0);
		//cout << "(End) Deal with the page fault with VAddr " << badVAddr << endl;
//cout << "hehe" << endl;
	} return;

	default:
		     switch(which)
{
             case ReadOnlyException:     // Write attempted to page marked
					cout << "ReadOnlyException" << endl; break;
					    // "read-only"
		     case BusErrorException:     // Translation resulted in an 
					cout << "ReadOnlyException" << endl; break;
					    // invalid physical address
		     case AddressErrorException: // Unaligned reference or one that
					cout << "AddressErrorException" << endl; break;
					    // was beyond the end of the
					    // address space
		     case OverflowException:     // Integer overflow in add or sub.
					cout << "OverflowException" << endl; break;
		     case IllegalInstrException: // Unimplemented or reserved instr.
					cout << "IllegalInstrException" << endl; break;
		     
		     case NumExceptionTypes:
					cout << "NumExceptionTypes" << endl; break;
}
	    cerr << "Unexpected user mode exception" << which << "\n";
	    break;
    }
    ASSERTNOTREACHED();
}
