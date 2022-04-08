#include "syscall.h"

int main() {
    char filename[100];
    int res;

    PrintString("Enter filename: ");
    ReadString(filename, 99);

    res = Remove(filename);
    if (res == -1) 
        PrintString("Fail to delete file\n");
    else
        PrintString("Successfully delete file\n");

    Halt();
}