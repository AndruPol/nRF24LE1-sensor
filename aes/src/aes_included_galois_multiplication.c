/////////////////////////////////////////////////////////////////////////////
//
// File: aes_included_galois_multiplication.c
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
// unsigned char aes_included_galois_multiplication(unsigned char a, unsigned char b)
//
// Description:
//  Executes a Galois multiplication over the field GF(2^8).
//
// Parameters:
//  unsigned char a - Multiplier in Galois multiplication
//  unsigned char b - Multiplicand in Galois multiplication
//
// Return value:
//  Galois multiplication of a and b over the field GF(2^8)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char aes_included_galois_multiplication(unsigned char a, unsigned char b) __REENTRANT
{
    unsigned char p = 0;
	unsigned char counter;
	unsigned char hi_bit_set;

	for(counter = 0; counter < 8; counter++)
	{
		if((b & 1) == 1)
		{
			p ^= a;
		}

		hi_bit_set = (a & 0x80);
		a <<= 1;

		if(hi_bit_set == 0x80)
		{
			a ^= 0x1b;
		}

		b >>= 1;
	}

	return p;
}
