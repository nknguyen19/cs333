#include "syscall.h"

int main (int argc, char** argv) {
    char* content[255];
    OpenFileId srcId; 
    OpenFileId desId;
    int content_len;
    if (Create("text2.txt") == -1) {
        PrintString("Cannot create destination file\n");
        Halt();
    }
    srcId = Open("text.txt");
    desId = Open("text2.txt");

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

    if (Write(content, content_len, desId)) {
        PrintString("Cannot copy to destination file\n");
        Halt();
    }

    PrintString("Copy file successfully\n");

    PrintNum(srcId);
    Halt();
}