#ifndef DECODE_H
#define DECODE_H

#include "cpu/cpu.h"

struct instr_desc;


typedef enum decode_status
{
	DECODE_SUCCESS,
	DECODE_UNDEFINED	
} decode_status_t;

typedef decode_status_t (*decode_call_t)(vcpu_t *vcpu, struct instr_desc *instr, instr_t op);
typedef exec_status_t (*emu_call_t)(vcpu_t *vcpu, struct instr_desc *instr, instr_t op);

typedef struct instr_desc
{
	uint16_t mask;
	uint16_t value;
	const char *name;
	emu_call_t execute;
} instr_desc_t;


/* ADDRESSING MODES */

#define REG_ADDR			0
#define REG_DEF_ADDR		1
#define AUTOINC_ADDR		2
#define AUTOINC_DEF_ADDR	3
#define AUTODEC_ADDR		4
#define AUTODEC_DEF_ADDR	5
#define INDEX_ADDR			6
#define INDEX_DEF_ADDR		7
#define IMM_PC				23
#define ABS_PC				31
#define REL_PC				55
#define REL_DEF_PC			63


#define ISA_MODE(op, isa_mode)	\
	do {	\
		isa_mode = op & 0x8000;	\
	} while (0)

#define ADDRESS_MODE_LOW(op, mode) \
	do {	\
		mode = op & 0x0038;	\
	} while (0)

#define ADDRESS_MODE_HIGH(op, mode)	\
	do {	\
		mode = (op & 0x0E00) >> 9;	\
	} while (0)

#define GET_SOURCE(op, src)	\
	do {	\
		src = (op & 0x01C0) >> 6;	\
	} while(0)

#define GET_DST(op, dst)	\
	do {	\
		dst = op & 0x0007;	\
	} while(0)

#define GET_REG(op, src)	\
	do {	\
		src = op & 0x01C0;	\
	} while(0)

#define GET_DST_SRC(op, dst)	\
	do {	\
		dst = op & 0x0007;	\
	} while(0)


#endif // DECODE_H