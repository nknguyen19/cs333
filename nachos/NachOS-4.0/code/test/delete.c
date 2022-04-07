#include "syscall.h"

int main(int argc, char *argv[]) {
    int res;
    res = Remove("text.txt");
    if (res == -1) 
        PrintString("Fail to delete file\n");
    else
        PrintString("Successfully delete file\n");

    Halt();
}