/////////////////////////////////////////////////////////////////////////////
//
// File: aes_user_options.h
//
// Copyright S. Brennen Ball, 2010
//
// The author provides no guarantees, warantees, or promises, implied or
//  otherwise.  By using this software you agree to indemnify the author
//  of any damages incurred by using it.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef AES_USER_OPTIONS_H_
#define AES_USER_OPTIONS_H_

/////////////////////////////////////////////////////////////
// User options (recompile library if these are changed)
/////////////////////////////////////////////////////////////
#define AES_FORCE_TABLES_INTO_CODE_MEMORY_WITH_SDCC		1	//Force the sbox, sbox_inv, and rcon tables into code memory using the __code keyword (only for SDCC)
#define AES_FORCE_FUNCTIONS_AS_REENTRANT_WITH_SDCC		0	//Force the aes_*_encrypt() and aes_*_decrypt() functions to be compiled as __reentrant
#define AES_USE_USER_DEFINED_GALOIS_MULTIPLY_FUNCTION	1	//Use section below to define an alternate Galois multiplier function than the one used in the library
#define AES_ALLOW_NON_ECB_BLOCK_CIPHER_MODES			0	//Allow the use of CBC, CFB, and OFB modes of operation in addition to ECB


///////////////////////////////////////////////////////////
// Definition for a user-defined Galois multiplier
///////////////////////////////////////////////////////////
#if (AES_USE_USER_DEFINED_GALOIS_MULTIPLY_FUNCTION != 0)
 //You must provide an alternate Galois multiply function that follows the following form (inlining the function is optional):
 //   [inline] unsigned char galois_multiply_function(unsigned char a, unsigned char b)
 // where a and b are the multiplier/muliplicand and the return value is the Galois product.
 extern unsigned char enc_dec_accel_galois_multiply(unsigned char a, unsigned char b);	//Extern function or include the header file for the function here
 #define aes_user_defined_galois_multiplication(a, b)	enc_dec_accel_galois_multiply(a, b)	//Change the name of the second function to your own
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Definition of __REENTRANT (use AES_FORCE_CRYPT_FUNCTIONS_REENTRANT_WITH_SDCC above to change...do not modify section below this comment!)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if (AES_FORCE_FUNCTIONS_AS_REENTRANT_WITH_SDCC != 0)
 #define __REENTRANT __reentrant
#else
 #define __REENTRANT
#endif


#endif /* AES_USER_OPTIONS_H_ */
