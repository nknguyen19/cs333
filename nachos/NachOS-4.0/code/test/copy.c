#include "syscall.h"

int main () {
    char content[255], src[100], des[100];
    OpenFileId srcId; 
    OpenFileId desId;
    int content_len;

    PrintString("Enter source file: ");
    ReadString(src, 99);
    PrintString("Enter destination file: ");
    ReadString(des, 99);

    if (Create(des) == -1) {
        PrintString("Cannot create destination file\n");
        Halt();
    }
    srcId = Open(src);
    desId = Open(des);


    if (srcId == -1) {
        PrintString("Cannot open source file\n");
        Halt();
    }  

    if (desId == -1) {
        PrintString("Cannot open destination file\n");
        Halt();
    }

    content_len = Read(content, 250, srcId);

    if (content_len == -1) {
        PrintString("Cannot read source fiele\n");
        Halt();
    }

    if (Write(content, content_len, desId) == -1) {
        PrintString("Cannot copy to destination file\n");
        Halt();
    }

    PrintString("Copy file successfully\n");

    Close(srcId);
    Close(desId);
    Halt();
}