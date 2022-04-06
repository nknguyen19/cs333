#include "syscall.h"

int main (int argc, char** argv) {
    char* content[255];
    OpenFileId fileId = Open("text.txt"); 
    Read(content, 250, fileId);
    PrintString(content);
    Halt();
}