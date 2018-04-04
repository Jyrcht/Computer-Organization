// DecryptData.cpp
//
// This file uses the input data and key information to decrypt the input data
//
// CBEDA
// ADEBC

#include "Main.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// code to decrypt the data as specified by the project assignment
int decryptData(char *data, int dataLength)
{
	int resulti = 0;

	gdebug1 = 0;					// a couple of global variables that could be used for debugging
	gdebug2 = 0;					// also can have a breakpoint in C code

	// You can not declare any local variables in C, but should use resulti to indicate any errors
	// Set up the stack frame and assign variables in assembly if you need to do so
	// access the parameters BEFORE setting up your own stack frame
	// Also, you cannot use a lot of global variables - work with registers

	__asm {

		xor ecx, ecx; //counter for reading in data
		xor ebx, ebx;
		xor edx, edx; //register for holding current byte of data (in dl)
		mov edi, data; //address of first element in data into edi

	READ_DATA:
			mov dl, byte ptr[edi + ecx]; //move current byte into dl

	PART_A:
			ror dl, 1;
	END_A:

	PART_D :
			xor ebx, ebx;
			mov dh, dl;
		HIGH_NIBBLE:
			and dh, 0xF0;
			shr dh, 4;
			rcr dh, 1;
			jnc LOWER_NIBBLE; //bypass fixing the one if 0
			or dh, 0x8;
		LOWER_NIBBLE:
			and dl, 0x0F;
			shl dl, 4;
			rcl dl, 1;
			jnc END_D; //bypass fixing the one if 0
			or dl, 0x10;
	END_D:
			shl dh, 4;
			shr dl, 4;
			or dl, dh;

	PART_E:
			lea esi, gDecodeTable;
			movzx edx, dl;
			mov dl, byte ptr[esi + edx];

	END_E:

			mov dh, dl;
	PART_B:
			shl dh, 4;
			shr dl, 4;
			or dl, dh;
	END_B:

			xor ebx, ebx;
	PART_C:
			shl dh, 1;
			rcr dl, 1;
			setc al;
			or dh, al;
			inc bl;
			cmp bl, 8;
			jne PART_C;
	END_C:
			mov dl, dh;

			mov byte ptr[edi + ecx], dl;
			inc ecx;
			cmp ecx, dataLength;
			jne READ_DATA;


	}

	return resulti;
} // decryptData

