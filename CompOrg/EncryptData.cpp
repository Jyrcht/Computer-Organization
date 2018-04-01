// EncryptData.cpp
//
// This file uses the input data and key information to encrypt the input data
//

#include "Main.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// code to encrypt the data as specified by the project assignment
int encryptData(char *data, int dataLength)
{
	int resulti = 0;

	gdebug1 = 0;					// a couple of global variables that could be used for debugging
	gdebug2 = 0;					// also can have a breakpoint in C code

	// You can not declare any local variables in C, but should use resulti to indicate any errors
	// Set up the stack frame and assign variables in assembly if you need to do so
	// access the parameters BEFORE setting up your own stack frame
	// Also, you cannot use a lot of global variables - work with registers

	__asm {
		// you will need to reference some of these global variables
		// (gptrPasswordHash or gPasswordHash), (gptrKey or gkey), gNumRounds
		
		xor ecx, ecx; //counter for reading in data
		xor ebx, ebx; //counter for ind. parts
		xor dh, dh;
		mov edi, data; //address of first element in data into edi
	READ_DATA:
		mov dl, byte ptr[edi + ecx]; //move current byte into dl
	PART_C:
		shl dh, 1;
		rcr dl, 1;
		setc al;
		add dh, al;
		inc bl;
		cmp bl, 8;
		je PART_B;
		jmp PART_C;
	PART_B:

		mov byte ptr[edi + ecx], dh;

		inc ecx;
		cmp ecx, dataLength;
		je DONE;
		jmp READ_DATA;

	DONE:      

	}

	return resulti;
} // encryptData