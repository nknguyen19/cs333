#include "syscall.h"

int main() {
    char filename1[100];
    char filename2[100];
    char content[255];
    int id1, id2, res;
    
    // temp
    PrintString("Enter first filename: ");
    ReadString(filename1, 99);

    PrintString("Enter second filename: ");
    ReadString(filename2, 99);

    id1 = Open(filename1);
    id2 = Open(filename2);
    
    if (id1 == -1){
        PrintString("Unable to open the first file\n");
        Halt();
    }

    if (id2 == -1){
        PrintString("Unable to open the second file\n");
        Halt();
    }

    
    res = Read(content, 250, id2);
    if (res == -1){
        PrintString("Unable to read the first file\n");
        Halt();
    }
    
    Seek(-1, id1);
    Write(content, res, id1);
    
    PrintString("Successfully concatenate file 2 to file 1\n");
    Close(id1);
    Close(id2);
    Halt();
}