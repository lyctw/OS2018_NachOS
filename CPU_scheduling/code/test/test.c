#include "syscall.h"

main() {
    int i;
    for(i = 0; i < 5; i++) {
        Sleep(2);
        PrintInt(i);
    }
    return 0;
}
