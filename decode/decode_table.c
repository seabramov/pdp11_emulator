#include "decode.h"
#include "cpu/cpu.h"
#include <stdlib.h>
#include "emulation/emulation.h"

/*
typedef struct instr_desc
{
	uint16_t mask;
	uint16_t value;
	const char *name;
	emu_call_t execute;
} instr_desc_t;
*/


instr_desc_t instr_table[] = 
{
	{0x7000, 0x1000, "mov", mov_emu},
	{0xf000, 0x6000, "add", add_emu},	
	{0xf000, 0xe000, "sub", sub_emu},
	{0x7fc0, 0x0a00, "clr", clr_emu},
	{0x7FC0, 0x0A40, "com", com_emu},
	{0x7FC0, 0x0A80, "inc", inc_emu},
	{0x7FC0, 0x0AC0, "dec", dec_emu},
	{0x7FC0, 0x0B00, "neg", neg_emu},
	{0x7FC0, 0x0BC0, "tst", tst_emu},
	{0x7FC0, 0x0C80, "asr", asr_emu},
	{0x7FC0, 0x0CC0, "asl", asl_emu},
	{0x7FC0, 0x0C00, "ror", ror_emu},
	{0x7FC0, 0x0C40, "rol", rol_emu},
	{0x0, 0x0, NULL, NULL}
};


// rol 0000110001000000   
//mask 0111111111000000