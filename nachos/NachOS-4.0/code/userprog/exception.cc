// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions
//	is in machine.h.
//----------------------------------------------------------------------
// void IncreasePC(){
// 	kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

/* Increase program counter */
void IncreasePC() 
{
	/* set previous programm counter (debugging only)*/
	kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

	/* set next programm counter for brach execution */
	kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
}

/* Copy buffer from user memory space to kernel memory space*/
char *User2System(int virtualAddr, int limit)
{
	char *kernelBuf = NULL;
	kernelBuf = new char[limit + 1];

	if (kernelBuf == NULL)
		return kernelBuf;

	memset(kernelBuf, 0, limit + 1);

	int oneChar;
	for (int i = 0; i < limit; i++)
	{
		kernel->machine->ReadMem(virtualAddr + i, 1, &oneChar);
		kernelBuf[i] = (char)oneChar;
		if (oneChar == 0)
			break;
	}

	return kernelBuf;
}

/* Coppy buffer from system memory space to user memory space */
int System2User(int virtualAddr, int length, char *buffer)
{
	if (length < 0)
		return -1;
	if (length == 0)
		return length;
	int oneChar = 0;
	int i = 0;
	do
	{
		oneChar = (int)buffer[i];
		kernel->machine->WriteMem(virtualAddr + i, 1, oneChar);
		i++;
	} while (i < length && oneChar != 0);

	return i;
}

