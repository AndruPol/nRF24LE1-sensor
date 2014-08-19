/////////////////////////////////////////////////////////////////////////////
//
// File: util.h
//
// Copyright S. Brennen Ball, 2010
//
// The author provides no guarantees, warantees, or promises, implied or
//  otherwise.  By using this software you agree to indemnify the author
//  of any damages incurred by using it.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef UTIL_H_
#define UTIL_H_





///////////////////////
// Function prototypes
///////////////////////
void util_xor_arrays(const unsigned char * input_block_a, const unsigned char * input_block_b, unsigned char * output_block, unsigned int len);
void util_copy_array(const unsigned char * input_block, unsigned char * output_block, unsigned int len);


#endif /* UTIL_H_ */
