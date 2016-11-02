#include "emulation.h"
#include "cpu/cpu.h"
#include "decode/decode.h"
#include <stdio.h>
#include <stdlib.h>
#include "emu_helper.h"


exec_status_t clr_emu(vcpu_t* vcpu, struct instr_desc *instr, instr_t op)
{
	uint16_t dst_disp = 0, dst = 0, dst_mode = 0, isa_mode = 0;

	ISA_MODE(op, isa_mode);	
	ADDRESS_MODE_LOW(op, dst_mode);
	GET_DST(op, dst_disp);

	uint8_t* dst_addr;

	fetch_op_general(vcpu, dst_disp, dst_mode, isa_mode, &dst_addr);		

	dst = 0;
	
	writeback_src_ops(vcpu, dst_disp, dst_mode, isa_mode);
	writeback_dst_ops(vcpu, dst, dst_disp, dst_mode, isa_mode, dst_addr);	

	CLEAR_N(vcpu);
	SET_Z(vcpu);
	CLEAR_V(vcpu);
	CLEAR_C(vcpu);

	return EXEC_SUCCESS;
}

exec_status_t com_emu(vcpu_t* vcpu, struct instr_desc *instr, instr_t op)
{
	uint16_t dst_disp = 0, dst = 0, dst_mode = 0, isa_mode = 0;

	ISA_MODE(op, isa_mode);	
	ADDRESS_MODE_LOW(op, dst_mode);
	GET_DST(op, dst_disp);

	uint8_t* dst_addr;

	dst = fetch_op_general(vcpu, dst_disp, dst_mode, isa_mode, &dst_addr);		

	dst = ~(dst);
	
	writeback_src_ops(vcpu, dst_disp, dst_mode, isa_mode);
	writeback_dst_ops(vcpu, dst, dst_disp, dst_mode, isa_mode, dst_addr);	

	if (dst & 0x8000)
		SET_N(vcpu);
	else
		CLEAR_N(vcpu);

	if (dst == 0)
		SET_Z(vcpu);
	else
		CLEAR_Z(vcpu);

	CLEAR_V(vcpu);
	SET_C(vcpu);

	return EXEC_SUCCESS;
}

exec_status_t inc_emu(vcpu_t* vcpu, struct instr_desc *instr, instr_t op)
{
	uint16_t dst_disp = 0, dst = 0, dst_mode = 0, isa_mode = 0;

	ISA_MODE(op, isa_mode);	
	ADDRESS_MODE_LOW(op, dst_mode);
	GET_DST(op, dst_disp);

	uint8_t* dst_addr;

	dst = fetch_op_general(vcpu, dst_disp, dst_mode, isa_mode, &dst_addr);	

	dst = dst + 1;

	writeback_src_ops(vcpu, dst_disp, dst_mode, isa_mode);
	writeback_dst_ops(vcpu, dst, dst_disp, dst_mode, isa_mode, dst_addr);

	if (dst < 0)
		SET_N(vcpu);
	else
		CLEAR_N(vcpu);

	if (dst == 0)
		SET_Z(vcpu);
	else
		CLEAR_Z(vcpu);	

	if (dst == 0x7FFF)		// 077777 in octal
		SET_V(vcpu);
	else
		CLEAR_V(vcpu);

	return EXEC_SUCCESS;
}

exec_status_t dec_emu(vcpu_t* vcpu, struct instr_desc *instr, instr_t op)
{
	uint16_t dst_disp = 0, dst = 0, dst_mode = 0, isa_mode = 0, src_dst = 0;

	ISA_MODE(op, isa_mode);	
	ADDRESS_MODE_LOW(op, dst_mode);
	GET_DST(op, dst_disp);

	uint8_t* dst_addr;

	dst = fetch_op_general(vcpu, dst_disp, dst_mode, isa_mode, &dst_addr);	
	src_dst = dst;

	dst = dst - 1;

	writeback_src_ops(vcpu, dst_disp, dst_mode, isa_mode);
	writeback_dst_ops(vcpu, dst, dst_disp, dst_mode, isa_mode, dst_addr);

	if (dst < 0)
		SET_N(vcpu);
	else
		CLEAR_N(vcpu);

	if (dst == 0)
		SET_Z(vcpu);
	else
		CLEAR_Z(vcpu);	

	if (src_dst == 0x8000)		// 100000 in octal
		SET_V(vcpu);
	else
		CLEAR_V(vcpu);

	return EXEC_SUCCESS;
}

