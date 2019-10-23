#include "syscall.h"

main() {
	int i;
	for(i = 0; i < 10; i++) {
		Sleep(50000);
		PrintInt(1);
	}
}