void ExceptionHandler(ExceptionType which)
{
	int type = kernel->machine->ReadRegister(2);
	switch (which)
	{
	case NoException:
		return;

	case PageFaultException:
		DEBUG(dbgSys, "No valid translation found\n");

		SysHalt();

		ASSERTNOTREACHED();
		break;

	case ReadOnlyException:
		DEBUG(dbgSys, "Write attempted to page marked \"read-only\"\n");

		SysHalt();

		ASSERTNOTREACHED();
		break;

	case BusErrorException:
		DEBUG(dbgSys, "Translation resulted in an invalid physical address\n");

		SysHalt();

		ASSERTNOTREACHED();
		break;

	case AddressErrorException:
		DEBUG(dbgSys, "Unaligned reference or one that was beyond the end of the address space\n");
		SysHalt();
		ASSERTNOTREACHED();
		break;

	case OverflowException:
		DEBUG(dbgSys, "Integer overflow in add or sub\n");
		SysHalt();
		ASSERTNOTREACHED();
		break;

	case IllegalInstrException:
		DEBUG(dbgSys, "Unimplemented or reserved instr\n");
		SysHalt();
		ASSERTNOTREACHED();
		break;

	case NumExceptionTypes:
		DEBUG(dbgSys, "Numer Exception Types\n");
		SysHalt();
		ASSERTNOTREACHED();
		break;

	case SyscallException:
		switch (type)
		{
		case SC_Halt:
			DEBUG(dbgSys, "Shutdown, initiated by user program.\n");

			SysHalt();

			ASSERTNOTREACHED();
			break;

		case SC_Add:
			DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");
			int result;
			result = SysAdd(/* int op1 */ (int)kernel->machine->ReadRegister(4),
							/* int op2 */ (int)kernel->machine->ReadRegister(5));

			DEBUG(dbgSys, "Add returning with " << result << "\n");
			/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)result);
			IncreasePC();

			return;
			ASSERTNOTREACHED();
			break;

		case SC_ReadNum:
		{
			DEBUG(dbgSys, "ReadNum\n");
			/* Process SysReadNum Systemcall */
			int result = SysReadNum();

			DEBUG(dbgSys, "ReadNum returning with " << result << "\n");
			/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)result);

			/* Update Program Counter value */
			IncreasePC();

			return;
			ASSERTNOTREACHED();
			break;
		}

		case SC_PrintNum:
		{
			DEBUG(dbgSys, "PrintNum\n");
			// read first argument from the register
			int number = kernel->machine->ReadRegister(4);

			DEBUG(dbgSys, "PrintNum printing " << number);
			// Process SysPrintNum Systemcall
			SysPrintNum(number);

			// Update ProgeamCounter value
			IncreasePC();

			return;
			ASSERTNOTREACHED();
			break;
		}

		case SC_RandomNum:
		{
			kernel->machine->WriteRegister(2, SysRandomNum());
			IncreasePC();

			return;
			ASSERTNOTREACHED();
			break;
		}

		case SC_ReadChar:
		{
			char c = SysReadChar();
			kernel->machine->WriteRegister(2, c);
			DEBUG(dbgSys, "Read char\n");

			IncreasePC();
			return;
			// ASSERTNOTREACHED();
			break;
		}

		case SC_PrintChar:
		{
			char c = kernel->machine->ReadRegister(4);
			SysPrintChar(c);
			DEBUG(dbgSys, "Print char\n");

			IncreasePC();
			return;
			ASSERTNOTREACHED();
			break;
		}

		case SC_ReadString:
		{
			DEBUG(dbgSys, "Read String\n");

			int virtualAddr = kernel->machine->ReadRegister(4);
			int length = kernel->machine->ReadRegister(5);
			char *buffer = User2System(virtualAddr, length); // copy string from user memory space to system memory space
			// char* buffer = new char [length + 1];
			SysReadString(buffer, length);			  // system reads string
			System2User(virtualAddr, length, buffer); // send back string to user memory space

			delete[] buffer;

			IncreasePC();
			return;
			ASSERTNOTREACHED();
			break;
		}

		case SC_PrintString:
		{
			DEBUG(dbgSys, "Print String\n");

			int virtualAddr = kernel->machine->ReadRegister(4);
			char *buffer = User2System(virtualAddr, 255);
			SysPrintString(buffer);

			delete[] buffer;

			IncreasePC();
			return;
			ASSERTNOTREACHED();
			break;
		}

		case SC_Create:
		{
			DEBUG(dbgSys,"SC_Create call ...\n");
			int virtAddr = kernel->machine->ReadRegister(4);
			char* filename = User2System(virtAddr, 100); // max length of file name is 100

			if (filename == NULL) {
				DEBUG(dbgSys,"Not enough memory in system\n");
				kernel->machine->WriteRegister(2,-1); // cannot create file, return -1
				delete[] filename;
				return;
			}

			if (!kernel->fileSystem->Create(filename)) {
				DEBUG(dbgSys,"Error in creating file\n");
				kernel->machine->WriteRegister(2,-1); // cannot create file, return -1
				delete[] filename;
				return;
			}

			kernel->machine->WriteRegister(2,0); // successfully create file, return 0
			delete[] filename;

			IncreasePC();
			return;
			ASSERTNOTREACHED();
			break;
		}

		case SC_Open:
		{
			DEBUG(dbgSys,"SC_Open call ...\n");
			int virtAddr = kernel->machine->ReadRegister(4);
			char* filename = User2System(virtAddr, 100); // max length of file name is 100

			if (filename == NULL) {
				DEBUG(dbgSys,"Not enough memory in system\n");
				kernel->machine->WriteRegister(2,-1); // cannot create file, return -1
				return;
			}

			int fileId = SysOpenFile(filename);
			if (fileId == -1) {
				DEBUG(dbgSys,"Cannot open file\n");
			}
			else {
				DEBUG(dbgSys,"Successfully open file\n");
			}
			kernel->machine->WriteRegister(2,fileId);
			// delete[] filename; No delete here, we need to save this filename in the File system

			IncreasePC();
			return;
			ASSERTNOTREACHED();
			break;
		}

		case SC_Close:
		{
			// int Close(OpenFileId id)
			DEBUG(dbgSys,"SC_Close call ...\n");

			int fileId = kernel -> machine -> ReadRegister(4);
			int result = SysCloseFile(fileId);
			kernel->machine->WriteRegister(2,result);

			IncreasePC();
			return;
			ASSERTNOTREACHED();
			break;
		}

		case SC_Read:
		{
			DEBUG(dbgSys,"SC_Read call ...\n");
			int virtualAddr = kernel->machine->ReadRegister(4);
			int size = kernel->machine->ReadRegister(5);
			int fileId = kernel->machine->ReadRegister(6);

			char *buffer = User2System(virtualAddr, size);

			int res = SysReadFile(fileId, buffer, size); // return number of bytes read, -1 if failed

			if (res != -1) {
				System2User(virtualAddr, size, buffer);
				kernel->machine->WriteRegister(2,res);
			}
			else kernel->machine->WriteRegister(2,-1);

			delete[] buffer;

			IncreasePC();
			return;
			ASSERTNOTREACHED();
			break;
		}

		case SC_Write:
		{
			DEBUG(dbgSys,"SC_Write call ...\n");
			int virtualAddr = kernel->machine->ReadRegister(4);
			int size = kernel->machine->ReadRegister(5);
			int fileId = kernel->machine->ReadRegister(6);

			char *buffer = User2System(virtualAddr, size);

			int res = SysWriteFile(fileId, buffer, size);

			if (res != -1) {
				DEBUG(dbgSys,"Write file successfully\n");
				System2User(virtualAddr, size, buffer);
				kernel->machine->WriteRegister(2,res);
			}
			else {
				DEBUG(dbgSys,"Error while writing file\n");
				kernel->machine->WriteRegister(2,-1);
			}

			delete[] buffer;

			IncreasePC();
			return;
			ASSERTNOTREACHED();
			break;
		}

		case SC_Seek:
		{
			// int Seek(int position, OpenFileId id);
			// return -1 if having errors, otherwise return true position
			DEBUG(dbgSys,"SC_Seek call ...\n");
			int pos = kernel->machine->ReadRegister(4);
			int fileId = kernel->machine->ReadRegister(5);

			kernel->machine->WriteRegister(2, SysSeekFile(pos, fileId));

			IncreasePC();
			return;
			ASSERTNOTREACHED();
			break;
		}

		case SC_Remove:
		{
			// int Remove(char *name)
			DEBUG(dbgSys,"SC_Remove call ...\n");
			int virtAddr = kernel->machine->ReadRegister(4);
			char* filename = User2System(virtAddr, 100);

			int result = SysRemoveFile(filename);
			kernel->machine->WriteRegister(2, result);

			IncreasePC();
			return;
			ASSERTNOTREACHED();
			break;
		}

		default:
			cerr << "Unexpected system call " << type << "\n";
			break;
		}
		break;
	default:
		cerr << "Unexpected user mode exception" << (int)which << "\n";
		break;
	}
	ASSERTNOTREACHED();
}
