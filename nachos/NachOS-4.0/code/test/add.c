/* add.c
 *	Simple program to test whether the systemcall interface works.
 *	
 *	Just do a add syscall that adds two values and returns the result.
 *
 */

#include "syscall.h"

int
main()
{
  //int result = ReadNum();
  //char c = ReadChar();
  //PrintChar('h');

  char a[100];
  ReadString(a, 10);
  PrintString(a);
  Halt();
  /* not reached */
}
