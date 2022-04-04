/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__
#define __USERPROG_KSYSCALL_H__

#include "kernel.h"
#include "synchconsole.h"
#include "machine.h"
#include "filesys.h"

void SysHalt()
{
  kernel->interrupt->Halt();
}

int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

char SysReadChar()
{
  char c = kernel->synchConsoleIn->GetChar();
  return c;
}

void SysPrintChar(char c)
{
  kernel->synchConsoleOut->PutChar(c);
}

int SysReadNum()
{
  int num = 0;
  bool sign = 0;         // 0: positive number, 1: negative number
  bool isInteger = true; // check input is an integer or not
  char c = kernel->synchConsoleIn->GetChar();

  if (c == '-')
  {
    sign = 1;
    c = kernel->synchConsoleIn->GetChar(); // input number is negative
  }

  // examine all input character until EOL
  while (c != '\n')
  {
    if (c >= '0' && c <= '9')
    {
      long long newNum = (long long)num * 10 + c - '0';
      if ((!sign && newNum > 2147483647LL) || (sign && newNum > 2147483648LL))
      // input number is out range of integer type
      {
        isInteger = false;
      }
      else
        num = (int)newNum;
    }
    else // input does not contain digit
    {
      isInteger = false;
    }
    c = kernel->synchConsoleIn->GetChar();
  }

  if (!isInteger)
    return 0; // if input does not an integer, return 0
  return sign ? (-num) : num;
}

void SysPrintNum(int number)
{
  char *buffer = new char[11]; // store the digits of the number
  if (number < 0)              // print minus sign first if number is negative
  {
    kernel->synchConsoleOut->PutChar('-');
    number = -number;
  }

  if (number == 0) // input value is 0
  {
    kernel->synchConsoleOut->PutChar('0');
  }

  int len = 0;
  while (number)
  // store the digit from te number from left to right (least significant first)
  {
    buffer[len++] = (char)(number % 10 + '0');
    number /= 10;
  }

  for (int i = len - 1; i >= 0; --i)
    // print the stored digits in reverse order
    kernel->synchConsoleOut->PutChar(buffer[i]);
  // kernel->synchConsoleOut->PutChar('\n'); // put EOF at the end
}

unsigned int SysRandomNum()
{
  RandomInit(time(0));
  return RandomNumber();
}

void SysReadString(char *buffer, int length)
{
  memset(buffer, 0, length + 1);
  char c;
  for (int i = 0; i < length; i++)
  {
    do
    {
      c = kernel->synchConsoleIn->GetChar();
    } while (c == EOF);           // ignore EOF
    if (c == '\001' || c == '\n') // encounter enter, stop reading
      break;
    buffer[i] = c;
  }
}

void SysPrintString(char *buffer)
{
  int i = 0;
  while (buffer[i]) // stop when encounter '\0'
  {
    kernel->synchConsoleOut->PutChar(buffer[i]);
    i++;
  }
}

int SysOpenFile(char *filename) {
  DEBUG(dbgSys,kernel->fileSystem->countFiles);
  OpenFile* openFile = kernel->fileSystem->Open(filename);
  DEBUG(dbgSys,"openFile\n");
  DEBUG(dbgSys,kernel->fileSystem->countFiles);
  if (openFile != NULL) {
    return kernel->fileSystem->countFiles - 1;
  }
  else return -1;
}

int SysReadFile(int fileId, char* buffer, int size) {
  if (fileId >= kernel->fileSystem->countFiles || fileId < 0 || fileId == 1){ 
		return -1; // go wrong <-- if try open `out of domain` fileSystem (10 openfile) or try to read stdout
	}

	if (kernel->fileSystem->files[fileId] == NULL) {
		return -1;
	}
	if (fileId == 0){ //Read from stdin
		SysReadString(buffer, size);
    return size;
	}

  int res = (kernel->fileSystem->files[fileId]->Read(buffer, size));
			
	if (res > 0){
		return res;
	} else {
		return -1;
	}
}

int SysWriteFile(int fileId, char*buffer, int size) {
  if (fileId > kernel->fileSystem->countFiles || fileId < 0 || fileId == 0) { // `out of domain` filesys + try to write to stdin 
		return -1;
	}
			
	if (kernel->fileSystem->files[fileId] == NULL) { //file does not exist
		return -1;
	}
  
  if (fileId == 1) { // Print to stdout
		SysPrintString(buffer);
	}

  int res = (kernel->fileSystem->files[fileId]->Write(buffer, size));

  if (res > 0) {
    return res;
	} else {
    return -1;
  }
}

#endif /* ! __USERPROG_KSYSCALL_H__ */
