/////////////////////////////////////////////////////////////////////////////
//
// File: aes.h
//
// Copyright S. Brennen Ball, 2010
//
// The author provides no guarantees, warantees, or promises, implied or
//  otherwise.  By using this software you agree to indemnify the author
//  of any damages incurred by using it.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef AES_H_
#define AES_H_

#include "util.h"
#include "aes_user_options.h"


///////////////////////////////////////////////////////////////////////////////
// Nb is the number of columns in the state matrix, which is always 4 with AES
///////////////////////////////////////////////////////////////////////////////
#define Nb		4

///////////////////////////////////////////////////
// Nk is the number of the 32-bit words in the key
///////////////////////////////////////////////////
#define Nk_128	4
#define Nk_192	6
#define Nk_256	8

///////////////////////////////////////////////////////
// Nr is the number of rounds to execute the algorithm
///////////////////////////////////////////////////////
#define Nr_128	10
#define Nr_192	12
#define Nr_256	14


////////////////////////////////////////////////////////////
// Enumeration to tell aes_initialize() how long the key is
////////////////////////////////////////////////////////////
typedef enum
{
	AES_KEY_LENGTH_128_BITS,
	AES_KEY_LENGTH_192_BITS,
	AES_KEY_LENGTH_256_BITS
} aes_key_length_t;

////////////////////////////////////////////////////////////
// Structure for holding data for an AES algorithm instance
////////////////////////////////////////////////////////////
typedef struct
{
	//For w, use the worst case size (i.e., that for 256 bit keys)
	unsigned char state[4][Nb];
#if 0 // save ram memory
//	unsigned char w[4][Nb * (Nr_256 + 1)];
#endif
	unsigned char w[4][Nb * (Nr_128 + 1)];

	//Nb is left out since it is always 4, there's no need to waste a byte of memory for it
	unsigned char Nk;
	unsigned char Nr;

	//Variable to hold initialization vector or intermediate data in CBC, CFB, and OFB modes
#if 0	//save ram memory
	unsigned char iv_or_intermediate_data[16];

	//Variable to hold a remaining bytes of key block data used for CFB and OFB modes
	unsigned int rem_key_bytes;
#endif
} aes_data_t;


///////////////////////
// Function prototypes
///////////////////////
void aes_initialize(aes_data_t * aes_data_p, aes_key_length_t aes_key_length, const unsigned char * key, const unsigned char * iv) __REENTRANT;

void aes_encrypt_ecb(aes_data_t * aes_data_p, const unsigned char * input_plain_text_block, unsigned char * output_cipher_text_block) __REENTRANT;
void aes_decrypt_ecb(aes_data_t * aes_data_p, const unsigned char * input_cipher_text_block, unsigned char * output_plain_text_block) __REENTRANT;

#if (AES_ALLOW_NON_ECB_BLOCK_CIPHER_MODES != 0)
 void aes_encrypt_cbc(aes_data_t * aes_data_p, const unsigned char * input_plain_text_block, unsigned char * output_cipher_text_block) __REENTRANT;
 void aes_decrypt_cbc(aes_data_t * aes_data_p, const unsigned char * input_cipher_text_block, unsigned char * output_plain_text_block) __REENTRANT;

 void aes_encrypt_cfb(aes_data_t * aes_data_p, const unsigned char * input_plain_text_block, unsigned char * output_cipher_text_block, unsigned int len) __REENTRANT;
 void aes_decrypt_cfb(aes_data_t * aes_data_p, const unsigned char * input_cipher_text_block, unsigned char * output_plain_text_block, unsigned int len) __REENTRANT;

 void aes_encrypt_ofb(aes_data_t * aes_data_p, const unsigned char * input_plain_text_block, unsigned char * output_cipher_text_block, unsigned int len) __REENTRANT;
 void aes_decrypt_ofb(aes_data_t * aes_data_p, const unsigned char * input_cipher_text_block, unsigned char * output_plain_text_block, unsigned int len) __REENTRANT;
#endif

#endif /* AES_H_ */
