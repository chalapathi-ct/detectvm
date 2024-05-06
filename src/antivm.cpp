#include <stdio.h>

#include "antivm.hpp"

int main(){
    bool isVm = false;

    if (DetectVM::IsVboxVM() == true) {isVm = true; printf("1");}
    else if (DetectVM::IsVMwareVM() == true) {isVm = true; printf("1");}
    else if (DetectVM::IsMsHyperV() == true) {isVm = true; printf("1");}
    else { printf("0");}
    return 0;
}
