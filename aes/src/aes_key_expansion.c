/////////////////////////////////////////////////////////////////////////////
//
// File: aes_key_expansion.c
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
// void aes_key_expansion(aes_data_t * aes_data_p, unsigned char * key)
//
// Description:
//  Runs the key expansion algorithm
//
// Parameters:
//  aes_data_t * aes_data_p - data structure for this instance
//  const unsigned char * key - key to use for encryption/decryption
//
// Return value:
//  None
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void aes_key_expansion(aes_data_t * aes_data_p, unsigned char * key) __REENTRANT
{
	unsigned char col;
	unsigned char temp[4];

	//the first round key is the key itself
	for(col = 0; col < aes_data_p->Nk; col++)
	{
		aes_data_p->w[0][col] = key[(4 * col)];
		aes_data_p->w[1][col] = key[(4 * col) + 1];
		aes_data_p->w[2][col] = key[(4 * col) + 2];
		aes_data_p->w[3][col] = key[(4 * col) + 3];
	}

	//subsequent rounds are derived from the previous round's key
	while (col < (Nb * (aes_data_p->Nr + 1)))
	{
		//copy the previous column
		temp[0] = aes_data_p->w[0][col - 1];
		temp[1] = aes_data_p->w[1][col - 1];
		temp[2] = aes_data_p->w[2][col - 1];
		temp[3] = aes_data_p->w[3][col - 1];

		//calculate the key adder
		if((col % aes_data_p->Nk) == 0)
		{
			aes_rot_word(temp);
			aes_sub_word(temp);

			//Only the zeroth entry has an item in rcon, the other three are 0's.
			//A value XOR'ed with 0 is itself, so we only need to XOR the zeroth entry.
			temp[0] = temp[0] ^ aes_get_rcon(col / aes_data_p->Nk);
		}
		else if((aes_data_p->Nk > 6) && ((col % aes_data_p->Nk) == 4))
		{
			aes_sub_word(temp);
		}

		//add the calculated word to the corresponding word in the previous round key to get the current key
		aes_data_p->w[0][col] = aes_data_p->w[0][col - aes_data_p->Nk] ^ temp[0];
		aes_data_p->w[1][col] = aes_data_p->w[1][col - aes_data_p->Nk] ^ temp[1];
		aes_data_p->w[2][col] = aes_data_p->w[2][col - aes_data_p->Nk] ^ temp[2];
		aes_data_p->w[3][col] = aes_data_p->w[3][col - aes_data_p->Nk] ^ temp[3];

		col++;
	}
}
