/////////////////////////////////////////////////////////////////////////////
//
// File: aes_shift_rows_inv.c
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
// void aes_shift_rows_inv(aes_data_t * aes_data_p)
//
// Description:
//  Shifts each row in the state by that row number of bytes to the right
//
// Parameters:
//  aes_data_t * aes_data_p - data structure for this instance
//
// Return value:
//  None
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void aes_shift_rows_inv(aes_data_t * aes_data_p) __REENTRANT
{
	unsigned char temp;

	//row 0 does not get rotated

	//rotate row 1 right by 1 column
	temp					= aes_data_p->state[1][0];
	aes_data_p->state[1][0] = aes_data_p->state[1][3];
	aes_data_p->state[1][3] = aes_data_p->state[1][2];
	aes_data_p->state[1][2] = aes_data_p->state[1][1];
	aes_data_p->state[1][1] = temp;

	//rotate row 2 right by 2 columns by switching items 0 and 2 and 1 and 3
	temp					= aes_data_p->state[2][0];
	aes_data_p->state[2][0] = aes_data_p->state[2][2];
	aes_data_p->state[2][2] = temp;
	temp					= aes_data_p->state[2][1];
	aes_data_p->state[2][1] = aes_data_p->state[2][3];
	aes_data_p->state[2][3] = temp;

	//rotate row 3 right by 3 columns
	temp					= aes_data_p->state[3][0];
	aes_data_p->state[3][0] = aes_data_p->state[3][1];
	aes_data_p->state[3][1] = aes_data_p->state[3][2];
	aes_data_p->state[3][2] = aes_data_p->state[3][3];
	aes_data_p->state[3][3] = temp;
}
