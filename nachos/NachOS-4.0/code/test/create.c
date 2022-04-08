#include "syscall.h"

int main() {
    /*Create a file*/
    char filename[100];
    PrintString("Enter filename: ");
    ReadString(filename, 99);

    if (Create(filename) == -1) {
        PrintString("Cannot create file!\n");
    }
    else {
        PrintString("Create file successfully!\n");  
    }
    Halt();
}