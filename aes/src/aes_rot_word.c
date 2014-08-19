/////////////////////////////////////////////////////////////////////////////
//
// File: aes_rot_word.c
//
// Copyright S. Brennen Ball, 2010
//
// The author provides no guarantees, warantees, or promises, implied or
//  otherwise.  By using this software you agree to indemnify the author
//  of any damages incurred by using it.
//
/////////////////////////////////////////////////////////////////////////////

#include "aes.h"
#include "aes_src.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// void aes_rot_word(unsigned char * word)
//
// Description:
//  Rotates a word one byte to the left
//
// Parameters:
//  unsigned char * word - pointer to the 4-byte word to rotate
//
// Return value:
//  None
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void aes_rot_word(unsigned char * word) __REENTRANT
{
	unsigned char temp;

	//rotate word one byte to the left
	temp	= word[0];
	word[0] = word[1];
	word[1] = word[2];
	word[2] = word[3];
	word[3] = temp;
}
