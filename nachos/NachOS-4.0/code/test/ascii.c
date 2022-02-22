#include "syscall.h"

char* Dec2Hex(int dec){
    char hex[10];
    int tmp, i = 0;
    while (dec != 0){
        tmp = dec%16;
        if (tmp < 10){
            hex[i] = tmp + 48;
        }
        else{
            hex[i] = tmp + 55;
        }
        i++;
        dec/=16;
    } 
    return hex;
}

int main()
{
    int row, col, dec_tmp;
    int dec = 32;
    PrintString("======================= ASCII TABLE ======================\n\n");
    PrintString("| Decimal Hex Char | Decimal Hex Char | Decimal Hex Char |\n");
    PrintString("|------------------|------------------|------------------|\n");
    for (row = 0; row < 32; row++){
        PrintString("|");
        for(col = 0; col < 3; col++){
            dec_tmp = dec + col*32;
            PrintString(" ");
            PrintNum(dec_tmp);
            PrintString(dec_tmp < 100 ? "      " : "     ");
            PrintString(Dec2Hex(dec_tmp));
            PrintString("  ");
            PrintChar(dec_tmp != 127 ? dec_tmp : ' ');
            PrintString("    |");
        }
        PrintChar('\n');
        dec += 1;
    }
    Halt();
}