/////////////////////////////////////////////////////////////////////////////
//
// File: aes_src.h
//
// Copyright S. Brennen Ball, 2010
//
// The author provides no guarantees, warantees, or promises, implied or
//  otherwise.  By using this software you agree to indemnify the author
//  of any damages incurred by using it.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef AES_SRC_H_
#define AES_SRC_H_

#include "aes.h"
#include "aes_user_options.h"


////////////////////////////////////////////////////////////////////////////////////////////////
// If the user is using SDCC and wants to force tables into code memory, use the __code keyword
////////////////////////////////////////////////////////////////////////////////////////////////
#if (AES_FORCE_TABLES_INTO_CODE_MEMORY_WITH_SDCC != 0)
#define CODE	__code
#else
#define CODE
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////
// If the user has defined their own multiplier, set it up here. Otherwise, use the built-in one.
//////////////////////////////////////////////////////////////////////////////////////////////////
#if (AES_USE_USER_DEFINED_GALOIS_MULTIPLY_FUNCTION != 0)
#define aes_galois_multiplication(a, b)	aes_user_defined_galois_multiplication(a, b)
#else
#define aes_galois_multiplication(a, b)	aes_included_galois_multiplication(a, b)
#endif


///////////////////////
// Function prototypes
///////////////////////
unsigned char aes_get_sbox(unsigned char index) __REENTRANT;
unsigned char aes_get_sbox_inv(unsigned char index) __REENTRANT;
unsigned char aes_get_rcon(unsigned char index) __REENTRANT;
void aes_rot_word(unsigned char * word) __REENTRANT;
void aes_sub_word(unsigned char * word) __REENTRANT;
void aes_shift_rows(aes_data_t * aes_data_p) __REENTRANT;
void aes_shift_rows_inv(aes_data_t * aes_data_p) __REENTRANT;
void aes_sub_bytes(aes_data_t * aes_data_p) __REENTRANT;
void aes_sub_bytes_inv(aes_data_t * aes_data_p) __REENTRANT;
void aes_add_round_key(aes_data_t * aes_data_p, unsigned char round) __REENTRANT;
unsigned char aes_included_galois_multiplication(unsigned char a, unsigned char b) __REENTRANT;
void aes_mix_columns(aes_data_t * aes_data_p) __REENTRANT;
void aes_mix_columns_inv(aes_data_t * aes_data_p) __REENTRANT;
void aes_key_expansion(aes_data_t * aes_data_p, unsigned char * key) __REENTRANT;

#endif
