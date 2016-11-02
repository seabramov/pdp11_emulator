#ifndef CPU_H
#define CPU_H

#include <stdint.h>

#define GPR_NUM	8
#define ADDR_SPACE_SIZE	0x20000	 //128K in order to contain registers, need to fix it more accurate later
#define MEM_SPACE_SIZE	0x10000	 // 64K	

#define PS_ADDR			0x0FFFE
#define PS_INIT_MASK 	0x0003		//0b0000000000000011	

#define GET_C(vcpu, bit_c)	\
	do {	\
		bit_c = (*(vcpu->psw)).c;	\
	} while (0)

#define LOAD_N(vcpu, flag)	\
	do {	\
		(*(vcpu->psw)).n = flag;	\
	} while (0)

#define LOAD_Z(vcpu, flag)	\
	do {	\
		(*(vcpu->psw)).z = flag;	\
	} while (0)

#define LOAD_V(vcpu, flag)	\
	do {	\
		(*(vcpu->psw)).v = flag;	\
	} while (0)

#define LOAD_C(vcpu, flag)	\
	do {	\
		(*(vcpu->psw)).c = flag;	\
	} while (0)		

#define SET_C(vcpu)	\
	do {	\
		(*(vcpu->psw)).reg_val |= 0x8000;	\
	} while (0)

#define SET_V(vcpu)	\
	do {	\
		(*(vcpu->psw)).reg_val |= 0x4000;	\
	} while (0)

#define SET_Z(vcpu)	\
	do {	\
		(*(vcpu->psw)).reg_val |= 0x2000;	\
	} while (0)

#define SET_N(vcpu)	\
	do {	\
		(*(vcpu->psw)).reg_val |= 0x1000;	\
	} while (0)

#define SET_T(vcpu)	\
	do {	\
		(*(vcpu->psw)).reg_val |= 0x0800;	\
	} while (0)

#define CLEAR_C(vcpu)	\
	do {	\
		(*(vcpu->psw)).reg_val &= 0x7fff;	\
	} while (0)

#define CLEAR_V(vcpu)	\
	do {	\
		(*(vcpu->psw)).reg_val &= 0xbfff;	\
	} while (0)

#define CLEAR_Z(vcpu)	\
	do {	\
		(*(vcpu->psw)).reg_val &= 0xdfff;	\
	} while (0)

#define CLEAR_N(vcpu)	\
	do {	\
		(*(vcpu->psw)).reg_val &= 0xefff;	\
	} while (0)

#define CLEAR_T(vcpu)	\
	do {	\
		(*(vcpu->psw)).reg_val &= 0xf7ff;	\
	} while (0)


#define PS_INIT(vcpu) \
	do {	\
		(*(vcpu->psw)).reg_val &= PS_INIT_MASK;	\
	} while (0)


typedef uint16_t reg16_t;
typedef uint16_t pdp_reg;
typedef uint16_t instr_t;


typedef enum reg_id
{
	REG0,
	REG1,
	REG2,
	REG3,
	REG4,
	REG5,
	REG6,
	REG7,

	SP = REG6,
	PC = REG7
}	reg_t;

typedef struct ps	
{
	union 
	{
		reg16_t reg_val;
		struct 
		{	
			reg16_t curmode:2;					
			reg16_t prevmode:2;
			reg16_t reserved:4;
			reg16_t priority:3;
			reg16_t t:1;
			reg16_t n:1;
			reg16_t z:1;
			reg16_t v:1;
			reg16_t c:1;
		};
	};
} __attribute__((packed)) ps_t;


typedef struct vcpu
{
	pdp_reg* regs;
	ps_t* psw;
	void* mem_entry;

} vcpu_t;

typedef enum exec_status
{	
	EXEC_SUCCESS,
	EXEC_UNDEFINED
} exec_status_t;



#endif	// CPU_H