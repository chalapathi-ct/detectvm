#include <stdio.h>

#include "antivm.hpp"

int main()
{
    if (DetectVM::IsVMwareVM() == true)
    {
        printf("1");
    }
    else
    {
        printf("0");
    }
    return 0;
}
