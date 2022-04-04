#include "syscall.h"

int main() {
    int len;
    char filename[101]; // maximum length of file name is 100
    OpenFileId fileId;
    char txt[10];

    /*Create a file*/
    // if (Create("text.txt") == -1) {
    //     PrintString("Cannot create file!\n");
    // }
    // else {
    //     PrintString("Create file successfully!\n");  
    // }


    fileId = Open("text.txt");
    Write("Hello", 5, fileId);
    Halt();
}