#include "syscall.h"

main()
        {
                int     n;
                for (n=66;n<=168;n++)
                {
                    if(n==70) Sleep(2);
                    if (n%3==1)
                        { PrintInt(n); }
                }
        }
