#include "syscall.h"

int main() {
 /*Create a file*/
    if (Create("text.txt") == -1) {
        PrintString("Cannot create file!\n");
    }
    else {
        PrintString("Create file successfully!\n");  
    }
    Halt();
}