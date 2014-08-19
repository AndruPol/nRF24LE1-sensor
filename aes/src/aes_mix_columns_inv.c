/////////////////////////////////////////////////////////////////////////////
//
// File: aes_mix_columns_inv.c
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
// void aes_mix_columns_inv(aes_data_t * aes_data_p)
//
// Description:
//  Runs the inverted mix columns algorithm
//
// Parameters:
//  aes_data_t * aes_data_p - data structure for this instance
//
// Return value:
//  None
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void aes_mix_columns_inv(aes_data_t * aes_data_p) __REENTRANT
{
	unsigned char temp[4];
    unsigned char col;

    //go through each column and calculate the Galois multiplication
    for (col = 0; col < Nb; col++)
    {
		temp[0] = aes_data_p->state[0][col];
		temp[1] = aes_data_p->state[1][col];
		temp[2] = aes_data_p->state[2][col];
		temp[3] = aes_data_p->state[3][col];

        aes_data_p->state[0][col] = aes_galois_multiplication(temp[0], 14) ^
									aes_galois_multiplication(temp[3],  9) ^
									aes_galois_multiplication(temp[2], 13) ^
									aes_galois_multiplication(temp[1], 11);

		aes_data_p->state[1][col] = aes_galois_multiplication(temp[1], 14) ^
									aes_galois_multiplication(temp[0],  9) ^
									aes_galois_multiplication(temp[3], 13) ^
									aes_galois_multiplication(temp[2], 11);

		aes_data_p->state[2][col] = aes_galois_multiplication(temp[2], 14) ^
									aes_galois_multiplication(temp[1],  9) ^
									aes_galois_multiplication(temp[0], 13) ^
									aes_galois_multiplication(temp[3], 11);

		aes_data_p->state[3][col] = aes_galois_multiplication(temp[3], 14) ^
									aes_galois_multiplication(temp[2],  9) ^
									aes_galois_multiplication(temp[1], 13) ^
									aes_galois_multiplication(temp[0], 11);
    }
}
