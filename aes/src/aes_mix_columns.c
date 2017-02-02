/////////////////////////////////////////////////////////////////////////////
//
// File: aes_mix_columns.c
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
// void aes_mix_columns(aes_data_t * aes_data_p)
//
// Description:
//  Runs the mix columns algorithm
//
// Parameters:
//  aes_data_t * aes_data_p - data structure for this instance
//
// Return value:
//  None
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void aes_mix_columns(aes_data_t * aes_data_p) __REENTRANT
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

        aes_data_p->state[0][col] = aes_galois_multiplication(temp[0], 2) ^
									aes_galois_multiplication(temp[3], 1) ^
									aes_galois_multiplication(temp[2], 1) ^
									aes_galois_multiplication(temp[1], 3);

		aes_data_p->state[1][col] = aes_galois_multiplication(temp[1], 2) ^
									aes_galois_multiplication(temp[0], 1) ^
									aes_galois_multiplication(temp[3], 1) ^
									aes_galois_multiplication(temp[2], 3);

		aes_data_p->state[2][col] = aes_galois_multiplication(temp[2], 2) ^
									aes_galois_multiplication(temp[1], 1) ^
									aes_galois_multiplication(temp[0], 1) ^
									aes_galois_multiplication(temp[3], 3);

		aes_data_p->state[3][col] = aes_galois_multiplication(temp[3], 2) ^
									aes_galois_multiplication(temp[2], 1) ^
									aes_galois_multiplication(temp[1], 1) ^
									aes_galois_multiplication(temp[0], 3);
    }
}