exec_status_t neg_emu(vcpu_t* vcpu, struct instr_desc *instr, instr_t op)
{
	uint16_t dst_disp = 0, dst = 0, dst_mode = 0, isa_mode = 0;

	ISA_MODE(op, isa_mode);	
	ADDRESS_MODE_LOW(op, dst_mode);
	GET_DST(op, dst_disp);

	uint8_t* dst_addr;

	dst = fetch_op_general(vcpu, dst_disp, dst_mode, isa_mode, &dst_addr);	

	dst = -dst;

	writeback_src_ops(vcpu, dst_disp, dst_mode, isa_mode);
	writeback_dst_ops(vcpu, dst, dst_disp, dst_mode, isa_mode, dst_addr);

	if (dst < 0)
		SET_N(vcpu);
	else
		CLEAR_N(vcpu);

	if (dst == 0)
	{	
		SET_Z(vcpu);
		CLEAR_C(vcpu);	
	}
	else
	{	
		CLEAR_Z(vcpu);	
		SET_C(vcpu);
	}

	if (dst == 0x8000)		// 100000 in octal
		SET_V(vcpu);
	else
		CLEAR_V(vcpu);

	return EXEC_SUCCESS;
}


exec_status_t tst_emu(vcpu_t* vcpu, struct instr_desc *instr, instr_t op)
{
	uint16_t dst_disp = 0, dst = 0, dst_mode = 0, isa_mode = 0;

	ISA_MODE(op, isa_mode);	
	ADDRESS_MODE_LOW(op, dst_mode);
	GET_DST(op, dst_disp);

	uint8_t* dst_addr;

	dst = fetch_op_general(vcpu, dst_disp, dst_mode, isa_mode, &dst_addr);	

	dst = dst;

	writeback_src_ops(vcpu, dst_disp, dst_mode, isa_mode);
	writeback_dst_ops(vcpu, dst, dst_disp, dst_mode, isa_mode, dst_addr);

	if (dst < 0)
		SET_N(vcpu);
	else
		CLEAR_N(vcpu);

	if (dst == 0)	
		SET_Z(vcpu);
	
	else
		CLEAR_Z(vcpu);	
	
	CLEAR_V(vcpu);
	CLEAR_C(vcpu);

	return EXEC_SUCCESS;
}

exec_status_t asr_emu(vcpu_t* vcpu, struct instr_desc *instr, instr_t op)
{
	uint16_t dst_disp = 0, dst = 0, dst_mode = 0, isa_mode = 0;

	ISA_MODE(op, isa_mode);	
	ADDRESS_MODE_LOW(op, dst_mode);
	GET_DST(op, dst_disp);

	uint8_t* dst_addr;

	dst = fetch_op_general(vcpu, dst_disp, dst_mode, isa_mode, &dst_addr);

	uint16_t high_bit = 0;
	uint16_t low_bit = 0;	

	if (isa_mode)						
	{
		high_bit = (dst & 0x0080);
		low_bit = (dst & 0x0001);
		dst = (dst >> 1) | high_bit;
		high_bit = high_bit >> 7;				
	}	
	else
	{
		high_bit = (dst & 0x8000);
		low_bit = (dst & 0x0001);
		dst = (dst >> 1) | high_bit;
		high_bit = high_bit >> 15;
	}
	
	writeback_src_ops(vcpu, dst_disp, dst_mode, isa_mode);
	writeback_dst_ops(vcpu, dst, dst_disp, dst_mode, isa_mode, dst_addr);	
/*
	if (high_bit)
		SET_N(vcpu);
	else
		CLEAR_N(vcpu);
*/
	LOAD_N(vcpu, high_bit);

	if (dst == 0)
		SET_Z(vcpu);
	else
		CLEAR_Z(vcpu);
/*
	if (high_bit ^ low_bit)
		SET_V(vcpu);
	else
		CLEAR_V(vcpu);
*/
	LOAD_V(vcpu, high_bit ^ low_bit);
/*
	if (low_bit)					// FIXME: Need to implement macroses with ability to just load bit in the psw
		SET_C(vcpu);
	else
		CLEAR_C(vcpu);
*/
	LOAD_C(vcpu, low_bit);

	return EXEC_SUCCESS;
}

