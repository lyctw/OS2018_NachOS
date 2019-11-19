#include "syscall.h"

main() {
	int i;
	for(i = 0; i < 5; i++) {
		Sleep(2 * 50000);
		PrintInt(2);
	}
}