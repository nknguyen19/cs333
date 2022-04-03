#include "syscall.h"

int main() {
    int len;
    char filename[101]; // maximum length of file name is 100

    /*Create a file*/
    if (Create("text.txt") == -1) {
        PrintString("Cannot create file!\n");
    }
    else {
        PrintString("Create file successfully!\n");  
    }
    Halt();
}