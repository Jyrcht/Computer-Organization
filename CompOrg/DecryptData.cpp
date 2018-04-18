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

	__asm {

			xor ecx, ecx   //counter for reading in data 
			xor eax, eax   //starting point + other
			xor ebx, ebx   //hop count + other
			xor esi, esi
			xor edx, edx   //register for holding current byte of data (in dl)
			mov edi, data

			mov ecx, gNumRounds

	ROUNDS :
			dec ecx
			push ecx //push current round number to stack [esp]	
			lea esi, gPasswordHash
			//Starting point in ax
			mov ah, [esi + ecx * 4]
			mov al, [esi + 1 + ecx * 4]
			//Hop count in bx
			mov bh, [esi + 2 + ecx * 4]
			mov bl, [esi + 3 + ecx * 4]
			xor ecx, ecx //Set ecx back to 0 for inner loop (before test because flags)
			test bx, bx
			jne READ_DATA
			mov bx, 0xFFFF

	READ_DATA:
			mov dl, byte ptr[edi + ecx]; //move current byte into dl

			push ebp //old ebp, located at [ebx]
			mov ebp, esp //esp in ebx
			push eax //[ebx-4] index saved on stack
			push ebx //[ebx-8] hop count saved on stack CURRENT ESP IS HERE

	BIT_MANIPULATION:


	PART_A:
			ror dl, 1;

	PART_D :
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

			mov dh, dl;
	PART_B:
			shl dh, 4;
			shr dl, 4;
			or dl, dh;

			xor eax, eax;
			xor ebx, ebx;
	PART_C:
			shl dh, 1;
			rcr dl, 1;
			setc al;
			or dh, al;
			inc bl;
			cmp bl, 8;
			jne PART_C;

			mov dl, dh;

	HOPPING:
			pop ebx
			pop eax
			pop ebp

			lea esi, gkey
			xor dl, [esi + eax] //xor with keyfile[index]
			add eax, ebx //add the hop count
			cmp eax, 0x10001 //cmp with 65537
			jnae NEXT
			sub eax, 0x10001

	NEXT:
			//Loop back for inner loop
			mov byte ptr[edi + ecx], dl;
			inc ecx;
			cmp ecx, dataLength;
			jne READ_DATA;
			//loop back for outer loop
			pop ecx
			test ecx, ecx
			jne ROUNDS

	}

	return resulti;
} // decryptData

