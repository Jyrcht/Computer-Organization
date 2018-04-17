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

		xor ecx, ecx   //counter for reading in data 
		xor eax, eax   //starting point + other
		xor ebx, ebx   //hop count + other
		xor esi, esi 
		xor edx, edx   //register for holding current byte of data (in dl)
		mov edi, data

	ROUNDS:
		lea esi, gPasswordHash
		//Starting point in ax
		mov ah, [esi ]
		mov al, [esi + 1]
		//Hop count in bx
		mov bh, [esi + 2]
		mov bl, [esi + 3]
		lea esi, gkey
		test bx, bx
		jne READ_DATA
		mov bx, 0xFFFF

	READ_DATA:
			movzx dl, byte ptr[edi + ecx]; //move current byte into dl

	HOPPING:
			xor dl, [esi+eax] //xor with keyfile[index]
			add eax, ebx //add the hop count
			cmp eax, 0x10001 //cmp with 65537
			jnae BIT_MANIPULATION
			sub eax, 0x10001

			
	BIT_MANIPULATION:
			push ebp //old ebp, located at [ebx]
			mov ebp, esp //esp in ebx
			push eax //[ebx-4] index saved on stack
			push ebx //[ebx-8] hop count saved on stack CURRENT ESP IS HERE

			xor eax, eax //free up eax and ebx for Part C
			xor ebx, ebx
	PART_C:
			shl dh, 1;
			rcr dl, 1;
			setc al;
			or dh, al;
			inc bl;
			cmp bl, 8;
			jne PART_C;		
			mov dl, dh;
	PART_B:
			shl dh, 4;
			shr dl, 4;
			or dl, dh;
	PART_E:
			lea esi, gEncodeTable;
			movzx edx, dl;
			mov dl, byte ptr[esi + edx];
	PART_D:
			mov dh, dl;
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


			//beginning of loop back for inner for loop
			//fix index & hop count while fixing stack
			pop ebx
			pop eax
			pop ebp
			mov byte ptr[edi+ ecx], dl;
			inc ecx;
			cmp ecx, dataLength;
			jne READ_DATA;     

	}

	return resulti;
} // encryptData