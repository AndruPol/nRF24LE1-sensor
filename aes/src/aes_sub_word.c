/////////////////////////////////////////////////////////////////////////////
//
// File: aes_sub_word.c
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
// void aes_sub_word(aes_data_t * aes_data_p)
//
// Description:
//  Substitutes the values in a word with corresponding values from the sbox array
//
// Parameters:
//  aes_data_t * aes_data_p - data structure for this instance
//
// Return value:
//  None
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void aes_sub_word(unsigned char * word) __REENTRANT
{
	//substitute each byte of "word" with its corresponding entry in sbox[]
	word[0] = aes_get_sbox(word[0]);
	word[1] = aes_get_sbox(word[1]);
	word[2] = aes_get_sbox(word[2]);
	word[3] = aes_get_sbox(word[3]);
}