exec_status_t asl_emu(vcpu_t* vcpu, struct instr_desc *instr, instr_t op)
{
	uint16_t dst_disp = 0, dst = 0, dst_mode = 0, isa_mode = 0;

	ISA_MODE(op, isa_mode);	
	ADDRESS_MODE_LOW(op, dst_mode);
	GET_DST(op, dst_disp);

	uint8_t* dst_addr;

	dst = fetch_op_general(vcpu, dst_disp, dst_mode, isa_mode, &dst_addr);

	uint16_t high_bit = 0;
	uint16_t neg_bit = 0;

	if (isa_mode)
	{
		high_bit = (dst & 0x0080) >> 7;
		neg_bit = ((dst << 1) & 0x0080) >> 7;
	}
	else
	{	
		high_bit = (dst & 0x8000) >> 15;		
		neg_bit = ((dst << 1) & 0x8000) >> 15;
	}	

	dst = dst << 1;
/*
	if (neg_bit)				// Need to check such issue, I am not sure that in byte 
		SET_N(vcpu);			// instructions high bit of the BYTE is taken into account
	else
		CLEAR_N(vcpu);
*/
	LOAD_N(vcpu, neg_bit);

	if (dst == 0)
		SET_Z(vcpu);
	else
		CLEAR_Z(vcpu);
/*
	if (neg_bit ^ high_bit)
		SET_V(vcpu);
	else
		CLEAR_V(vcpu);
*/
	LOAD_V(vcpu, neg_bit ^ high_bit);
/*
	if (high_bit)
		SET_C(vcpu);
	else
		CLEAR_C(vcpu);
*/

	LOAD_C(vcpu, high_bit);

	return EXEC_SUCCESS;
}


exec_status_t ror_emu(vcpu_t* vcpu, struct instr_desc *instr, instr_t op)
{
	uint16_t dst_disp = 0, dst = 0, dst_mode = 0, isa_mode = 0;

	ISA_MODE(op, isa_mode);	
	ADDRESS_MODE_LOW(op, dst_mode);
	GET_DST(op, dst_disp);

	uint8_t* dst_addr;

	dst = fetch_op_general(vcpu, dst_disp, dst_mode, isa_mode, &dst_addr);	

	uint16_t prev_bit_c = 0, low_bit = 0;;

	GET_C(vcpu, prev_bit_c);
	low_bit = dst & 0x0001;

	dst = dst >> 1;
	
	if (isa_mode)
		dst = dst | (prev_bit_c << 15);
	else 
		dst = dst | (prev_bit_c << 7);

/*
	if (prev_bit_c)
		SET_N(vcpu);
	else
		CLEAR_N(vcpu);
*/
	LOAD_N(vcpu, prev_bit_c);

	if (dst == 0)
		SET_Z(vcpu);
	else
		CLEAR_Z(vcpu);

	LOAD_V(vcpu, prev_bit_c ^ low_bit);
/*
	if (prev_bit_c ^ low_bit)
		SET_V(vcpu);
	else
		CLEAR_V(vcpu);
*/

	LOAD_C(vcpu, low_bit);	
/*
	if (low_bit)
		SET_C(vcpu);
	else
		CLEAR_C(vcpu);
*/
	return EXEC_SUCCESS;
}


exec_status_t rol_emu(vcpu_t* vcpu, struct instr_desc *instr, instr_t op)
{
	uint16_t dst_disp = 0, dst = 0, dst_mode = 0, isa_mode = 0;

	ISA_MODE(op, isa_mode);	
	ADDRESS_MODE_LOW(op, dst_mode);
	GET_DST(op, dst_disp);

	uint8_t* dst_addr;

	dst = fetch_op_general(vcpu, dst_disp, dst_mode, isa_mode, &dst_addr);	

	uint16_t prev_bit_c = 0, high_bit = 0;;

	GET_C(vcpu, prev_bit_c);
	
	if (isa_mode)
		high_bit = dst & 0x0080;  
	else
		high_bit = dst & 0x8000;

	dst = dst << 1;
	dst = dst | prev_bit_c;

//	LOAD_N(vcpu, );			// FIXME: Need to fix N flag operations in all functions

	if (dst == 0)
		SET_Z(vcpu);
	else
		CLEAR_Z(vcpu);

	LOAD_V(vcpu, prev_bit_c ^ low_bit);
/*
	if (prev_bit_c ^ low_bit)
		SET_V(vcpu);
	else
		CLEAR_V(vcpu);
*/

	LOAD_C(vcpu, low_bit);	
/*
	if (low_bit)
		SET_C(vcpu);
	else
		CLEAR_C(vcpu);
*/
	return EXEC_SUCCESS;
}

