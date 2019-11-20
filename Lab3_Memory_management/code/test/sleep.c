#include "syscall.h"
main() {
    int i;
    for(i = 1; i < 6; i++) {
        Sleep(1000);
        PrintInt(i);
    }
    return 0;
}
