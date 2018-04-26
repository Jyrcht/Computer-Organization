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

	__asm {

		xor ecx, ecx   //counter for reading in data 
		xor eax, eax   //starting point + other
		xor ebx, ebx   //hop count + other
		xor esi, esi 
		xor edx, edx   //register for holding current byte of data (in dl)
		mov edi, data  //first byte of data into edi

	//Outer loop that handles the curent round up to a max of three
	ROUNDS:
		/*
		Save the current round number to stack and use the password to get the current starting point and hop count
		Sets ecx to 0 before moving into inner for-loop
		*/
			push ecx //push current round number to stack [esp]	
			lea esi, gPasswordHash
			//Starting point in ax
			mov ah, [esi + ecx*4]
			mov al, [esi + 1 + ecx*4]
			//Hop count in bx
			mov bh, [esi + 2 + ecx*4]
			mov bl, [esi + 3 + ecx*4]
			xor ecx, ecx //Set ecx back to 0 for inner loop (before test because flags)
			test bx, bx
			jne READ_DATA
			mov bx, 0xFFFF
	//Inner loop that applies the encryption algorithm
	//label for beginning of loop, puts next byte of data into dl
	READ_DATA:
			movzx dl, byte ptr[edi + ecx] //move current byte into dl
	//Provide encryption based on user entered password
	HOPPING:
			lea esi, gkey
			xor dl, [esi+eax] //xor with keyfile[index]
			add eax, ebx //add the hop count
			cmp eax, 0x10001 //cmp with 65537
			jnae BIT_MANIPULATION
			sub eax, 0x10001
	/*
	Label for the 5 steps, part C reverses the bits, B swaps nibbles, 
	E swaps with encode table, D rol high nibble & ror low nibble, and Adoes a simple one simple rol
	*/
	BIT_MANIPULATION:
			push ebp //old ebp, located at [ebx]
			mov ebp, esp //esp in ebx
			push eax //[ebx-4] index saved on stack
			push ebx //[ebx-8] hop count saved on stack CURRENT ESP IS HERE

			xor eax, eax //free up eax and ebx for Part C
			xor ebx, ebx
	PART_C:
			shl dh, 1
			rcr dl, 1  //rcr, when done fully, the carry flag at each rotation is the reverse order of the byte
			setc al    //So naturally save the carry in al and set 0th bit in dh, upon looping back move it into the next bit position
			or dh, al
			inc bl
			cmp bl, 8
			jne PART_C		
			mov dl, dh
	PART_B:
			shl dh, 4
			shr dl, 4  //move lower nibble one nibble left and higher nibble one nibble right
			or dl, dh  //then add together
	PART_E:
			lea esi, gEncodeTable
			movzx edx, dl
			mov dl, byte ptr[esi + edx] //swap with encode...
	PART_D:
			mov dh, dl
		HIGH_NIBBLE:
			and dh, 0xF0     //clear lower nibble
			rcl dh,1
			jnc LOWER_NIBBLE //bypass fixing the one if 0
			or dh, 0x10
		LOWER_NIBBLE:
			and dl, 0x0F    //clear higher nibble
			rcr dl, 1
			jnc END_D //bypass fixing the one if 0
			or dl, 8
	END_D:
			or dl, dh //save result back into dl

	PART_A:
			rol dl, 1
	//End of Bit Manipulation

			//fix index & hop count while fixing stack
			pop ebx
			pop eax
			pop ebp
			//loop back for inner for loop
			mov byte ptr[edi+ ecx], dl
			inc ecx
			cmp ecx, dataLength
			jne READ_DATA 
	//End of inner loop
			//loop back for outer loop
			pop ecx 
			inc ecx //get round number and ++
			cmp ecx, gNumRounds
			jne ROUNDS
	//End of outer loop


	}

	return resulti;
} // encryptData