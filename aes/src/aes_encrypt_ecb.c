/////////////////////////////////////////////////////////////////////////////
//
// File: aes_encrypt_ecb.c
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
// void aes_encrypt_ecb(aes_data_t * aes_data_p, const unsigned char * input_plain_text_block, unsigned char * output_cipher_text_block)
//
// Description:
//  Encrypts an input block of plain text into an output block of cipher text using electronic codebook (ECB) mode
//
// Parameters:
//  aes_data_t * aes_data_p - data structure for this instance
//  const unsigned char * input_plain_text_block - plain text to be encrypted (the first 16 bytes of this array will be encrypted)
//  unsigned char * output_cipher_text_block - encrypted cipher text (must be at least 16 bytes in length)
//
// Return value:
//  None
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void aes_encrypt_ecb(aes_data_t * aes_data_p, const unsigned char * input_plain_text_block, unsigned char * output_cipher_text_block) __REENTRANT
{
	unsigned char row, col, round;

	//start out by copying the input plain text onto the state
	for(row = 0; row < 4; row++)
	{
		for(col = 0; col < Nb; col++)
		{
			aes_data_p->state[row][col] = input_plain_text_block[row + (4 * col)];
		}
	}

	//add the round key at round 0
	aes_add_round_key(aes_data_p, 0);

	//Rather than having round only count up to Nr - 1 and then calling the functions (less aes_mix_columns()) again outside
	// the for() loop, I am counting all the way up to Nr, but not calling aes_mix_columns on the last iteration.
	for(round = 1; round <= aes_data_p->Nr; round++)
	{
		aes_sub_bytes(aes_data_p);
		aes_shift_rows(aes_data_p);

		//don't apply mix columns on the last iteration
		if(round <= (aes_data_p->Nr - 1))
		{
			aes_mix_columns(aes_data_p);
		}

		aes_add_round_key(aes_data_p, round);
	}

	//copy the encrypted state onto the output cipher text
	for(row = 0; row < 4; row++)
	{
		for(col = 0; col < Nb; col++)
		{
			output_cipher_text_block[row + (4 * col)] = aes_data_p->state[row][col];
		}
	}
}
