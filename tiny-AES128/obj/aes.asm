;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.6.0 #9615 (Linux)
;--------------------------------------------------------
	.module aes
	.optsdcc -mmcs51 --model-large
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _enc_dec_accel_galois_multiply
	.globl _AES128_ECB_decrypt_PARM_3
	.globl _AES128_ECB_decrypt_PARM_2
	.globl _AES128_ECB_encrypt_PARM_3
	.globl _AES128_ECB_encrypt_PARM_2
	.globl _Key
	.globl _RoundKey
	.globl _state
	.globl _AES128_ECB_encrypt
	.globl _AES128_ECB_decrypt
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
	.area RSEG    (ABS,DATA)
	.org 0x0000
;--------------------------------------------------------
; special function bits
;--------------------------------------------------------
	.area RSEG    (ABS,DATA)
	.org 0x0000
;--------------------------------------------------------
; overlayable register banks
;--------------------------------------------------------
	.area REG_BANK_0	(REL,OVR,DATA)
	.ds 8
;--------------------------------------------------------
; internal ram data
;--------------------------------------------------------
	.area DSEG    (DATA)
_KeyExpansion_sloc0_1_0:
	.ds 2
_KeyExpansion_sloc1_1_0:
	.ds 1
_KeyExpansion_sloc2_1_0:
	.ds 4
_KeyExpansion_sloc3_1_0:
	.ds 4
_KeyExpansion_sloc4_1_0:
	.ds 4
_MixColumns_sloc0_1_0:
	.ds 1
_MixColumns_sloc1_1_0:
	.ds 3
_MixColumns_sloc2_1_0:
	.ds 3
_InvMixColumns_sloc0_1_0:
	.ds 1
_InvMixColumns_sloc1_1_0:
	.ds 2
_InvMixColumns_sloc2_1_0:
	.ds 3
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
	.area	OSEG    (OVR,DATA)
_AddRoundKey_sloc0_1_0:
	.ds 1
_AddRoundKey_sloc1_1_0:
	.ds 1
	.area	OSEG    (OVR,DATA)
_BlockCopy_sloc0_1_0:
	.ds 3
;--------------------------------------------------------
; indirectly addressable internal ram data
;--------------------------------------------------------
	.area ISEG    (DATA)
;--------------------------------------------------------
; absolute internal ram data
;--------------------------------------------------------
	.area IABS    (ABS,DATA)
	.area IABS    (ABS,DATA)
;--------------------------------------------------------
; bit data
;--------------------------------------------------------
	.area BSEG    (BIT)
;--------------------------------------------------------
; paged external ram data
;--------------------------------------------------------
	.area PSEG    (PAG,XDATA)
;--------------------------------------------------------
; external ram data
;--------------------------------------------------------
	.area XSEG    (XDATA)
_state::
	.ds 3
_RoundKey::
	.ds 176
_Key::
	.ds 3
_getSBoxValue_num_1_24:
	.ds 1
_getSBoxInvert_num_1_26:
	.ds 1
_KeyExpansion_tempa_1_29:
	.ds 4
_AddRoundKey_round_1_38:
	.ds 1
_ShiftRows_temp_1_47:
	.ds 1
_xtime_x_1_48:
	.ds 1
_MixColumns_Tmp_1_51:
	.ds 1
_MixColumns_t_1_51:
	.ds 1
_InvMixColumns_a_1_54:
	.ds 1
_InvMixColumns_b_1_54:
	.ds 1
_InvMixColumns_c_1_54:
	.ds 1
_InvMixColumns_d_1_54:
	.ds 1
_InvShiftRows_temp_1_61:
	.ds 1
_BlockCopy_PARM_2:
	.ds 3
_BlockCopy_output_1_68:
	.ds 3
_AES128_ECB_encrypt_PARM_2:
	.ds 3
_AES128_ECB_encrypt_PARM_3:
	.ds 3
_AES128_ECB_encrypt_input_1_71:
	.ds 3
_AES128_ECB_decrypt_PARM_2:
	.ds 3
_AES128_ECB_decrypt_PARM_3:
	.ds 3
_AES128_ECB_decrypt_input_1_73:
	.ds 3
