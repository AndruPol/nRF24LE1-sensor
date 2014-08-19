/////////////////////////////////////////////////////////////////////////////
//
// File: aes_add_round_key.c
//
// Copyright S. Brennen Ball, 2010
//
// The author provides no guarantees, warantees, or promises, implied or
//  otherwise.  By using this software you agree to indemnify the author
//  of any damages incurred by using it.
//
/////////////////////////////////////////////////////////////////////////////

#include "aes.h"
//#include "aes_src.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// void aes_add_round_key(aes_data_t * aes_data_p, unsigned char round)
//
// Description:
//  Adds the round key at "round" in w[] to state[].
//
// Parameters:
//  aes_data_t * aes_data_p - data structure for this instance
//  unsigned char round - round of key to add
//
// Return value:
//  None
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void aes_add_round_key(aes_data_t * aes_data_p, unsigned char round) __REENTRANT
{
	unsigned char row, col;

	//loop through and add the appropriate state and round bytes
	for(row = 0; row < 4; row++)
	{
		for(col = 0; col < Nb; col++)
		{
			aes_data_p->state[row][col] = aes_data_p->state[row][col] ^ aes_data_p->w[row][(round * 4) + col];
		}
	}
}
