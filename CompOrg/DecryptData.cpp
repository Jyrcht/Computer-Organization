// DecryptData.cpp
//
// This file uses the input data and key information to decrypt the input data
//
// Let's test something over here eh?

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
		// you will need to reference some of these global variables
		// (gptrPasswordHash or gPasswordHash), (gptrKey or gkey), gNumRounds

		xor ecx, ecx //set ecx to zero
		mov edi, data //address of first element in data into edi

		READ_DATA:     //Label for the beginning of the loop that reads the data, byte for byte
			xor byte ptr[edi + ecx], 1 //xor current byte with 1
			inc ecx //counter++
			cmp ecx, dataLength //check for eof
			je DONE //exit if eof
			jmp READ_DATA //jumps to beginning of loop

		DONE:     //Label for exiting the READ_DATA loop

	}

	return resulti;
} // decryptData