;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area XABS    (ABS,XDATA)
;--------------------------------------------------------
; external initialized ram data
;--------------------------------------------------------
	.area XISEG   (XDATA)
	.area HOME    (CODE)
	.area GSINIT0 (CODE)
	.area GSINIT1 (CODE)
	.area GSINIT2 (CODE)
	.area GSINIT3 (CODE)
	.area GSINIT4 (CODE)
	.area GSINIT5 (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area CSEG    (CODE)
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area HOME    (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area GSINIT  (CODE)
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME    (CODE)
	.area HOME    (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG    (CODE)
;------------------------------------------------------------
;Allocation info for local variables in function 'getSBoxValue'
;------------------------------------------------------------
;num                       Allocated with name '_getSBoxValue_num_1_24'
;------------------------------------------------------------
;	src/aes.c:147: static uint8_t getSBoxValue(uint8_t num)
;	-----------------------------------------
;	 function getSBoxValue
;	-----------------------------------------
_getSBoxValue:
	ar7 = 0x07
	ar6 = 0x06
	ar5 = 0x05
	ar4 = 0x04
	ar3 = 0x03
	ar2 = 0x02
	ar1 = 0x01
	ar0 = 0x00
	mov	a,dpl
	mov	dptr,#_getSBoxValue_num_1_24
	movx	@dptr,a
;	src/aes.c:149: return sbox[num];
	movx	a,@dptr
	mov	dptr,#_sbox
	movc	a,@a+dptr
	mov	dpl,a
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'getSBoxInvert'
;------------------------------------------------------------
;num                       Allocated with name '_getSBoxInvert_num_1_26'
;------------------------------------------------------------
;	src/aes.c:152: static uint8_t getSBoxInvert(uint8_t num)
;	-----------------------------------------
;	 function getSBoxInvert
;	-----------------------------------------
_getSBoxInvert:
	mov	a,dpl
	mov	dptr,#_getSBoxInvert_num_1_26
	movx	@dptr,a
;	src/aes.c:154: return rsbox[num];
	movx	a,@dptr
	mov	dptr,#_rsbox
	movc	a,@a+dptr
	mov	dpl,a
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'KeyExpansion'
;------------------------------------------------------------
;sloc0                     Allocated with name '_KeyExpansion_sloc0_1_0'
;sloc1                     Allocated with name '_KeyExpansion_sloc1_1_0'
;sloc2                     Allocated with name '_KeyExpansion_sloc2_1_0'
;sloc3                     Allocated with name '_KeyExpansion_sloc3_1_0'
;sloc4                     Allocated with name '_KeyExpansion_sloc4_1_0'
;i                         Allocated with name '_KeyExpansion_i_1_29'
;j                         Allocated with name '_KeyExpansion_j_1_29'
;k                         Allocated with name '_KeyExpansion_k_1_29'
;tempa                     Allocated with name '_KeyExpansion_tempa_1_29'
;------------------------------------------------------------
;	src/aes.c:158: static void KeyExpansion(void)
;	-----------------------------------------
;	 function KeyExpansion
;	-----------------------------------------
_KeyExpansion:
;	src/aes.c:164: for(i = 0; i < Nk; ++i)
	mov	r4,#0x00
	mov	r5,#0x00
	mov	r6,#0x00
	mov	r7,#0x00
00110$:
;	src/aes.c:166: RoundKey[(i * 4) + 0] = Key[(i * 4) + 0];
	mov	ar3,r4
	mov	a,r3
	add	a,r3
	add	a,acc
	mov	_KeyExpansion_sloc1_1_0,a
	add	a,#_RoundKey
	mov	_KeyExpansion_sloc0_1_0,a
	clr	a
	addc	a,#(_RoundKey >> 8)
	mov	(_KeyExpansion_sloc0_1_0 + 1),a
	mov	a,r4
	add	a,r4
	mov	_KeyExpansion_sloc2_1_0,a
	mov	a,r5
	rlc	a
	mov	(_KeyExpansion_sloc2_1_0 + 1),a
	mov	a,r6
	rlc	a
	mov	(_KeyExpansion_sloc2_1_0 + 2),a
	mov	a,r7
	rlc	a
	mov	(_KeyExpansion_sloc2_1_0 + 3),a
	mov	a,_KeyExpansion_sloc2_1_0
	add	a,_KeyExpansion_sloc2_1_0
	mov	_KeyExpansion_sloc2_1_0,a
	mov	a,(_KeyExpansion_sloc2_1_0 + 1)
	rlc	a
	mov	(_KeyExpansion_sloc2_1_0 + 1),a
	mov	a,(_KeyExpansion_sloc2_1_0 + 2)
	rlc	a
	mov	(_KeyExpansion_sloc2_1_0 + 2),a
	mov	a,(_KeyExpansion_sloc2_1_0 + 3)
	rlc	a
	mov	(_KeyExpansion_sloc2_1_0 + 3),a
	mov	dptr,#_Key
	movx	a,@dptr
	mov	r1,a
	inc	dptr
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	a,_KeyExpansion_sloc2_1_0
	add	a,r1
	mov	r1,a
	mov	a,(_KeyExpansion_sloc2_1_0 + 1)
	addc	a,r2
	mov	r2,a
	mov	dpl,r1
	mov	dph,r2
	mov	b,r3
	lcall	__gptrget
	mov	dpl,_KeyExpansion_sloc0_1_0
	mov	dph,(_KeyExpansion_sloc0_1_0 + 1)
	movx	@dptr,a
;	src/aes.c:167: RoundKey[(i * 4) + 1] = Key[(i * 4) + 1];
	mov	a,_KeyExpansion_sloc1_1_0
	inc	a
	add	a,#_RoundKey
	mov	_KeyExpansion_sloc0_1_0,a
	clr	a
	addc	a,#(_RoundKey >> 8)
	mov	(_KeyExpansion_sloc0_1_0 + 1),a
	mov	a,#0x01
	add	a,_KeyExpansion_sloc2_1_0
	mov	_KeyExpansion_sloc3_1_0,a
	clr	a
	addc	a,(_KeyExpansion_sloc2_1_0 + 1)
	mov	(_KeyExpansion_sloc3_1_0 + 1),a
	clr	a
	addc	a,(_KeyExpansion_sloc2_1_0 + 2)
	mov	(_KeyExpansion_sloc3_1_0 + 2),a
	clr	a
	addc	a,(_KeyExpansion_sloc2_1_0 + 3)
	mov	(_KeyExpansion_sloc3_1_0 + 3),a
	mov	dptr,#_Key
	movx	a,@dptr
	mov	r1,a
	inc	dptr
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	a,_KeyExpansion_sloc3_1_0
	add	a,r1
	mov	r1,a
	mov	a,(_KeyExpansion_sloc3_1_0 + 1)
	addc	a,r2
	mov	r2,a
	mov	dpl,r1
	mov	dph,r2
	mov	b,r3
	lcall	__gptrget
	mov	dpl,_KeyExpansion_sloc0_1_0
	mov	dph,(_KeyExpansion_sloc0_1_0 + 1)
	movx	@dptr,a
;	src/aes.c:168: RoundKey[(i * 4) + 2] = Key[(i * 4) + 2];
	mov	a,#0x02
	add	a,_KeyExpansion_sloc1_1_0
	add	a,#_RoundKey
	mov	_KeyExpansion_sloc3_1_0,a
	clr	a
	addc	a,#(_RoundKey >> 8)
	mov	(_KeyExpansion_sloc3_1_0 + 1),a
	mov	a,#0x02
	add	a,_KeyExpansion_sloc2_1_0
	mov	_KeyExpansion_sloc4_1_0,a
	clr	a
	addc	a,(_KeyExpansion_sloc2_1_0 + 1)
	mov	(_KeyExpansion_sloc4_1_0 + 1),a
	clr	a
	addc	a,(_KeyExpansion_sloc2_1_0 + 2)
	mov	(_KeyExpansion_sloc4_1_0 + 2),a
	clr	a
	addc	a,(_KeyExpansion_sloc2_1_0 + 3)
	mov	(_KeyExpansion_sloc4_1_0 + 3),a
	mov	dptr,#_Key
	movx	a,@dptr
	mov	r1,a
	inc	dptr
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	a,_KeyExpansion_sloc4_1_0
	add	a,r1
	mov	r1,a
	mov	a,(_KeyExpansion_sloc4_1_0 + 1)
	addc	a,r2
	mov	r2,a
	mov	dpl,r1
	mov	dph,r2
	mov	b,r3
	lcall	__gptrget
	mov	dpl,_KeyExpansion_sloc3_1_0
	mov	dph,(_KeyExpansion_sloc3_1_0 + 1)
	movx	@dptr,a
;	src/aes.c:169: RoundKey[(i * 4) + 3] = Key[(i * 4) + 3];
	mov	a,#0x03
	add	a,_KeyExpansion_sloc1_1_0
	add	a,#_RoundKey
	mov	_KeyExpansion_sloc4_1_0,a
	clr	a
	addc	a,#(_RoundKey >> 8)
	mov	(_KeyExpansion_sloc4_1_0 + 1),a
	mov	a,#0x03
	add	a,_KeyExpansion_sloc2_1_0
	mov	_KeyExpansion_sloc3_1_0,a
	clr	a
	addc	a,(_KeyExpansion_sloc2_1_0 + 1)
	mov	(_KeyExpansion_sloc3_1_0 + 1),a
	clr	a
	addc	a,(_KeyExpansion_sloc2_1_0 + 2)
	mov	(_KeyExpansion_sloc3_1_0 + 2),a
	clr	a
	addc	a,(_KeyExpansion_sloc2_1_0 + 3)
	mov	(_KeyExpansion_sloc3_1_0 + 3),a
	mov	dptr,#_Key
	movx	a,@dptr
	mov	r1,a
	inc	dptr
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	a,_KeyExpansion_sloc3_1_0
	add	a,r1
	mov	r1,a
	mov	a,(_KeyExpansion_sloc3_1_0 + 1)
	addc	a,r2
	mov	r2,a
	mov	dpl,r1
	mov	dph,r2
	mov	b,r3
	lcall	__gptrget
	mov	r1,a
	mov	dpl,_KeyExpansion_sloc4_1_0
	mov	dph,(_KeyExpansion_sloc4_1_0 + 1)
	movx	@dptr,a
;	src/aes.c:164: for(i = 0; i < Nk; ++i)
	inc	r4
	cjne	r4,#0x00,00142$
	inc	r5
	cjne	r5,#0x00,00142$
	inc	r6
	cjne	r6,#0x00,00142$
	inc	r7
00142$:
	clr	c
	mov	a,r4
	subb	a,#0x04
	mov	a,r5
	subb	a,#0x00
	mov	a,r6
	subb	a,#0x00
	mov	a,r7
	subb	a,#0x00
	jnc	00143$
	ljmp	00110$
00143$:
00115$:
;	src/aes.c:173: for(; (i < (Nb * (Nr + 1))); ++i)
	clr	c
	mov	a,r4
	subb	a,#0x2c
	mov	a,r5
	subb	a,#0x00
	mov	a,r6
	subb	a,#0x00
	mov	a,r7
	subb	a,#0x00
	jc	00144$
	ret
00144$:
;	src/aes.c:175: for(j = 0; j < 4; ++j)
	mov	ar0,r4
	mov	ar1,r5
	mov	ar2,r6
	mov	ar3,r7
	clr	a
	mov	_KeyExpansion_sloc4_1_0,a
	mov	(_KeyExpansion_sloc4_1_0 + 1),a
	mov	(_KeyExpansion_sloc4_1_0 + 2),a
	mov	(_KeyExpansion_sloc4_1_0 + 3),a
00112$:
;	src/aes.c:177: tempa[j]=RoundKey[(i-1) * 4 + j];
	push	ar4
	push	ar5
	push	ar6
	push	ar7
	mov	a,_KeyExpansion_sloc4_1_0
	add	a,#_KeyExpansion_tempa_1_29
	mov	r6,a
	mov	a,(_KeyExpansion_sloc4_1_0 + 1)
	addc	a,#(_KeyExpansion_tempa_1_29 >> 8)
	mov	r7,a
	mov	ar5,r0
	dec	r5
	mov	a,r5
	add	a,r5
	add	a,acc
	mov	r5,a
	mov	a,_KeyExpansion_sloc4_1_0
	mov	r4,a
	add	a,r5
	add	a,#_RoundKey
	mov	dpl,a
	clr	a
	addc	a,#(_RoundKey >> 8)
	mov	dph,a
	movx	a,@dptr
	mov	r5,a
	mov	dpl,r6
	mov	dph,r7
	movx	@dptr,a
;	src/aes.c:175: for(j = 0; j < 4; ++j)
	inc	_KeyExpansion_sloc4_1_0
	clr	a
	cjne	a,_KeyExpansion_sloc4_1_0,00145$
	inc	(_KeyExpansion_sloc4_1_0 + 1)
	cjne	a,(_KeyExpansion_sloc4_1_0 + 1),00145$
	inc	(_KeyExpansion_sloc4_1_0 + 2)
	cjne	a,(_KeyExpansion_sloc4_1_0 + 2),00145$
	inc	(_KeyExpansion_sloc4_1_0 + 3)
00145$:
	clr	c
	mov	a,_KeyExpansion_sloc4_1_0
	subb	a,#0x04
	mov	a,(_KeyExpansion_sloc4_1_0 + 1)
	subb	a,#0x00
	mov	a,(_KeyExpansion_sloc4_1_0 + 2)
	subb	a,#0x00
	mov	a,(_KeyExpansion_sloc4_1_0 + 3)
	subb	a,#0x00
	pop	ar7
	pop	ar6
	pop	ar5
	pop	ar4
	jc	00112$
;	src/aes.c:179: if (i % Nk == 0)
	mov	a,r0
	anl	a,#0x03
	jz	00148$
	ljmp	00108$
00148$:
;	src/aes.c:186: k = tempa[0];
	push	ar4
	push	ar5
	push	ar6
	push	ar7
	mov	dptr,#_KeyExpansion_tempa_1_29
	movx	a,@dptr
	mov	r7,a
	mov	_KeyExpansion_sloc4_1_0,r7
	mov	(_KeyExpansion_sloc4_1_0 + 1),#0x00
	mov	(_KeyExpansion_sloc4_1_0 + 2),#0x00
	mov	(_KeyExpansion_sloc4_1_0 + 3),#0x00
;	src/aes.c:187: tempa[0] = tempa[1];
	mov	dptr,#(_KeyExpansion_tempa_1_29 + 0x0001)
	movx	a,@dptr
	mov	r7,a
	mov	dptr,#_KeyExpansion_tempa_1_29
	movx	@dptr,a
;	src/aes.c:188: tempa[1] = tempa[2];
	mov	dptr,#(_KeyExpansion_tempa_1_29 + 0x0002)
	movx	a,@dptr
	mov	dptr,#(_KeyExpansion_tempa_1_29 + 0x0001)
	movx	@dptr,a
;	src/aes.c:189: tempa[2] = tempa[3];
	mov	dptr,#(_KeyExpansion_tempa_1_29 + 0x0003)
	movx	a,@dptr
	mov	dptr,#(_KeyExpansion_tempa_1_29 + 0x0002)
	movx	@dptr,a
;	src/aes.c:190: tempa[3] = k;
	mov	r6,_KeyExpansion_sloc4_1_0
	mov	dptr,#(_KeyExpansion_tempa_1_29 + 0x0003)
	mov	a,r6
	movx	@dptr,a
;	src/aes.c:198: tempa[0] = getSBoxValue(tempa[0]);
	mov	dpl,r7
	push	ar6
	push	ar5
	push	ar4
	push	ar3
	push	ar2
	push	ar1
	push	ar0
	lcall	_getSBoxValue
	mov	r7,dpl
	mov	dptr,#_KeyExpansion_tempa_1_29
	mov	a,r7
	movx	@dptr,a
;	src/aes.c:199: tempa[1] = getSBoxValue(tempa[1]);
	mov	dptr,#(_KeyExpansion_tempa_1_29 + 0x0001)
	movx	a,@dptr
	mov	dpl,a
	lcall	_getSBoxValue
	mov	r7,dpl
	mov	dptr,#(_KeyExpansion_tempa_1_29 + 0x0001)
	mov	a,r7
	movx	@dptr,a
;	src/aes.c:200: tempa[2] = getSBoxValue(tempa[2]);
	mov	dptr,#(_KeyExpansion_tempa_1_29 + 0x0002)
	movx	a,@dptr
	mov	dpl,a
	lcall	_getSBoxValue
	mov	r7,dpl
	mov	dptr,#(_KeyExpansion_tempa_1_29 + 0x0002)
	mov	a,r7
	movx	@dptr,a
;	src/aes.c:201: tempa[3] = getSBoxValue(tempa[3]);
	mov	dptr,#(_KeyExpansion_tempa_1_29 + 0x0003)
	movx	a,@dptr
	mov	dpl,a
	lcall	_getSBoxValue
	mov	r7,dpl
	pop	ar0
	pop	ar1
	pop	ar2
	pop	ar3
	pop	ar4
	pop	ar5
	pop	ar6
	mov	dptr,#(_KeyExpansion_tempa_1_29 + 0x0003)
	mov	a,r7
	movx	@dptr,a
;	src/aes.c:204: tempa[0] =  tempa[0] ^ Rcon[i/Nk];
	mov	dptr,#_KeyExpansion_tempa_1_29
	movx	a,@dptr
	mov	r7,a
	mov	a,r3
	clr	c
	rrc	a
	mov	r3,a
	mov	a,r2
	rrc	a
	mov	r2,a
	mov	a,r1
	rrc	a
	mov	r1,a
	mov	a,r0
	rrc	a
	mov	r0,a
	mov	a,r3
	clr	c
	rrc	a
	mov	r3,a
	mov	a,r2
	rrc	a
	mov	r2,a
	mov	a,r1
	rrc	a
	mov	r1,a
	mov	a,r0
	rrc	a
	mov	r0,a
	add	a,#_Rcon
	mov	dpl,a
	mov	a,r1
	addc	a,#(_Rcon >> 8)
	mov	dph,a
	clr	a
	movc	a,@a+dptr
	xrl	ar7,a
	mov	dptr,#_KeyExpansion_tempa_1_29
	mov	a,r7
	movx	@dptr,a
;	src/aes.c:173: for(; (i < (Nb * (Nr + 1))); ++i)
	pop	ar7
	pop	ar6
	pop	ar5
	pop	ar4
;	src/aes.c:213: tempa[3] = getSBoxValue(tempa[3]);
00108$:
;	src/aes.c:216: RoundKey[i * 4 + 0] = RoundKey[(i - Nk) * 4 + 0] ^ tempa[0];
	mov	ar3,r4
	mov	a,r3
	add	a,r3
	add	a,acc
	mov	r2,a
	add	a,#_RoundKey
	mov	_KeyExpansion_sloc4_1_0,a
	clr	a
	addc	a,#(_RoundKey >> 8)
	mov	(_KeyExpansion_sloc4_1_0 + 1),a
	dec	r3
	dec	r3
	dec	r3
	dec	r3
	mov	a,r3
	add	a,r3
	add	a,acc
	mov	r3,a
	add	a,#_RoundKey
	mov	dpl,a
	clr	a
	addc	a,#(_RoundKey >> 8)
	mov	dph,a
	movx	a,@dptr
	mov	r1,a
	mov	dptr,#_KeyExpansion_tempa_1_29
	movx	a,@dptr
	xrl	ar1,a
	mov	dpl,_KeyExpansion_sloc4_1_0
	mov	dph,(_KeyExpansion_sloc4_1_0 + 1)
	mov	a,r1
	movx	@dptr,a
;	src/aes.c:217: RoundKey[i * 4 + 1] = RoundKey[(i - Nk) * 4 + 1] ^ tempa[1];
	mov	a,r2
	inc	a
	add	a,#_RoundKey
	mov	_KeyExpansion_sloc4_1_0,a
	clr	a
	addc	a,#(_RoundKey >> 8)
	mov	(_KeyExpansion_sloc4_1_0 + 1),a
	mov	a,r3
	inc	a
	add	a,#_RoundKey
	mov	dpl,a
	clr	a
	addc	a,#(_RoundKey >> 8)
	mov	dph,a
	movx	a,@dptr
	mov	r1,a
	mov	dptr,#(_KeyExpansion_tempa_1_29 + 0x0001)
	movx	a,@dptr
	xrl	ar1,a
	mov	dpl,_KeyExpansion_sloc4_1_0
	mov	dph,(_KeyExpansion_sloc4_1_0 + 1)
	mov	a,r1
	movx	@dptr,a
;	src/aes.c:218: RoundKey[i * 4 + 2] = RoundKey[(i - Nk) * 4 + 2] ^ tempa[2];
	mov	a,#0x02
	add	a,r2
	add	a,#_RoundKey
	mov	_KeyExpansion_sloc4_1_0,a
	clr	a
	addc	a,#(_RoundKey >> 8)
	mov	(_KeyExpansion_sloc4_1_0 + 1),a
	mov	a,#0x02
	add	a,r3
	add	a,#_RoundKey
	mov	dpl,a
	clr	a
	addc	a,#(_RoundKey >> 8)
	mov	dph,a
	movx	a,@dptr
	mov	r1,a
	mov	dptr,#(_KeyExpansion_tempa_1_29 + 0x0002)
	movx	a,@dptr
	xrl	ar1,a
	mov	dpl,_KeyExpansion_sloc4_1_0
	mov	dph,(_KeyExpansion_sloc4_1_0 + 1)
	mov	a,r1
	movx	@dptr,a
;	src/aes.c:219: RoundKey[i * 4 + 3] = RoundKey[(i - Nk) * 4 + 3] ^ tempa[3];
	mov	a,#0x03
	add	a,r2
	add	a,#_RoundKey
	mov	r1,a
	clr	a
	addc	a,#(_RoundKey >> 8)
	mov	r2,a
	mov	a,#0x03
	add	a,r3
	add	a,#_RoundKey
	mov	dpl,a
	clr	a
	addc	a,#(_RoundKey >> 8)
	mov	dph,a
	movx	a,@dptr
	mov	r3,a
	mov	dptr,#(_KeyExpansion_tempa_1_29 + 0x0003)
	movx	a,@dptr
	mov	r0,a
	xrl	ar3,a
	mov	dpl,r1
	mov	dph,r2
	mov	a,r3
	movx	@dptr,a
;	src/aes.c:173: for(; (i < (Nb * (Nr + 1))); ++i)
	inc	r4
	cjne	r4,#0x00,00149$
	inc	r5
	cjne	r5,#0x00,00149$
	inc	r6
	cjne	r6,#0x00,00149$
	inc	r7
00149$:
	ljmp	00115$
;------------------------------------------------------------
;Allocation info for local variables in function 'AddRoundKey'
;------------------------------------------------------------
;round                     Allocated with name '_AddRoundKey_round_1_38'
;i                         Allocated with name '_AddRoundKey_i_1_39'
;j                         Allocated with name '_AddRoundKey_j_1_39'
;sloc0                     Allocated with name '_AddRoundKey_sloc0_1_0'
;sloc1                     Allocated with name '_AddRoundKey_sloc1_1_0'
;------------------------------------------------------------
;	src/aes.c:225: static void AddRoundKey(uint8_t round)
;	-----------------------------------------
;	 function AddRoundKey
;	-----------------------------------------
_AddRoundKey:
	mov	a,dpl
	mov	dptr,#_AddRoundKey_round_1_38
	movx	@dptr,a
;	src/aes.c:228: for(i=0;i<4;++i)
	movx	a,@dptr
	swap	a
	anl	a,#0xf0
	mov	r7,a
	mov	r6,#0x00
;	src/aes.c:230: for(j = 0; j < 4; ++j)
00109$:
	mov	a,r6
	mov	_AddRoundKey_sloc0_1_0,a
	add	a,acc
	add	a,acc
	mov	r4,a
	add	a,r7
	mov	r3,a
	mov	r2,#0x00
00103$:
;	src/aes.c:232: (*state)[i][j] ^= RoundKey[round * Nb * 4 + i * Nb + j];
	push	ar7
	mov	dptr,#_state
	movx	a,@dptr
	mov	r0,a
	inc	dptr
	movx	a,@dptr
	mov	r1,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,r4
	add	a,r0
	mov	r0,a
	clr	a
	addc	a,r1
	mov	r1,a
	mov	a,r2
	add	a,r0
	mov	r0,a
	clr	a
	addc	a,r1
	mov	r1,a
	mov	a,r2
	add	a,r3
	add	a,#_RoundKey
	mov	dpl,a
	clr	a
	addc	a,#(_RoundKey >> 8)
	mov	dph,a
	movx	a,@dptr
	mov	_AddRoundKey_sloc1_1_0,a
	mov	dpl,r0
	mov	dph,r1
	mov	b,r7
	lcall	__gptrget
	mov	r5,a
	mov	a,_AddRoundKey_sloc1_1_0
	xrl	ar5,a
	mov	dpl,r0
	mov	dph,r1
	mov	b,r7
	mov	a,r5
	lcall	__gptrput
;	src/aes.c:230: for(j = 0; j < 4; ++j)
	inc	r2
	cjne	r2,#0x04,00120$
00120$:
	pop	ar7
	jc	00103$
;	src/aes.c:228: for(i=0;i<4;++i)
	mov	a,_AddRoundKey_sloc0_1_0
	inc	a
	mov	r6,a
	cjne	r6,#0x04,00122$
00122$:
	jc	00109$
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'SubBytes'
;------------------------------------------------------------
;i                         Allocated with name '_SubBytes_i_1_43'
;j                         Allocated with name '_SubBytes_j_1_43'
;------------------------------------------------------------
;	src/aes.c:239: static void SubBytes(void)
;	-----------------------------------------
;	 function SubBytes
;	-----------------------------------------
_SubBytes:
;	src/aes.c:242: for(i = 0; i < 4; ++i)
	mov	r7,#0x00
;	src/aes.c:244: for(j = 0; j < 4; ++j)
00109$:
	mov	ar6,r7
	mov	r5,#0x00
00103$:
;	src/aes.c:246: (*state)[j][i] = getSBoxValue((*state)[j][i]);
	mov	dptr,#_state
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	mov	a,r5
	add	a,r5
	add	a,acc
	add	a,r2
	mov	r2,a
	clr	a
	addc	a,r3
	mov	r3,a
	mov	a,r6
	add	a,r2
	mov	r2,a
	clr	a
	addc	a,r3
	mov	r3,a
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
	mov	dpl,a
	push	ar6
	push	ar5
	push	ar4
	push	ar3
	push	ar2
	lcall	_getSBoxValue
	mov	r1,dpl
	pop	ar2
	pop	ar3
	pop	ar4
	pop	ar5
	pop	ar6
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r1
	lcall	__gptrput
;	src/aes.c:244: for(j = 0; j < 4; ++j)
	inc	r5
	cjne	r5,#0x04,00120$
00120$:
	jc	00103$
;	src/aes.c:242: for(i = 0; i < 4; ++i)
	mov	a,r6
	inc	a
	mov	r7,a
	cjne	r7,#0x04,00122$
00122$:
	jc	00109$
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'ShiftRows'
;------------------------------------------------------------
;temp                      Allocated with name '_ShiftRows_temp_1_47'
;------------------------------------------------------------
;	src/aes.c:254: static void ShiftRows(void)
;	-----------------------------------------
;	 function ShiftRows
;	-----------------------------------------
_ShiftRows:
;	src/aes.c:259: temp           = (*state)[0][1];
	mov	dptr,#_state
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x01
	add	a,r5
	mov	r2,a
	clr	a
	addc	a,r6
	mov	r3,a
	mov	ar4,r7
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
	mov	dptr,#_ShiftRows_temp_1_47
	movx	@dptr,a
;	src/aes.c:260: (*state)[0][1] = (*state)[1][1];
	mov	a,#0x05
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrput
;	src/aes.c:261: (*state)[1][1] = (*state)[2][1];
	mov	dptr,#_state
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x05
	add	a,r5
	mov	r2,a
	clr	a
	addc	a,r6
	mov	r3,a
	mov	ar4,r7
	mov	a,#0x09
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrput
;	src/aes.c:262: (*state)[2][1] = (*state)[3][1];
	mov	dptr,#_state
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x09
	add	a,r5
	mov	r2,a
	clr	a
	addc	a,r6
	mov	r3,a
	mov	ar4,r7
	mov	a,#0x0d
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrput
;	src/aes.c:263: (*state)[3][1] = temp;
	mov	dptr,#_state
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x0d
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dptr,#_ShiftRows_temp_1_47
	movx	a,@dptr
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrput
;	src/aes.c:266: temp           = (*state)[0][2];
	mov	dptr,#_state
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x02
	add	a,r5
	mov	r2,a
	clr	a
	addc	a,r6
	mov	r3,a
	mov	ar4,r7
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
	mov	dptr,#_ShiftRows_temp_1_47
	movx	@dptr,a
;	src/aes.c:267: (*state)[0][2] = (*state)[2][2];
	mov	a,#0x0a
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrput
;	src/aes.c:268: (*state)[2][2] = temp;
	mov	dptr,#_state
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x0a
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dptr,#_ShiftRows_temp_1_47
	movx	a,@dptr
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrput
;	src/aes.c:270: temp       = (*state)[1][2];
	mov	dptr,#_state
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x06
	add	a,r5
	mov	r2,a
	clr	a
	addc	a,r6
	mov	r3,a
	mov	ar4,r7
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
	mov	dptr,#_ShiftRows_temp_1_47
	movx	@dptr,a
;	src/aes.c:271: (*state)[1][2] = (*state)[3][2];
	mov	a,#0x0e
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrput
;	src/aes.c:272: (*state)[3][2] = temp;
	mov	dptr,#_state
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x0e
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dptr,#_ShiftRows_temp_1_47
	movx	a,@dptr
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrput
;	src/aes.c:275: temp       = (*state)[0][3];
	mov	dptr,#_state
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x03
	add	a,r5
	mov	r2,a
	clr	a
	addc	a,r6
	mov	r3,a
	mov	ar4,r7
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
	mov	dptr,#_ShiftRows_temp_1_47
	movx	@dptr,a
;	src/aes.c:276: (*state)[0][3] = (*state)[3][3];
	mov	a,#0x0f
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrput
;	src/aes.c:277: (*state)[3][3] = (*state)[2][3];
	mov	dptr,#_state
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x0f
	add	a,r5
	mov	r2,a
	clr	a
	addc	a,r6
	mov	r3,a
	mov	ar4,r7
	mov	a,#0x0b
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrput
;	src/aes.c:278: (*state)[2][3] = (*state)[1][3];
	mov	dptr,#_state
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x0b
	add	a,r5
	mov	r2,a
	clr	a
	addc	a,r6
	mov	r3,a
	mov	ar4,r7
	mov	a,#0x07
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrput
;	src/aes.c:279: (*state)[1][3] = temp;
	mov	dptr,#_state
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x07
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dptr,#_ShiftRows_temp_1_47
	movx	a,@dptr
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	ljmp	__gptrput
;------------------------------------------------------------
;Allocation info for local variables in function 'xtime'
;------------------------------------------------------------
;x                         Allocated with name '_xtime_x_1_48'
;------------------------------------------------------------
;	src/aes.c:282: static uint8_t xtime(uint8_t x)
;	-----------------------------------------
;	 function xtime
;	-----------------------------------------
_xtime:
	mov	a,dpl
	mov	dptr,#_xtime_x_1_48
	movx	@dptr,a
;	src/aes.c:284: return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
	movx	a,@dptr
	mov	r7,a
	add	a,acc
	mov	r6,a
	mov	a,r7
	rl	a
	anl	a,#0x01
	mov	b,#0x1b
	mul	ab
	mov	r7,a
	mov	a,r6
	xrl	ar7,a
	mov	dpl,r7
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'MixColumns'
;------------------------------------------------------------
;sloc0                     Allocated with name '_MixColumns_sloc0_1_0'
;sloc1                     Allocated with name '_MixColumns_sloc1_1_0'
;sloc2                     Allocated with name '_MixColumns_sloc2_1_0'
;i                         Allocated with name '_MixColumns_i_1_51'
;Tmp                       Allocated with name '_MixColumns_Tmp_1_51'
;Tm                        Allocated with name '_MixColumns_Tm_1_51'
;t                         Allocated with name '_MixColumns_t_1_51'
;------------------------------------------------------------
;	src/aes.c:288: static void MixColumns(void)
;	-----------------------------------------
;	 function MixColumns
;	-----------------------------------------
_MixColumns:
;	src/aes.c:292: for(i = 0; i < 4; ++i)
	mov	r7,#0x00
00102$:
;	src/aes.c:294: t   = (*state)[i][0];
	mov	dptr,#_state
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	mov	a,r7
	add	a,r7
	add	a,acc
	mov	r3,a
	add	a,r4
	mov	r4,a
	clr	a
	addc	a,r5
	mov	r5,a
	mov	dpl,r4
	mov	dph,r5
	mov	b,r6
	lcall	__gptrget
	mov	_MixColumns_sloc0_1_0,a
	mov	dptr,#_MixColumns_t_1_51
	movx	@dptr,a
;	src/aes.c:295: Tmp = (*state)[i][0] ^ (*state)[i][1] ^ (*state)[i][2] ^ (*state)[i][3] ;
	mov	a,#0x01
	add	a,r4
	mov	r0,a
	clr	a
	addc	a,r5
	mov	r1,a
	mov	ar2,r6
	mov	dpl,r0
	mov	dph,r1
	mov	b,r2
	lcall	__gptrget
	xrl	_MixColumns_sloc0_1_0,a
	mov	a,#0x02
	add	a,r4
	mov	r0,a
	clr	a
	addc	a,r5
	mov	r1,a
	mov	ar2,r6
	mov	dpl,r0
	mov	dph,r1
	mov	b,r2
	lcall	__gptrget
	mov	r0,a
	mov	a,_MixColumns_sloc0_1_0
	xrl	ar0,a
	mov	a,#0x03
	add	a,r4
	mov	r4,a
	clr	a
	addc	a,r5
	mov	r5,a
	mov	dpl,r4
	mov	dph,r5
	mov	b,r6
	lcall	__gptrget
	mov	dptr,#_MixColumns_Tmp_1_51
	xrl	a,r0
	movx	@dptr,a
;	src/aes.c:296: Tm  = (*state)[i][0] ^ (*state)[i][1] ; Tm = xtime(Tm);  (*state)[i][0] ^= Tm ^ Tmp ;
	mov	dpl,_MixColumns_sloc0_1_0
	push	ar7
	push	ar3
	lcall	_xtime
	mov	r6,dpl
	pop	ar3
	mov	dptr,#_state
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	a,r3
	add	a,r2
	mov	r2,a
	clr	a
	addc	a,r4
	mov	r4,a
	mov	dptr,#_MixColumns_Tmp_1_51
	movx	a,@dptr
	mov	r1,a
	xrl	ar6,a
	mov	dpl,r2
	mov	dph,r4
	mov	b,r5
	lcall	__gptrget
	xrl	ar6,a
	mov	dpl,r2
	mov	dph,r4
	mov	b,r5
	mov	a,r6
	lcall	__gptrput
;	src/aes.c:297: Tm  = (*state)[i][1] ^ (*state)[i][2] ; Tm = xtime(Tm);  (*state)[i][1] ^= Tm ^ Tmp ;
	mov	dptr,#_state
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	mov	a,r3
	add	a,r4
	mov	_MixColumns_sloc1_1_0,a
	clr	a
	addc	a,r5
	mov	(_MixColumns_sloc1_1_0 + 1),a
	mov	(_MixColumns_sloc1_1_0 + 2),r6
	mov	a,#0x01
	add	a,_MixColumns_sloc1_1_0
	mov	_MixColumns_sloc2_1_0,a
	clr	a
	addc	a,(_MixColumns_sloc1_1_0 + 1)
	mov	(_MixColumns_sloc2_1_0 + 1),a
	mov	(_MixColumns_sloc2_1_0 + 2),(_MixColumns_sloc1_1_0 + 2)
	mov	a,#0x02
	add	a,_MixColumns_sloc1_1_0
	mov	r4,a
	clr	a
	addc	a,(_MixColumns_sloc1_1_0 + 1)
	mov	r5,a
	mov	r6,(_MixColumns_sloc1_1_0 + 2)
	mov	dpl,r4
	mov	dph,r5
	mov	b,r6
	lcall	__gptrget
	mov	r4,a
	mov	dpl,_MixColumns_sloc2_1_0
	mov	dph,(_MixColumns_sloc2_1_0 + 1)
	mov	b,(_MixColumns_sloc2_1_0 + 2)
	lcall	__gptrget
	xrl	ar4,a
	mov	dpl,r4
	push	ar3
	push	ar1
	lcall	_xtime
	mov	r6,dpl
	pop	ar1
	pop	ar3
	pop	ar7
	mov	dptr,#_state
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	a,r3
	add	a,r2
	mov	r2,a
	clr	a
	addc	a,r4
	mov	r4,a
	inc	r2
	cjne	r2,#0x00,00110$
	inc	r4
00110$:
	mov	a,r1
	xrl	ar6,a
	mov	dpl,r2
	mov	dph,r4
	mov	b,r5
	lcall	__gptrget
	xrl	ar6,a
	mov	dpl,r2
	mov	dph,r4
	mov	b,r5
	mov	a,r6
	lcall	__gptrput
;	src/aes.c:298: Tm  = (*state)[i][2] ^ (*state)[i][3] ; Tm = xtime(Tm);  (*state)[i][2] ^= Tm ^ Tmp ;
	mov	dptr,#_state
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	mov	a,r3
	add	a,r4
	mov	_MixColumns_sloc2_1_0,a
	clr	a
	addc	a,r5
	mov	(_MixColumns_sloc2_1_0 + 1),a
	mov	(_MixColumns_sloc2_1_0 + 2),r6
	mov	a,#0x02
	add	a,_MixColumns_sloc2_1_0
	mov	_MixColumns_sloc1_1_0,a
	clr	a
	addc	a,(_MixColumns_sloc2_1_0 + 1)
	mov	(_MixColumns_sloc1_1_0 + 1),a
	mov	(_MixColumns_sloc1_1_0 + 2),(_MixColumns_sloc2_1_0 + 2)
	mov	a,#0x03
	add	a,_MixColumns_sloc2_1_0
	mov	r4,a
	clr	a
	addc	a,(_MixColumns_sloc2_1_0 + 1)
	mov	r5,a
	mov	r6,(_MixColumns_sloc2_1_0 + 2)
	mov	dpl,r4
	mov	dph,r5
	mov	b,r6
	lcall	__gptrget
	mov	r4,a
	mov	dpl,_MixColumns_sloc1_1_0
	mov	dph,(_MixColumns_sloc1_1_0 + 1)
	mov	b,(_MixColumns_sloc1_1_0 + 2)
	lcall	__gptrget
	xrl	ar4,a
	mov	dpl,r4
	push	ar7
	push	ar3
	push	ar1
	lcall	_xtime
	mov	r6,dpl
	pop	ar1
	pop	ar3
	mov	dptr,#_state
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	a,r3
	add	a,r2
	mov	r2,a
	clr	a
	addc	a,r4
	mov	r4,a
	mov	a,#0x02
	add	a,r2
	mov	r2,a
	clr	a
	addc	a,r4
	mov	r4,a
	mov	a,r1
	xrl	ar6,a
	mov	dpl,r2
	mov	dph,r4
	mov	b,r5
	lcall	__gptrget
	xrl	ar6,a
	mov	dpl,r2
	mov	dph,r4
	mov	b,r5
	mov	a,r6
	lcall	__gptrput
;	src/aes.c:299: Tm  = (*state)[i][3] ^ t ;        Tm = xtime(Tm);  (*state)[i][3] ^= Tm ^ Tmp ;
	mov	dptr,#_state
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	mov	a,r3
	add	a,r4
	mov	r4,a
	clr	a
	addc	a,r5
	mov	r5,a
	mov	a,#0x03
	add	a,r4
	mov	r4,a
	clr	a
	addc	a,r5
	mov	r5,a
	mov	dptr,#_MixColumns_t_1_51
	movx	a,@dptr
	mov	r2,a
	mov	dpl,r4
	mov	dph,r5
	mov	b,r6
	lcall	__gptrget
	xrl	ar2,a
	mov	dpl,r2
	push	ar3
	push	ar1
	lcall	_xtime
	mov	r6,dpl
	pop	ar1
	pop	ar3
	pop	ar7
	mov	dptr,#_state
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	a,r3
	add	a,r2
	mov	r2,a
	clr	a
	addc	a,r4
	mov	r4,a
	mov	a,#0x03
	add	a,r2
	mov	r2,a
	clr	a
	addc	a,r4
	mov	r4,a
	mov	a,r6
	xrl	ar1,a
	mov	dpl,r2
	mov	dph,r4
	mov	b,r5
	lcall	__gptrget
	mov	r6,a
	xrl	ar1,a
	mov	dpl,r2
	mov	dph,r4
	mov	b,r5
	mov	a,r1
	lcall	__gptrput
;	src/aes.c:292: for(i = 0; i < 4; ++i)
	inc	r7
	cjne	r7,#0x04,00111$
00111$:
	jnc	00112$
	ljmp	00102$
00112$:
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'InvMixColumns'
;------------------------------------------------------------
;sloc0                     Allocated with name '_InvMixColumns_sloc0_1_0'
;sloc1                     Allocated with name '_InvMixColumns_sloc1_1_0'
;sloc2                     Allocated with name '_InvMixColumns_sloc2_1_0'
;i                         Allocated with name '_InvMixColumns_i_1_54'
;a                         Allocated with name '_InvMixColumns_a_1_54'
;b                         Allocated with name '_InvMixColumns_b_1_54'
;c                         Allocated with name '_InvMixColumns_c_1_54'
;d                         Allocated with name '_InvMixColumns_d_1_54'
;------------------------------------------------------------
;	src/aes.c:328: static void InvMixColumns(void)
;	-----------------------------------------
;	 function InvMixColumns
;	-----------------------------------------
_InvMixColumns:
;	src/aes.c:332: for(i=0;i<4;++i)
	clr	a
	mov	_InvMixColumns_sloc1_1_0,a
	mov	(_InvMixColumns_sloc1_1_0 + 1),a
00102$:
;	src/aes.c:334: a = (*state)[i][0];
	mov	dptr,#_state
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	r1,_InvMixColumns_sloc1_1_0
	mov	a,(_InvMixColumns_sloc1_1_0 + 1)
	xch	a,r1
	add	a,acc
	xch	a,r1
	rlc	a
	xch	a,r1
	add	a,acc
	xch	a,r1
	rlc	a
	mov	r2,a
	mov	a,r1
	add	a,r3
	mov	r3,a
	mov	a,r2
	addc	a,r4
	mov	r4,a
	mov	dpl,r3
	mov	dph,r4
	mov	b,r5
	lcall	__gptrget
	mov	_InvMixColumns_sloc0_1_0,a
	mov	dptr,#_InvMixColumns_a_1_54
	movx	@dptr,a
;	src/aes.c:335: b = (*state)[i][1];
	mov	a,#0x01
	add	a,r3
	mov	r0,a
	clr	a
	addc	a,r4
	mov	r6,a
	mov	ar7,r5
	mov	dpl,r0
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	dptr,#_InvMixColumns_b_1_54
	movx	@dptr,a
;	src/aes.c:336: c = (*state)[i][2];
	mov	a,#0x02
	add	a,r3
	mov	r0,a
	clr	a
	addc	a,r4
	mov	r6,a
	mov	ar7,r5
	mov	dpl,r0
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	dptr,#_InvMixColumns_c_1_54
	movx	@dptr,a
;	src/aes.c:337: d = (*state)[i][3];
	mov	a,#0x03
	add	a,r3
	mov	r0,a
	clr	a
	addc	a,r4
	mov	r6,a
	mov	ar7,r5
	mov	dpl,r0
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	dptr,#_InvMixColumns_d_1_54
	movx	@dptr,a
;	src/aes.c:339: (*state)[i][0] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
	mov	_InvMixColumns_sloc2_1_0,r3
	mov	(_InvMixColumns_sloc2_1_0 + 1),r4
	mov	(_InvMixColumns_sloc2_1_0 + 2),r5
	mov	dptr,#_enc_dec_accel_galois_multiply_PARM_2
	mov	a,#0x0e
	movx	@dptr,a
	mov	dpl,_InvMixColumns_sloc0_1_0
	push	ar2
	push	ar1
	lcall	_enc_dec_accel_galois_multiply
	mov	r7,dpl
	pop	ar1
	pop	ar2
	mov	dptr,#_InvMixColumns_b_1_54
	movx	a,@dptr
	mov	r6,a
	mov	dptr,#_enc_dec_accel_galois_multiply_PARM_2
	mov	a,#0x0b
	movx	@dptr,a
	mov	dpl,r6
	push	ar7
	push	ar6
	push	ar2
	push	ar1
	lcall	_enc_dec_accel_galois_multiply
	mov	r0,dpl
	pop	ar1
	pop	ar2
	pop	ar6
	pop	ar7
	mov	a,r0
	xrl	a,r7
	mov	_InvMixColumns_sloc0_1_0,a
	mov	dptr,#_InvMixColumns_c_1_54
	movx	a,@dptr
	mov	r0,a
	mov	dptr,#_enc_dec_accel_galois_multiply_PARM_2
	mov	a,#0x0d
	movx	@dptr,a
	mov	dpl,r0
	push	ar6
	push	ar2
	push	ar1
	push	ar0
	lcall	_enc_dec_accel_galois_multiply
	mov	r7,dpl
	pop	ar0
	pop	ar1
	pop	ar2
	pop	ar6
	mov	a,r7
	xrl	_InvMixColumns_sloc0_1_0,a
	mov	dptr,#_InvMixColumns_d_1_54
	movx	a,@dptr
	mov	r7,a
	mov	dptr,#_enc_dec_accel_galois_multiply_PARM_2
	mov	a,#0x09
	movx	@dptr,a
	mov	dpl,r7
	push	ar7
	push	ar6
	push	ar2
	push	ar1
	push	ar0
	lcall	_enc_dec_accel_galois_multiply
	mov	r5,dpl
	pop	ar0
	pop	ar1
	pop	ar2
	mov	a,_InvMixColumns_sloc0_1_0
	xrl	ar5,a
	mov	dpl,_InvMixColumns_sloc2_1_0
	mov	dph,(_InvMixColumns_sloc2_1_0 + 1)
	mov	b,(_InvMixColumns_sloc2_1_0 + 2)
	mov	a,r5
	lcall	__gptrput
;	src/aes.c:340: (*state)[i][1] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
	mov	dptr,#_state
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	a,r1
	add	a,r3
	mov	r3,a
	mov	a,r2
	addc	a,r4
	mov	r4,a
	mov	a,#0x01
	add	a,r3
	mov	_InvMixColumns_sloc2_1_0,a
	clr	a
	addc	a,r4
	mov	(_InvMixColumns_sloc2_1_0 + 1),a
	mov	(_InvMixColumns_sloc2_1_0 + 2),r5
	mov	dptr,#_InvMixColumns_a_1_54
	movx	a,@dptr
	mov	_InvMixColumns_sloc0_1_0,a
	mov	dptr,#_enc_dec_accel_galois_multiply_PARM_2
	mov	a,#0x09
	movx	@dptr,a
	mov	dpl,_InvMixColumns_sloc0_1_0
	push	ar2
	push	ar1
	push	ar0
	lcall	_enc_dec_accel_galois_multiply
	mov	r4,dpl
	pop	ar0
	pop	ar1
	pop	ar2
	pop	ar6
	mov	dptr,#_enc_dec_accel_galois_multiply_PARM_2
	mov	a,#0x0e
	movx	@dptr,a
	mov	dpl,r6
	push	ar6
	push	ar4
	push	ar2
	push	ar1
	push	ar0
	lcall	_enc_dec_accel_galois_multiply
	mov	r3,dpl
	pop	ar0
	pop	ar1
	pop	ar2
	pop	ar4
	mov	a,r3
	xrl	ar4,a
	mov	dptr,#_enc_dec_accel_galois_multiply_PARM_2
	mov	a,#0x0b
	movx	@dptr,a
	mov	dpl,r0
	push	ar4
	push	ar2
	push	ar1
	push	ar0
	lcall	_enc_dec_accel_galois_multiply
	mov	r3,dpl
	pop	ar0
	pop	ar1
	pop	ar2
	pop	ar4
	pop	ar6
	pop	ar7
	mov	a,r3
	xrl	ar4,a
	mov	dptr,#_enc_dec_accel_galois_multiply_PARM_2
	mov	a,#0x0d
	movx	@dptr,a
	mov	dpl,r7
	push	ar7
	push	ar6
	push	ar4
	push	ar2
	push	ar1
	push	ar0
	lcall	_enc_dec_accel_galois_multiply
	mov	r3,dpl
	pop	ar0
	pop	ar1
	pop	ar2
	pop	ar4
	mov	a,r3
	xrl	ar4,a
	mov	dpl,_InvMixColumns_sloc2_1_0
	mov	dph,(_InvMixColumns_sloc2_1_0 + 1)
	mov	b,(_InvMixColumns_sloc2_1_0 + 2)
	mov	a,r4
	lcall	__gptrput
;	src/aes.c:341: (*state)[i][2] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
	mov	dptr,#_state
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	a,r1
	add	a,r3
	mov	r3,a
	mov	a,r2
	addc	a,r4
	mov	r4,a
	mov	a,#0x02
	add	a,r3
	mov	_InvMixColumns_sloc2_1_0,a
	clr	a
	addc	a,r4
	mov	(_InvMixColumns_sloc2_1_0 + 1),a
	mov	(_InvMixColumns_sloc2_1_0 + 2),r5
	mov	dptr,#_enc_dec_accel_galois_multiply_PARM_2
	mov	a,#0x0d
	movx	@dptr,a
	mov	dpl,_InvMixColumns_sloc0_1_0
	push	ar2
	push	ar1
	push	ar0
	lcall	_enc_dec_accel_galois_multiply
	mov	r5,dpl
	pop	ar0
	pop	ar1
	pop	ar2
	pop	ar6
	mov	dptr,#_enc_dec_accel_galois_multiply_PARM_2
	mov	a,#0x09
	movx	@dptr,a
	mov	dpl,r6
	push	ar6
	push	ar5
	push	ar2
	push	ar1
	push	ar0
	lcall	_enc_dec_accel_galois_multiply
	mov	r4,dpl
	pop	ar0
	pop	ar1
	pop	ar2
	pop	ar5
	mov	a,r4
	xrl	ar5,a
	mov	dptr,#_enc_dec_accel_galois_multiply_PARM_2
	mov	a,#0x0e
	movx	@dptr,a
	mov	dpl,r0
	push	ar5
	push	ar2
	push	ar1
	push	ar0
	lcall	_enc_dec_accel_galois_multiply
	mov	r4,dpl
	pop	ar0
	pop	ar1
	pop	ar2
	pop	ar5
	pop	ar6
	pop	ar7
	mov	a,r4
	xrl	ar5,a
	mov	dptr,#_enc_dec_accel_galois_multiply_PARM_2
	mov	a,#0x0b
	movx	@dptr,a
	mov	dpl,r7
	push	ar7
	push	ar6
	push	ar5
	push	ar2
	push	ar1
	push	ar0
	lcall	_enc_dec_accel_galois_multiply
	mov	r4,dpl
	pop	ar0
	pop	ar1
	pop	ar2
	pop	ar5
	mov	a,r4
	xrl	ar5,a
	mov	dpl,_InvMixColumns_sloc2_1_0
	mov	dph,(_InvMixColumns_sloc2_1_0 + 1)
	mov	b,(_InvMixColumns_sloc2_1_0 + 2)
	mov	a,r5
	lcall	__gptrput
;	src/aes.c:342: (*state)[i][3] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
	mov	dptr,#_state
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	a,r1
	add	a,r3
	mov	r3,a
	mov	a,r2
	addc	a,r4
	mov	r4,a
	mov	a,#0x03
	add	a,r3
	mov	r3,a
	clr	a
	addc	a,r4
	mov	r4,a
	mov	dptr,#_enc_dec_accel_galois_multiply_PARM_2
	mov	a,#0x0b
	movx	@dptr,a
	mov	dpl,_InvMixColumns_sloc0_1_0
	push	ar5
	push	ar4
	push	ar3
	push	ar0
	lcall	_enc_dec_accel_galois_multiply
	mov	r2,dpl
	pop	ar0
	pop	ar3
	pop	ar4
	pop	ar5
	pop	ar6
	mov	dptr,#_enc_dec_accel_galois_multiply_PARM_2
	mov	a,#0x0d
	movx	@dptr,a
	mov	dpl,r6
	push	ar5
	push	ar4
	push	ar3
	push	ar2
	push	ar0
	lcall	_enc_dec_accel_galois_multiply
	mov	r6,dpl
	pop	ar0
	pop	ar2
	mov	a,r6
	xrl	ar2,a
	mov	dptr,#_enc_dec_accel_galois_multiply_PARM_2
	mov	a,#0x09
	movx	@dptr,a
	mov	dpl,r0
	push	ar2
	lcall	_enc_dec_accel_galois_multiply
	mov	r6,dpl
	pop	ar2
	pop	ar3
	pop	ar4
	pop	ar5
	pop	ar7
	mov	a,r6
	xrl	ar2,a
	mov	dptr,#_enc_dec_accel_galois_multiply_PARM_2
	mov	a,#0x0e
	movx	@dptr,a
	mov	dpl,r7
	push	ar5
	push	ar4
	push	ar3
	push	ar2
	lcall	_enc_dec_accel_galois_multiply
	mov	r7,dpl
	pop	ar2
	pop	ar3
	pop	ar4
	pop	ar5
	mov	a,r7
	xrl	ar2,a
	mov	dpl,r3
	mov	dph,r4
	mov	b,r5
	mov	a,r2
	lcall	__gptrput
;	src/aes.c:332: for(i=0;i<4;++i)
	inc	_InvMixColumns_sloc1_1_0
	clr	a
	cjne	a,_InvMixColumns_sloc1_1_0,00110$
	inc	(_InvMixColumns_sloc1_1_0 + 1)
00110$:
	clr	c
	mov	a,_InvMixColumns_sloc1_1_0
	subb	a,#0x04
	mov	a,(_InvMixColumns_sloc1_1_0 + 1)
	xrl	a,#0x80
	subb	a,#0x80
	jnc	00111$
	ljmp	00102$
00111$:
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'InvSubBytes'
;------------------------------------------------------------
;i                         Allocated with name '_InvSubBytes_i_1_57'
;j                         Allocated with name '_InvSubBytes_j_1_57'
;------------------------------------------------------------
;	src/aes.c:349: static void InvSubBytes(void)
;	-----------------------------------------
;	 function InvSubBytes
;	-----------------------------------------
_InvSubBytes:
;	src/aes.c:352: for(i=0;i<4;++i)
	mov	r7,#0x00
;	src/aes.c:354: for(j=0;j<4;++j)
00109$:
	mov	ar6,r7
	mov	r5,#0x00
00103$:
;	src/aes.c:356: (*state)[j][i] = getSBoxInvert((*state)[j][i]);
	mov	dptr,#_state
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	mov	a,r5
	add	a,r5
	add	a,acc
	add	a,r2
	mov	r2,a
	clr	a
	addc	a,r3
	mov	r3,a
	mov	a,r6
	add	a,r2
	mov	r2,a
	clr	a
	addc	a,r3
	mov	r3,a
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
	mov	dpl,a
	push	ar6
	push	ar5
	push	ar4
	push	ar3
	push	ar2
	lcall	_getSBoxInvert
	mov	r1,dpl
	pop	ar2
	pop	ar3
	pop	ar4
	pop	ar5
	pop	ar6
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r1
	lcall	__gptrput
;	src/aes.c:354: for(j=0;j<4;++j)
	inc	r5
	cjne	r5,#0x04,00120$
00120$:
	jc	00103$
;	src/aes.c:352: for(i=0;i<4;++i)
	mov	a,r6
	inc	a
	mov	r7,a
	cjne	r7,#0x04,00122$
00122$:
	jc	00109$
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'InvShiftRows'
;------------------------------------------------------------
;temp                      Allocated with name '_InvShiftRows_temp_1_61'
;------------------------------------------------------------
;	src/aes.c:361: static void InvShiftRows(void)
;	-----------------------------------------
;	 function InvShiftRows
;	-----------------------------------------
_InvShiftRows:
;	src/aes.c:366: temp=(*state)[3][1];
	mov	dptr,#_state
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x0d
	add	a,r5
	mov	r2,a
	clr	a
	addc	a,r6
	mov	r3,a
	mov	ar4,r7
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
	mov	r1,a
	mov	dptr,#_InvShiftRows_temp_1_61
	movx	@dptr,a
;	src/aes.c:367: (*state)[3][1]=(*state)[2][1];
	mov	a,#0x09
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrput
;	src/aes.c:368: (*state)[2][1]=(*state)[1][1];
	mov	dptr,#_state
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x09
	add	a,r5
	mov	r2,a
	clr	a
	addc	a,r6
	mov	r3,a
	mov	ar4,r7
	mov	a,#0x05
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrput
;	src/aes.c:369: (*state)[1][1]=(*state)[0][1];
	mov	dptr,#_state
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x05
	add	a,r5
	mov	r2,a
	clr	a
	addc	a,r6
	mov	r3,a
	mov	ar4,r7
	inc	r5
	cjne	r5,#0x00,00103$
	inc	r6
00103$:
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrput
;	src/aes.c:370: (*state)[0][1]=temp;
	mov	dptr,#_state
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	inc	r5
	cjne	r5,#0x00,00104$
	inc	r6
00104$:
	mov	dptr,#_InvShiftRows_temp_1_61
	movx	a,@dptr
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrput
;	src/aes.c:373: temp=(*state)[0][2];
	mov	dptr,#_state
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x02
	add	a,r5
	mov	r2,a
	clr	a
	addc	a,r6
	mov	r3,a
	mov	ar4,r7
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
	mov	dptr,#_InvShiftRows_temp_1_61
	movx	@dptr,a
;	src/aes.c:374: (*state)[0][2]=(*state)[2][2];
	mov	a,#0x0a
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrput
;	src/aes.c:375: (*state)[2][2]=temp;
	mov	dptr,#_state
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x0a
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dptr,#_InvShiftRows_temp_1_61
	movx	a,@dptr
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrput
;	src/aes.c:377: temp=(*state)[1][2];
	mov	dptr,#_state
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x06
	add	a,r5
	mov	r2,a
	clr	a
	addc	a,r6
	mov	r3,a
	mov	ar4,r7
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
	mov	dptr,#_InvShiftRows_temp_1_61
	movx	@dptr,a
;	src/aes.c:378: (*state)[1][2]=(*state)[3][2];
	mov	a,#0x0e
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrput
;	src/aes.c:379: (*state)[3][2]=temp;
	mov	dptr,#_state
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x0e
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dptr,#_InvShiftRows_temp_1_61
	movx	a,@dptr
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrput
;	src/aes.c:382: temp=(*state)[0][3];
	mov	dptr,#_state
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x03
	add	a,r5
	mov	r2,a
	clr	a
	addc	a,r6
	mov	r3,a
	mov	ar4,r7
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
	mov	dptr,#_InvShiftRows_temp_1_61
	movx	@dptr,a
;	src/aes.c:383: (*state)[0][3]=(*state)[1][3];
	mov	a,#0x07
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrput
;	src/aes.c:384: (*state)[1][3]=(*state)[2][3];
	mov	dptr,#_state
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x07
	add	a,r5
	mov	r2,a
	clr	a
	addc	a,r6
	mov	r3,a
	mov	ar4,r7
	mov	a,#0x0b
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrput
;	src/aes.c:385: (*state)[2][3]=(*state)[3][3];
	mov	dptr,#_state
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x0b
	add	a,r5
	mov	r2,a
	clr	a
	addc	a,r6
	mov	r3,a
	mov	ar4,r7
	mov	a,#0x0f
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrput
;	src/aes.c:386: (*state)[3][3]=temp;
	mov	dptr,#_state
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x0f
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dptr,#_InvShiftRows_temp_1_61
	movx	a,@dptr
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	ljmp	__gptrput
;------------------------------------------------------------
;Allocation info for local variables in function 'Cipher'
;------------------------------------------------------------
;round                     Allocated with name '_Cipher_round_1_63'
;------------------------------------------------------------
;	src/aes.c:391: static void Cipher(void)
;	-----------------------------------------
;	 function Cipher
;	-----------------------------------------
_Cipher:
;	src/aes.c:396: AddRoundKey(0); 
	mov	dpl,#0x00
	lcall	_AddRoundKey
;	src/aes.c:401: for(round = 1; round < Nr; ++round)
	mov	r7,#0x01
00102$:
;	src/aes.c:403: SubBytes();
	push	ar7
	lcall	_SubBytes
;	src/aes.c:404: ShiftRows();
	lcall	_ShiftRows
;	src/aes.c:405: MixColumns();
	lcall	_MixColumns
	pop	ar7
;	src/aes.c:406: AddRoundKey(round);
	mov	dpl,r7
	push	ar7
	lcall	_AddRoundKey
	pop	ar7
;	src/aes.c:401: for(round = 1; round < Nr; ++round)
	inc	r7
	cjne	r7,#0x0a,00113$
00113$:
	jc	00102$
;	src/aes.c:411: SubBytes();
	lcall	_SubBytes
;	src/aes.c:412: ShiftRows();
	lcall	_ShiftRows
;	src/aes.c:413: AddRoundKey(Nr);
	mov	dpl,#0x0a
	ljmp	_AddRoundKey
;------------------------------------------------------------
;Allocation info for local variables in function 'InvCipher'
;------------------------------------------------------------
;round                     Allocated with name '_InvCipher_round_1_66'
;------------------------------------------------------------
;	src/aes.c:416: static void InvCipher(void)
;	-----------------------------------------
;	 function InvCipher
;	-----------------------------------------
_InvCipher:
;	src/aes.c:421: AddRoundKey(Nr); 
	mov	dpl,#0x0a
	lcall	_AddRoundKey
;	src/aes.c:426: for(round=Nr-1;round>0;round--)
	mov	r7,#0x09
00102$:
;	src/aes.c:428: InvShiftRows();
	push	ar7
	lcall	_InvShiftRows
;	src/aes.c:429: InvSubBytes();
	lcall	_InvSubBytes
	pop	ar7
;	src/aes.c:430: AddRoundKey(round);
	mov	dpl,r7
	push	ar7
	lcall	_AddRoundKey
;	src/aes.c:431: InvMixColumns();
	lcall	_InvMixColumns
	pop	ar7
;	src/aes.c:426: for(round=Nr-1;round>0;round--)
	djnz	r7,00102$
;	src/aes.c:436: InvShiftRows();
	lcall	_InvShiftRows
;	src/aes.c:437: InvSubBytes();
	lcall	_InvSubBytes
;	src/aes.c:438: AddRoundKey(0);
	mov	dpl,#0x00
	ljmp	_AddRoundKey
;------------------------------------------------------------
;Allocation info for local variables in function 'BlockCopy'
;------------------------------------------------------------
;input                     Allocated with name '_BlockCopy_PARM_2'
;output                    Allocated with name '_BlockCopy_output_1_68'
;i                         Allocated with name '_BlockCopy_i_1_69'
;sloc0                     Allocated with name '_BlockCopy_sloc0_1_0'
;------------------------------------------------------------
;	src/aes.c:441: static void BlockCopy(uint8_t* output, const uint8_t* input)
;	-----------------------------------------
;	 function BlockCopy
;	-----------------------------------------
_BlockCopy:
	mov	r7,b
	mov	r6,dph
	mov	a,dpl
	mov	dptr,#_BlockCopy_output_1_68
	movx	@dptr,a
	mov	a,r6
	inc	dptr
	movx	@dptr,a
	mov	a,r7
	inc	dptr
	movx	@dptr,a
;	src/aes.c:444: for (i=0;i<KEYLEN;++i)
	mov	dptr,#_BlockCopy_output_1_68
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	dptr,#_BlockCopy_PARM_2
	movx	a,@dptr
	mov	_BlockCopy_sloc0_1_0,a
	inc	dptr
	movx	a,@dptr
	mov	(_BlockCopy_sloc0_1_0 + 1),a
	inc	dptr
	movx	a,@dptr
	mov	(_BlockCopy_sloc0_1_0 + 2),a
	mov	r1,#0x00
00102$:
;	src/aes.c:446: output[i] = input[i];
	mov	a,r1
	add	a,r5
	mov	r0,a
	clr	a
	addc	a,r6
	mov	r3,a
	mov	ar4,r7
	push	ar5
	push	ar6
	push	ar7
	mov	a,r1
	add	a,_BlockCopy_sloc0_1_0
	mov	r2,a
	clr	a
	addc	a,(_BlockCopy_sloc0_1_0 + 1)
	mov	r6,a
	mov	r7,(_BlockCopy_sloc0_1_0 + 2)
	mov	dpl,r2
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	r2,a
	mov	dpl,r0
	mov	dph,r3
	mov	b,r4
	lcall	__gptrput
;	src/aes.c:444: for (i=0;i<KEYLEN;++i)
	inc	r1
	cjne	r1,#0x10,00110$
00110$:
	pop	ar7
	pop	ar6
	pop	ar5
	jc	00102$
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'AES128_ECB_encrypt'
;------------------------------------------------------------
;key                       Allocated with name '_AES128_ECB_encrypt_PARM_2'
;output                    Allocated with name '_AES128_ECB_encrypt_PARM_3'
;input                     Allocated with name '_AES128_ECB_encrypt_input_1_71'
;------------------------------------------------------------
;	src/aes.c:458: void AES128_ECB_encrypt(const uint8_t* input, const uint8_t* key, uint8_t* output)
;	-----------------------------------------
;	 function AES128_ECB_encrypt
;	-----------------------------------------
_AES128_ECB_encrypt:
	mov	r7,b
	mov	r6,dph
	mov	a,dpl
	mov	dptr,#_AES128_ECB_encrypt_input_1_71
	movx	@dptr,a
	mov	a,r6
	inc	dptr
	movx	@dptr,a
	mov	a,r7
	inc	dptr
	movx	@dptr,a
;	src/aes.c:461: BlockCopy(output, input);
	mov	dptr,#_AES128_ECB_encrypt_PARM_3
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	dptr,#_AES128_ECB_encrypt_input_1_71
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	mov	dptr,#_BlockCopy_PARM_2
	mov	a,r2
	movx	@dptr,a
	mov	a,r3
	inc	dptr
	movx	@dptr,a
	mov	a,r4
	inc	dptr
	movx	@dptr,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	_BlockCopy
;	src/aes.c:462: state = (state_t*)output;
	mov	dptr,#_AES128_ECB_encrypt_PARM_3
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	dptr,#_state
	mov	a,r5
	movx	@dptr,a
	mov	a,r6
	inc	dptr
	movx	@dptr,a
	mov	a,r7
	inc	dptr
	movx	@dptr,a
;	src/aes.c:464: Key = key;
	mov	dptr,#_AES128_ECB_encrypt_PARM_2
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	dptr,#_Key
	mov	a,r5
	movx	@dptr,a
	mov	a,r6
	inc	dptr
	movx	@dptr,a
	mov	a,r7
	inc	dptr
	movx	@dptr,a
;	src/aes.c:465: KeyExpansion();
	lcall	_KeyExpansion
;	src/aes.c:468: Cipher();
	ljmp	_Cipher
;------------------------------------------------------------
;Allocation info for local variables in function 'AES128_ECB_decrypt'
;------------------------------------------------------------
;key                       Allocated with name '_AES128_ECB_decrypt_PARM_2'
;output                    Allocated with name '_AES128_ECB_decrypt_PARM_3'
;input                     Allocated with name '_AES128_ECB_decrypt_input_1_73'
;------------------------------------------------------------
;	src/aes.c:471: void AES128_ECB_decrypt(const uint8_t* input, const uint8_t* key, uint8_t *output)
;	-----------------------------------------
;	 function AES128_ECB_decrypt
;	-----------------------------------------
_AES128_ECB_decrypt:
	mov	r7,b
	mov	r6,dph
	mov	a,dpl
	mov	dptr,#_AES128_ECB_decrypt_input_1_73
	movx	@dptr,a
	mov	a,r6
	inc	dptr
	movx	@dptr,a
	mov	a,r7
	inc	dptr
	movx	@dptr,a
;	src/aes.c:474: BlockCopy(output, input);
	mov	dptr,#_AES128_ECB_decrypt_PARM_3
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	dptr,#_AES128_ECB_decrypt_input_1_73
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	mov	dptr,#_BlockCopy_PARM_2
	mov	a,r2
	movx	@dptr,a
	mov	a,r3
	inc	dptr
	movx	@dptr,a
	mov	a,r4
	inc	dptr
	movx	@dptr,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	_BlockCopy
;	src/aes.c:475: state = (state_t*)output;
	mov	dptr,#_AES128_ECB_decrypt_PARM_3
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	dptr,#_state
	mov	a,r5
	movx	@dptr,a
	mov	a,r6
	inc	dptr
	movx	@dptr,a
	mov	a,r7
	inc	dptr
	movx	@dptr,a
;	src/aes.c:478: Key = key;
	mov	dptr,#_AES128_ECB_decrypt_PARM_2
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	dptr,#_Key
	mov	a,r5
	movx	@dptr,a
	mov	a,r6
	inc	dptr
	movx	@dptr,a
	mov	a,r7
	inc	dptr
	movx	@dptr,a
;	src/aes.c:479: KeyExpansion();
	lcall	_KeyExpansion
;	src/aes.c:481: InvCipher();
	ljmp	_InvCipher
	.area CSEG    (CODE)
	.area CONST   (CODE)
_sbox:
	.db #0x63	; 99	'c'
	.db #0x7c	; 124
	.db #0x77	; 119	'w'
	.db #0x7b	; 123
	.db #0xf2	; 242
	.db #0x6b	; 107	'k'
	.db #0x6f	; 111	'o'
	.db #0xc5	; 197
	.db #0x30	; 48	'0'
	.db #0x01	; 1
	.db #0x67	; 103	'g'
	.db #0x2b	; 43
	.db #0xfe	; 254
	.db #0xd7	; 215
	.db #0xab	; 171
	.db #0x76	; 118	'v'
	.db #0xca	; 202
	.db #0x82	; 130
	.db #0xc9	; 201
	.db #0x7d	; 125
	.db #0xfa	; 250
	.db #0x59	; 89	'Y'
	.db #0x47	; 71	'G'
	.db #0xf0	; 240
	.db #0xad	; 173
	.db #0xd4	; 212
	.db #0xa2	; 162
	.db #0xaf	; 175
	.db #0x9c	; 156
	.db #0xa4	; 164
	.db #0x72	; 114	'r'
	.db #0xc0	; 192
	.db #0xb7	; 183
	.db #0xfd	; 253
	.db #0x93	; 147
	.db #0x26	; 38
	.db #0x36	; 54	'6'
	.db #0x3f	; 63
	.db #0xf7	; 247
	.db #0xcc	; 204
	.db #0x34	; 52	'4'
	.db #0xa5	; 165
	.db #0xe5	; 229
	.db #0xf1	; 241
	.db #0x71	; 113	'q'
	.db #0xd8	; 216
	.db #0x31	; 49	'1'
	.db #0x15	; 21
	.db #0x04	; 4
	.db #0xc7	; 199
	.db #0x23	; 35
	.db #0xc3	; 195
	.db #0x18	; 24
	.db #0x96	; 150
	.db #0x05	; 5
	.db #0x9a	; 154
	.db #0x07	; 7
	.db #0x12	; 18
	.db #0x80	; 128
	.db #0xe2	; 226
	.db #0xeb	; 235
	.db #0x27	; 39
	.db #0xb2	; 178
	.db #0x75	; 117	'u'
	.db #0x09	; 9
	.db #0x83	; 131
	.db #0x2c	; 44
	.db #0x1a	; 26
	.db #0x1b	; 27
	.db #0x6e	; 110	'n'
	.db #0x5a	; 90	'Z'
	.db #0xa0	; 160
	.db #0x52	; 82	'R'
	.db #0x3b	; 59
	.db #0xd6	; 214
	.db #0xb3	; 179
	.db #0x29	; 41
	.db #0xe3	; 227
	.db #0x2f	; 47
	.db #0x84	; 132
	.db #0x53	; 83	'S'
	.db #0xd1	; 209
	.db #0x00	; 0
	.db #0xed	; 237
	.db #0x20	; 32
	.db #0xfc	; 252
	.db #0xb1	; 177
	.db #0x5b	; 91
	.db #0x6a	; 106	'j'
	.db #0xcb	; 203
	.db #0xbe	; 190
	.db #0x39	; 57	'9'
	.db #0x4a	; 74	'J'
	.db #0x4c	; 76	'L'
	.db #0x58	; 88	'X'
	.db #0xcf	; 207
	.db #0xd0	; 208
	.db #0xef	; 239
	.db #0xaa	; 170
	.db #0xfb	; 251
	.db #0x43	; 67	'C'
	.db #0x4d	; 77	'M'
	.db #0x33	; 51	'3'
	.db #0x85	; 133
	.db #0x45	; 69	'E'
	.db #0xf9	; 249
	.db #0x02	; 2
	.db #0x7f	; 127
	.db #0x50	; 80	'P'
	.db #0x3c	; 60
	.db #0x9f	; 159
	.db #0xa8	; 168
	.db #0x51	; 81	'Q'
	.db #0xa3	; 163
	.db #0x40	; 64
	.db #0x8f	; 143
	.db #0x92	; 146
	.db #0x9d	; 157
	.db #0x38	; 56	'8'
	.db #0xf5	; 245
	.db #0xbc	; 188
	.db #0xb6	; 182
	.db #0xda	; 218
	.db #0x21	; 33
	.db #0x10	; 16
	.db #0xff	; 255
	.db #0xf3	; 243
	.db #0xd2	; 210
	.db #0xcd	; 205
	.db #0x0c	; 12
	.db #0x13	; 19
	.db #0xec	; 236
	.db #0x5f	; 95
	.db #0x97	; 151
	.db #0x44	; 68	'D'
	.db #0x17	; 23
	.db #0xc4	; 196
	.db #0xa7	; 167
	.db #0x7e	; 126
	.db #0x3d	; 61
	.db #0x64	; 100	'd'
	.db #0x5d	; 93
	.db #0x19	; 25
	.db #0x73	; 115	's'
	.db #0x60	; 96
	.db #0x81	; 129
	.db #0x4f	; 79	'O'
	.db #0xdc	; 220
	.db #0x22	; 34
	.db #0x2a	; 42
	.db #0x90	; 144
	.db #0x88	; 136
	.db #0x46	; 70	'F'
	.db #0xee	; 238
	.db #0xb8	; 184
	.db #0x14	; 20
	.db #0xde	; 222
	.db #0x5e	; 94
	.db #0x0b	; 11
	.db #0xdb	; 219
	.db #0xe0	; 224
	.db #0x32	; 50	'2'
	.db #0x3a	; 58
	.db #0x0a	; 10
	.db #0x49	; 73	'I'
	.db #0x06	; 6
	.db #0x24	; 36
	.db #0x5c	; 92
	.db #0xc2	; 194
	.db #0xd3	; 211
	.db #0xac	; 172
	.db #0x62	; 98	'b'
	.db #0x91	; 145
	.db #0x95	; 149
	.db #0xe4	; 228
	.db #0x79	; 121	'y'
	.db #0xe7	; 231
	.db #0xc8	; 200
	.db #0x37	; 55	'7'
	.db #0x6d	; 109	'm'
	.db #0x8d	; 141
	.db #0xd5	; 213
	.db #0x4e	; 78	'N'
	.db #0xa9	; 169
	.db #0x6c	; 108	'l'
	.db #0x56	; 86	'V'
	.db #0xf4	; 244
	.db #0xea	; 234
	.db #0x65	; 101	'e'
	.db #0x7a	; 122	'z'
	.db #0xae	; 174
	.db #0x08	; 8
	.db #0xba	; 186
	.db #0x78	; 120	'x'
	.db #0x25	; 37
	.db #0x2e	; 46
	.db #0x1c	; 28
	.db #0xa6	; 166
	.db #0xb4	; 180
	.db #0xc6	; 198
	.db #0xe8	; 232
	.db #0xdd	; 221
	.db #0x74	; 116	't'
	.db #0x1f	; 31
	.db #0x4b	; 75	'K'
	.db #0xbd	; 189
	.db #0x8b	; 139
	.db #0x8a	; 138
	.db #0x70	; 112	'p'
	.db #0x3e	; 62
	.db #0xb5	; 181
	.db #0x66	; 102	'f'
	.db #0x48	; 72	'H'
	.db #0x03	; 3
	.db #0xf6	; 246
	.db #0x0e	; 14
	.db #0x61	; 97	'a'
	.db #0x35	; 53	'5'
	.db #0x57	; 87	'W'
	.db #0xb9	; 185
	.db #0x86	; 134
	.db #0xc1	; 193
	.db #0x1d	; 29
	.db #0x9e	; 158
	.db #0xe1	; 225
	.db #0xf8	; 248
	.db #0x98	; 152
	.db #0x11	; 17
	.db #0x69	; 105	'i'
	.db #0xd9	; 217
	.db #0x8e	; 142
	.db #0x94	; 148
	.db #0x9b	; 155
	.db #0x1e	; 30
	.db #0x87	; 135
	.db #0xe9	; 233
	.db #0xce	; 206
	.db #0x55	; 85	'U'
	.db #0x28	; 40
	.db #0xdf	; 223
	.db #0x8c	; 140
	.db #0xa1	; 161
	.db #0x89	; 137
	.db #0x0d	; 13
	.db #0xbf	; 191
	.db #0xe6	; 230
	.db #0x42	; 66	'B'
	.db #0x68	; 104	'h'
	.db #0x41	; 65	'A'
	.db #0x99	; 153
	.db #0x2d	; 45
	.db #0x0f	; 15
	.db #0xb0	; 176
	.db #0x54	; 84	'T'
	.db #0xbb	; 187
	.db #0x16	; 22
_rsbox:
	.db #0x52	; 82	'R'
	.db #0x09	; 9
	.db #0x6a	; 106	'j'
	.db #0xd5	; 213
	.db #0x30	; 48	'0'
	.db #0x36	; 54	'6'
	.db #0xa5	; 165
	.db #0x38	; 56	'8'
	.db #0xbf	; 191
	.db #0x40	; 64
	.db #0xa3	; 163
	.db #0x9e	; 158
	.db #0x81	; 129
	.db #0xf3	; 243
	.db #0xd7	; 215
	.db #0xfb	; 251
	.db #0x7c	; 124
	.db #0xe3	; 227
	.db #0x39	; 57	'9'
	.db #0x82	; 130
	.db #0x9b	; 155
	.db #0x2f	; 47
	.db #0xff	; 255
	.db #0x87	; 135
	.db #0x34	; 52	'4'
	.db #0x8e	; 142
	.db #0x43	; 67	'C'
	.db #0x44	; 68	'D'
	.db #0xc4	; 196
	.db #0xde	; 222
	.db #0xe9	; 233
	.db #0xcb	; 203
	.db #0x54	; 84	'T'
	.db #0x7b	; 123
	.db #0x94	; 148
	.db #0x32	; 50	'2'
	.db #0xa6	; 166
	.db #0xc2	; 194
	.db #0x23	; 35
	.db #0x3d	; 61
	.db #0xee	; 238
	.db #0x4c	; 76	'L'
	.db #0x95	; 149
	.db #0x0b	; 11
	.db #0x42	; 66	'B'
	.db #0xfa	; 250
	.db #0xc3	; 195
	.db #0x4e	; 78	'N'
	.db #0x08	; 8
	.db #0x2e	; 46
	.db #0xa1	; 161
	.db #0x66	; 102	'f'
	.db #0x28	; 40
	.db #0xd9	; 217
	.db #0x24	; 36
	.db #0xb2	; 178
	.db #0x76	; 118	'v'
	.db #0x5b	; 91
	.db #0xa2	; 162
	.db #0x49	; 73	'I'
	.db #0x6d	; 109	'm'
	.db #0x8b	; 139
	.db #0xd1	; 209
	.db #0x25	; 37
	.db #0x72	; 114	'r'
	.db #0xf8	; 248
	.db #0xf6	; 246
	.db #0x64	; 100	'd'
	.db #0x86	; 134
	.db #0x68	; 104	'h'
	.db #0x98	; 152
	.db #0x16	; 22
	.db #0xd4	; 212
	.db #0xa4	; 164
	.db #0x5c	; 92
	.db #0xcc	; 204
	.db #0x5d	; 93
	.db #0x65	; 101	'e'
	.db #0xb6	; 182
	.db #0x92	; 146
	.db #0x6c	; 108	'l'
	.db #0x70	; 112	'p'
	.db #0x48	; 72	'H'
	.db #0x50	; 80	'P'
	.db #0xfd	; 253
	.db #0xed	; 237
	.db #0xb9	; 185
	.db #0xda	; 218
	.db #0x5e	; 94
	.db #0x15	; 21
	.db #0x46	; 70	'F'
	.db #0x57	; 87	'W'
	.db #0xa7	; 167
	.db #0x8d	; 141
	.db #0x9d	; 157
	.db #0x84	; 132
	.db #0x90	; 144
	.db #0xd8	; 216
	.db #0xab	; 171
	.db #0x00	; 0
	.db #0x8c	; 140
	.db #0xbc	; 188
	.db #0xd3	; 211
	.db #0x0a	; 10
	.db #0xf7	; 247
	.db #0xe4	; 228
	.db #0x58	; 88	'X'
	.db #0x05	; 5
	.db #0xb8	; 184
	.db #0xb3	; 179
	.db #0x45	; 69	'E'
	.db #0x06	; 6
	.db #0xd0	; 208
	.db #0x2c	; 44
	.db #0x1e	; 30
	.db #0x8f	; 143
	.db #0xca	; 202
	.db #0x3f	; 63
	.db #0x0f	; 15
	.db #0x02	; 2
	.db #0xc1	; 193
	.db #0xaf	; 175
	.db #0xbd	; 189
	.db #0x03	; 3
	.db #0x01	; 1
	.db #0x13	; 19
	.db #0x8a	; 138
	.db #0x6b	; 107	'k'
	.db #0x3a	; 58
	.db #0x91	; 145
	.db #0x11	; 17
	.db #0x41	; 65	'A'
	.db #0x4f	; 79	'O'
	.db #0x67	; 103	'g'
	.db #0xdc	; 220
	.db #0xea	; 234
	.db #0x97	; 151
	.db #0xf2	; 242
	.db #0xcf	; 207
	.db #0xce	; 206
	.db #0xf0	; 240
	.db #0xb4	; 180
	.db #0xe6	; 230
	.db #0x73	; 115	's'
	.db #0x96	; 150
	.db #0xac	; 172
	.db #0x74	; 116	't'
	.db #0x22	; 34
	.db #0xe7	; 231
	.db #0xad	; 173
	.db #0x35	; 53	'5'
	.db #0x85	; 133
	.db #0xe2	; 226
	.db #0xf9	; 249
	.db #0x37	; 55	'7'
	.db #0xe8	; 232
	.db #0x1c	; 28
	.db #0x75	; 117	'u'
	.db #0xdf	; 223
	.db #0x6e	; 110	'n'
	.db #0x47	; 71	'G'
	.db #0xf1	; 241
	.db #0x1a	; 26
	.db #0x71	; 113	'q'
	.db #0x1d	; 29
	.db #0x29	; 41
	.db #0xc5	; 197
	.db #0x89	; 137
	.db #0x6f	; 111	'o'
	.db #0xb7	; 183
	.db #0x62	; 98	'b'
	.db #0x0e	; 14
	.db #0xaa	; 170
	.db #0x18	; 24
	.db #0xbe	; 190
	.db #0x1b	; 27
	.db #0xfc	; 252
	.db #0x56	; 86	'V'
	.db #0x3e	; 62
	.db #0x4b	; 75	'K'
	.db #0xc6	; 198
	.db #0xd2	; 210
	.db #0x79	; 121	'y'
	.db #0x20	; 32
	.db #0x9a	; 154
	.db #0xdb	; 219
	.db #0xc0	; 192
	.db #0xfe	; 254
	.db #0x78	; 120	'x'
	.db #0xcd	; 205
	.db #0x5a	; 90	'Z'
	.db #0xf4	; 244
	.db #0x1f	; 31
	.db #0xdd	; 221
	.db #0xa8	; 168
	.db #0x33	; 51	'3'
	.db #0x88	; 136
	.db #0x07	; 7
	.db #0xc7	; 199
	.db #0x31	; 49	'1'
	.db #0xb1	; 177
	.db #0x12	; 18
	.db #0x10	; 16
	.db #0x59	; 89	'Y'
	.db #0x27	; 39
	.db #0x80	; 128
	.db #0xec	; 236
	.db #0x5f	; 95
	.db #0x60	; 96
	.db #0x51	; 81	'Q'
	.db #0x7f	; 127
	.db #0xa9	; 169
	.db #0x19	; 25
	.db #0xb5	; 181
	.db #0x4a	; 74	'J'
	.db #0x0d	; 13
	.db #0x2d	; 45
	.db #0xe5	; 229
	.db #0x7a	; 122	'z'
	.db #0x9f	; 159
	.db #0x93	; 147
	.db #0xc9	; 201
	.db #0x9c	; 156
	.db #0xef	; 239
	.db #0xa0	; 160
	.db #0xe0	; 224
	.db #0x3b	; 59
	.db #0x4d	; 77	'M'
	.db #0xae	; 174
	.db #0x2a	; 42
	.db #0xf5	; 245
	.db #0xb0	; 176
	.db #0xc8	; 200
	.db #0xeb	; 235
	.db #0xbb	; 187
	.db #0x3c	; 60
	.db #0x83	; 131
	.db #0x53	; 83	'S'
	.db #0x99	; 153
	.db #0x61	; 97	'a'
	.db #0x17	; 23
	.db #0x2b	; 43
	.db #0x04	; 4
	.db #0x7e	; 126
	.db #0xba	; 186
	.db #0x77	; 119	'w'
	.db #0xd6	; 214
	.db #0x26	; 38
	.db #0xe1	; 225
	.db #0x69	; 105	'i'
	.db #0x14	; 20
	.db #0x63	; 99	'c'
	.db #0x55	; 85	'U'
	.db #0x21	; 33
	.db #0x0c	; 12
	.db #0x7d	; 125
_Rcon:
	.db #0x8d	; 141
	.db #0x01	; 1
	.db #0x02	; 2
	.db #0x04	; 4
	.db #0x08	; 8
	.db #0x10	; 16
	.db #0x20	; 32
	.db #0x40	; 64
	.db #0x80	; 128
	.db #0x1b	; 27
	.db #0x36	; 54	'6'
	.db #0x6c	; 108	'l'
	.db #0xd8	; 216
	.db #0xab	; 171
	.db #0x4d	; 77	'M'
	.db #0x9a	; 154
	.db #0x2f	; 47
	.db #0x5e	; 94
	.db #0xbc	; 188
	.db #0x63	; 99	'c'
	.db #0xc6	; 198
	.db #0x97	; 151
	.db #0x35	; 53	'5'
	.db #0x6a	; 106	'j'
	.db #0xd4	; 212
	.db #0xb3	; 179
	.db #0x7d	; 125
	.db #0xfa	; 250
	.db #0xef	; 239
	.db #0xc5	; 197
	.db #0x91	; 145
	.db #0x39	; 57	'9'
	.db #0x72	; 114	'r'
	.db #0xe4	; 228
	.db #0xd3	; 211
	.db #0xbd	; 189
	.db #0x61	; 97	'a'
	.db #0xc2	; 194
	.db #0x9f	; 159
	.db #0x25	; 37
	.db #0x4a	; 74	'J'
	.db #0x94	; 148
	.db #0x33	; 51	'3'
	.db #0x66	; 102	'f'
	.db #0xcc	; 204
	.db #0x83	; 131
	.db #0x1d	; 29
	.db #0x3a	; 58
	.db #0x74	; 116	't'
	.db #0xe8	; 232
	.db #0xcb	; 203
	.db #0x8d	; 141
	.db #0x01	; 1
	.db #0x02	; 2
	.db #0x04	; 4
	.db #0x08	; 8
	.db #0x10	; 16
	.db #0x20	; 32
	.db #0x40	; 64
	.db #0x80	; 128
	.db #0x1b	; 27
	.db #0x36	; 54	'6'
	.db #0x6c	; 108	'l'
	.db #0xd8	; 216
	.db #0xab	; 171
	.db #0x4d	; 77	'M'
	.db #0x9a	; 154
	.db #0x2f	; 47
	.db #0x5e	; 94
	.db #0xbc	; 188
	.db #0x63	; 99	'c'
	.db #0xc6	; 198
	.db #0x97	; 151
	.db #0x35	; 53	'5'
	.db #0x6a	; 106	'j'
	.db #0xd4	; 212
	.db #0xb3	; 179
	.db #0x7d	; 125
	.db #0xfa	; 250
	.db #0xef	; 239
	.db #0xc5	; 197
	.db #0x91	; 145
	.db #0x39	; 57	'9'
	.db #0x72	; 114	'r'
	.db #0xe4	; 228
	.db #0xd3	; 211
	.db #0xbd	; 189
	.db #0x61	; 97	'a'
	.db #0xc2	; 194
	.db #0x9f	; 159
	.db #0x25	; 37
	.db #0x4a	; 74	'J'
	.db #0x94	; 148
	.db #0x33	; 51	'3'
	.db #0x66	; 102	'f'
	.db #0xcc	; 204
	.db #0x83	; 131
	.db #0x1d	; 29
	.db #0x3a	; 58
	.db #0x74	; 116	't'
	.db #0xe8	; 232
	.db #0xcb	; 203
	.db #0x8d	; 141
	.db #0x01	; 1
	.db #0x02	; 2
	.db #0x04	; 4
	.db #0x08	; 8
	.db #0x10	; 16
	.db #0x20	; 32
	.db #0x40	; 64
	.db #0x80	; 128
	.db #0x1b	; 27
	.db #0x36	; 54	'6'
	.db #0x6c	; 108	'l'
	.db #0xd8	; 216
	.db #0xab	; 171
	.db #0x4d	; 77	'M'
	.db #0x9a	; 154
	.db #0x2f	; 47
	.db #0x5e	; 94
	.db #0xbc	; 188
	.db #0x63	; 99	'c'
	.db #0xc6	; 198
	.db #0x97	; 151
	.db #0x35	; 53	'5'
	.db #0x6a	; 106	'j'
	.db #0xd4	; 212
	.db #0xb3	; 179
	.db #0x7d	; 125
	.db #0xfa	; 250
	.db #0xef	; 239
	.db #0xc5	; 197
	.db #0x91	; 145
	.db #0x39	; 57	'9'
	.db #0x72	; 114	'r'
	.db #0xe4	; 228
	.db #0xd3	; 211
	.db #0xbd	; 189
	.db #0x61	; 97	'a'
	.db #0xc2	; 194
	.db #0x9f	; 159
	.db #0x25	; 37
	.db #0x4a	; 74	'J'
	.db #0x94	; 148
	.db #0x33	; 51	'3'
	.db #0x66	; 102	'f'
	.db #0xcc	; 204
	.db #0x83	; 131
	.db #0x1d	; 29
	.db #0x3a	; 58
	.db #0x74	; 116	't'
	.db #0xe8	; 232
	.db #0xcb	; 203
	.db #0x8d	; 141
	.db #0x01	; 1
	.db #0x02	; 2
	.db #0x04	; 4
	.db #0x08	; 8
	.db #0x10	; 16
	.db #0x20	; 32
	.db #0x40	; 64
	.db #0x80	; 128
	.db #0x1b	; 27
	.db #0x36	; 54	'6'
	.db #0x6c	; 108	'l'
	.db #0xd8	; 216
	.db #0xab	; 171
	.db #0x4d	; 77	'M'
	.db #0x9a	; 154
	.db #0x2f	; 47
	.db #0x5e	; 94
	.db #0xbc	; 188
	.db #0x63	; 99	'c'
	.db #0xc6	; 198
	.db #0x97	; 151
	.db #0x35	; 53	'5'
	.db #0x6a	; 106	'j'
	.db #0xd4	; 212
	.db #0xb3	; 179
	.db #0x7d	; 125
	.db #0xfa	; 250
	.db #0xef	; 239
	.db #0xc5	; 197
	.db #0x91	; 145
	.db #0x39	; 57	'9'
	.db #0x72	; 114	'r'
	.db #0xe4	; 228
	.db #0xd3	; 211
	.db #0xbd	; 189
	.db #0x61	; 97	'a'
	.db #0xc2	; 194
	.db #0x9f	; 159
	.db #0x25	; 37
	.db #0x4a	; 74	'J'
	.db #0x94	; 148
	.db #0x33	; 51	'3'
	.db #0x66	; 102	'f'
	.db #0xcc	; 204
	.db #0x83	; 131
	.db #0x1d	; 29
	.db #0x3a	; 58
	.db #0x74	; 116	't'
	.db #0xe8	; 232
	.db #0xcb	; 203
	.db #0x8d	; 141
	.db #0x01	; 1
	.db #0x02	; 2
	.db #0x04	; 4
	.db #0x08	; 8
	.db #0x10	; 16
	.db #0x20	; 32
	.db #0x40	; 64
	.db #0x80	; 128
	.db #0x1b	; 27
	.db #0x36	; 54	'6'
	.db #0x6c	; 108	'l'
	.db #0xd8	; 216
	.db #0xab	; 171
	.db #0x4d	; 77	'M'
	.db #0x9a	; 154
	.db #0x2f	; 47
	.db #0x5e	; 94
	.db #0xbc	; 188
	.db #0x63	; 99	'c'
	.db #0xc6	; 198
	.db #0x97	; 151
	.db #0x35	; 53	'5'
	.db #0x6a	; 106	'j'
	.db #0xd4	; 212
	.db #0xb3	; 179
	.db #0x7d	; 125
	.db #0xfa	; 250
	.db #0xef	; 239
	.db #0xc5	; 197
	.db #0x91	; 145
	.db #0x39	; 57	'9'
	.db #0x72	; 114	'r'
	.db #0xe4	; 228
	.db #0xd3	; 211
	.db #0xbd	; 189
	.db #0x61	; 97	'a'
	.db #0xc2	; 194
	.db #0x9f	; 159
	.db #0x25	; 37
	.db #0x4a	; 74	'J'
	.db #0x94	; 148
	.db #0x33	; 51	'3'
	.db #0x66	; 102	'f'
	.db #0xcc	; 204
	.db #0x83	; 131
	.db #0x1d	; 29
	.db #0x3a	; 58
	.db #0x74	; 116	't'
	.db #0xe8	; 232
	.db #0xcb	; 203
	.area XINIT   (CODE)
	.area CABS    (ABS,CODE)
