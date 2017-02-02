/////////////////////////////////////////////////////////////////////////////
//
// File: aes_decrypt_ecb.c
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
// void aes_decrypt_ecb(aes_data_t * aes_data_p, const unsigned char * input_cipher_text_block, unsigned char * output_plain_text_block)
//
// Description:
//  Decrypts an input block of cipher text into an output block of plain text using electronic codebook (ECB) mode
//
// Parameters:
//  aes_data_t * aes_data_p - data structure for this instance
//  const unsigned char * input_cipher_text_block - cipher text to be decrypted (the first 16 bytes of this array will be decrypted)
//  unsigned char * output_plain_text_block - decrypted plain text (must be at least 16 bytes in length)
//
// Return value:
//  None
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void aes_decrypt_ecb(aes_data_t * aes_data_p, const unsigned char * input_cipher_text_block, unsigned char * output_plain_text_block) __REENTRANT
{
	unsigned char row, col, round;

	//start out by copying the input cipher text onto the state
	for(row = 0; row < 4; row++)
	{
		for(col = 0; col < Nb; col++)
		{
			aes_data_p->state[row][col] = input_cipher_text_block[row + (4 * col)];
		}
	}

	//add the round key at round Nr
	aes_add_round_key(aes_data_p, aes_data_p->Nr);

	//To take this for loop all the way down to zero, the exit clause would look weird (either round < 255 or round != 255),
	// so we leave it out and just break when we have processed round zero.
	for(round = (aes_data_p->Nr - 1); ; round--)
	{
		aes_shift_rows_inv(aes_data_p);
		aes_sub_bytes_inv(aes_data_p);

		aes_add_round_key(aes_data_p, round);

		//we do not apply mix columns at iteration zero, but we do exit on iteration zero
		if(round != 0)
		{
			aes_mix_columns_inv(aes_data_p);
		}
		else
		{
			break;
		}
	}

	//copy the decrypted state onto the output plain text
	for(row = 0; row < 4; row++)
	{
		for(col = 0; col < Nb; col++)
		{
			output_plain_text_block[row + (4 * col)] = aes_data_p->state[row][col];
		}
	}
}
