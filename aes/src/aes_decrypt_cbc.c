/////////////////////////////////////////////////////////////////////////////
//
// File: aes_decrypt_cbc.c
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
// void aes_decrypt_cbc(aes_data_t * aes_data_p, const unsigned char * input_cipher_text_block, unsigned char * output_plain_text_block);
//
// Description:
//  Decrypts an input block of cipher text into an output block of plain text using cipher-block chaining (CBC) mode
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
void aes_decrypt_cbc(aes_data_t * aes_data_p, const unsigned char * input_cipher_text_block, unsigned char * output_plain_text_block) __REENTRANT
{
	//The general algorithm for CBC decryption is
	// 1. Decrypt the cipher text
	// 2. XOR the IV (first decryption) or the previous cipher text block with the output of (1)
	// 3. Since the cipher text must be saved in aes_data_p->iv_or_intermediate_data, copy it
	aes_decrypt_ecb(aes_data_p, input_cipher_text_block, output_plain_text_block);
	util_xor_arrays(aes_data_p->iv_or_intermediate_data, output_plain_text_block, output_plain_text_block, 16);
	util_copy_array(input_cipher_text_block, aes_data_p->iv_or_intermediate_data, 16);
}
#endif
