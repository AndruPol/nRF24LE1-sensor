/////////////////////////////////////////////////////////////////////////////
//
// File: aes_encrypt_cbc.c
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


#if (AES_ALLOW_NON_ECB_BLOCK_CIPHER_MODES != 0)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// void aes_encrypt_cbc(aes_data_t * aes_data_p, const unsigned char * input_plain_text_block, unsigned char * output_cipher_text_block)
//
// Description:
//  Encrypts an input block of plain text into an output block of cipher text using cipher-block chaining (CBC) mode
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
void aes_encrypt_cbc(aes_data_t * aes_data_p, const unsigned char * input_plain_text_block, unsigned char * output_cipher_text_block) __REENTRANT
{
	//The general algorithm for CBC encryption is
	// 1. XOR the IV (first decryption) or the previous cipher text block with the input plain text block
	// 2. Encryp the result of (1)
	// 3. Since the cipher text must be saved in aes_data_p->iv_or_intermediate_data, copy it
	util_xor_arrays(aes_data_p->iv_or_intermediate_data, input_plain_text_block, aes_data_p->iv_or_intermediate_data, 16);
	aes_encrypt_ecb(aes_data_p, aes_data_p->iv_or_intermediate_data, aes_data_p->iv_or_intermediate_data);
	util_copy_array(aes_data_p->iv_or_intermediate_data, output_cipher_text_block, 16);
}
#endif
