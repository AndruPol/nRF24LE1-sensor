/////////////////////////////////////////////////////////////////////////////
//
// File: aes_sub_bytes_inv.c
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
// void aes_sub_bytes_inv(aes_data_t * aes_data_p)
//
// Description:
//  Substitutes the values in the state array with corresponding values from the sbox_inv array
//
// Parameters:
//  aes_data_t * aes_data_p - data structure for this instance
//
// Return value:
//  None
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void aes_sub_bytes_inv(aes_data_t * aes_data_p) __REENTRANT
{
	int row, col;

	//substitute each byte in the state array with its corresponding entry in sbox_inv[]
	for(row = 0; row < 4; row++)
	{
		for(col = 0; col < Nb; col++)
		{
			aes_data_p->state[row][col] = aes_get_sbox_inv(aes_data_p->state[row][col]);
		}
	}
}
