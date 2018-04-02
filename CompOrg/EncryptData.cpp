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
		xor ebx, ebx; 
		xor edx, edx; //register for holding current byte of data (in dl)
		mov edi, data; //address of first element in data into edi

	READ_DATA:
		mov dl, byte ptr[edi + ecx]; //move current byte into dl
		nop;
	PART_C:
		shl dh, 1;
		rcr dl, 1;
		setc al;
		or dh, al;
		inc bl;
		cmp bl, 8;
		je END_C;
		jmp PART_C;
	END_C:		
		mov dl, dh;

	PART_B:
		shl dh, 4;
		shr dl, 4;
		or dl, dh;
	END_B:

	PART_E :
		lea esi, gkey;
		movzx ebx, dl;
		mov dl, byte ptr[esi+ebx];
	END_E:

	PART_D :
		xor ebx, ebx;
		mov dh, dl;
		jmp HIGH_NIBBLE;
	HIGH_NIBBLE:
		and dh, 0xF0;
		rcl dh,1;
		jnc LOWER_NIBBLE; //bypass fixing the one if 0
		or dh, 0x10;
	LOWER_NIBBLE:
		and dl, 0x0F;
		rcr dl, 1;
		jnc END_D; //bypass fixing the one if 0
		or dl, 8;
	END_D:
		or dl, dh;

	PART_A:
		rol dl, 1;
	END_A:

		mov byte ptr[edi + ecx], dl;
		inc ecx;
		cmp ecx, dataLength;
		je DONE;
		jmp READ_DATA;

	DONE:      

	}

	return resulti;
} // encryptData