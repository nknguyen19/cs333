#include "syscall.h"

int main () {
    char content[255], filename[100];
    OpenFileId fileId; 

    PrintString("Enter filename: ");
    ReadString(filename, 99);
    fileId = Open(filename);
    if (fileId == -1) {
        PrintString("Cannot open file\n");
        Halt();
    }
    Read(content, 250, fileId);
    PrintString(content);
    Close(fileId);
    Halt();
}