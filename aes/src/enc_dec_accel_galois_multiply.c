/////////////////////////////////////////////////////////////////////////////
//
// File: enc_dec_accel_galois_multiply.c
//
// Copyright S. Brennen Ball, 2010
//
// The author provides no guarantees, warantees, or promises, implied or
//  otherwise.  By using this software you agree to indemnify the author
//  of any damages incurred by using it.
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
/////////////////////////////////////////////////////////////////////////////

#include "enc_dec_accel.h"
#include "reg24le1.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// static INLINE unsigned char enc_dec_accel_galois_multiply(unsigned char a, unsigned char b)
//
// Description:
//  Performs a GF(2^8) mutliplication in hardware and returns the result
//
// Parameters:
//  unsigned char a - multiplier in the operation
//  unsigned char b - multiplicand in the operation
//
// Return value:
//  Result of the GF(2^8) multiplication
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static INLINE unsigned char enc_dec_accel_galois_multiply(unsigned char a, unsigned char b)
{
	//Load the hardware registers for the operators
	CCPDATIA = a;
	CCPDATIB = b;

	//Return the result of the hardware GF(2^8) multiplication
	return CCPDATO;
}
