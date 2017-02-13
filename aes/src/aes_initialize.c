/////////////////////////////////////////////////////////////////////////////
//
// File: aes_initialize.c
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
#include <stddef.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// void aes_initialize(aes_data_t * aes_data_p, aes_key_length_t aes_key_length, const unsigned char * key, const unsigned char * iv)
//
// Description:
//  Initialize aes_data_p.  This function only needs to be called before using aes_data_p the first time and then at any time the
//   key or key length is changed.
//
// Parameters:
//  aes_data_t * aes_data_p - data structure for this instance
//  aes_key_length_t aes_key_length - enumeration entry for the number of bits in the key
//  const unsigned char * key - key to use for encryption/decryption
//  const unsigned char * iv - only used with non-ECB modes, this is the initialization vector for encryption/decryption (set to
//                             NULL to not use)
//
// Return value:
//  Galois multiplication of a and b over the field GF(2^8)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void aes_initialize(aes_data_t * aes_data_p, aes_key_length_t aes_key_length, const unsigned char * key, const unsigned char * iv) __REENTRANT
{
	unsigned char i;

	//set up the constants to allow all 3 key lengths. Nb is not set here since it's always 4 for AES.
	if(aes_key_length == AES_KEY_LENGTH_128_BITS)
	{
		aes_data_p->Nk = Nk_128;
		aes_data_p->Nr = Nr_128;
	}
	else if(aes_key_length == AES_KEY_LENGTH_192_BITS)
	{
		aes_data_p->Nk = Nk_192;
		aes_data_p->Nr = Nr_192;
	}
	else
	{
		//default to 256 bit key length
		aes_data_p->Nk = Nk_256;
		aes_data_p->Nr = Nr_256;
	}

	//run the key expansion algorithm
	aes_key_expansion(aes_data_p, key);

#if 0 //save memory
	//Set up the initialization vector if provided
	if(iv != NULL)
	{
		for(i = 0; i < 16; i++)
		{
			aes_data_p->iv_or_intermediate_data[i] = iv[i];
		}
	}
	aes_data_p->rem_key_bytes = 0;
#endif
}
