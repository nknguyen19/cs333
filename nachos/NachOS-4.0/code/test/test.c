/*
 * test.c
 * test all the implemented system calls
*/

#include "syscall.h"

int 
main()
{
    int num;
    char str[255], ch;
    num = RandomNum();
    PrintString("Generate a random number: ");
    PrintNum(num); PrintChar('\n');

    PrintString("Please enter an integer: ");
    num = ReadNum();
    PrintString("Inputed integer: ");
    PrintNum(num); PrintChar('\n');

    PrintString("Please enter a character: ");
    ch = ReadChar();
    PrintString("Inputed character: ");
    PrintChar(ch); PrintChar('\n');
    
    ReadChar();
    PrintString("Please enter a string: ");
    ReadString(str, 100);
    PrintString("Inputed string: "); 
    PrintString(str); PrintChar('\n');
    
    Halt();
}

