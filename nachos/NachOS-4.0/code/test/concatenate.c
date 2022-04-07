#include "syscall.h"

int main(int argc, char *argv[]) {
    char* filename1;
    char* filename2;
    char* content[255];
    int id1, id2, res;
    
    // temp
    filename1 = "text1.txt";
    filename2 = "text2.txt";

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
    Write(content, 250, id1);
    
    PrintString("Successfully concatenate file 2 to file 1\n");
    Halt();
}