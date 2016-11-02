#ifndef EMULATION_H
#define EMULATION_H

#include "decode/decode.h"
#include "cpu/cpu.h"

/* double operand */

exec_status_t mov_emu(vcpu_t* vcpu, struct instr_desc *instr, instr_t op);
exec_status_t add_emu(vcpu_t* vcpu, struct instr_desc *instr, instr_t op);
exec_status_t sub_emu(vcpu_t* vcpu, struct instr_desc *instr, instr_t op);


/* single operand */

exec_status_t clr_emu(vcpu_t* vcpu, struct instr_desc *instr, instr_t op);
exec_status_t com_emu(vcpu_t* vcpu, struct instr_desc *instr, instr_t op);
exec_status_t inc_emu(vcpu_t* vcpu, struct instr_desc *instr, instr_t op);
exec_status_t dec_emu(vcpu_t* vcpu, struct instr_desc *instr, instr_t op);
exec_status_t neg_emu(vcpu_t* vcpu, struct instr_desc *instr, instr_t op);
exec_status_t tst_emu(vcpu_t* vcpu, struct instr_desc *instr, instr_t op);
exec_status_t asr_emu(vcpu_t* vcpu, struct instr_desc *instr, instr_t op);
exec_status_t asl_emu(vcpu_t* vcpu, struct instr_desc *instr, instr_t op);
exec_status_t ror_emu(vcpu_t* vcpu, struct instr_desc *instr, instr_t op);

#endif //EMULATION_H