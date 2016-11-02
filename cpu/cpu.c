#include "cpu.h"
#include <stdio.h>
#include <sys/mman.h>	// for mmap
#include <stdlib.h>	// for abort
#include <decode/decode.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>



uint8_t exec_array[] = 
{
	0b01000000, 0b00011100, 0b00001010, 0b00000000,
	0b10000000, 0b10001100, 0b01111111, 0b01111111, 
	0b01111111, 0b01111111, 0b10010001, 0b10000000,
	0b01111111, 0b01111111, 0b01111111, 0b01111111,
	0b01111111, 0b01111111, 0b01111111, 0b01111111, 
	0b00000011, 0b00000000, 0b01111111, 0b01111111
};
// 	0b00000100, 0b01100100, 0b01111111, 0b01111111,


int read_file(char* path, void* mem)
{
/*
	int fd = open(path, O_RDWR);

	if (fd == -1)
	{	
		printf("Can not open file \n");
		abort();
	}

	long long size = lseek(fd, 0, SEEK_END);

	printf("number of rom bytes: %lld\n", size);
*/

	int size = sizeof(exec_array);

	printf("array size: %d\n", size);

	memcpy(mem, exec_array, size);

	int i = 0;

	for (i = 0; i < size; i++)
	{
		printf("mem: 0x%x\n", ((uint8_t*)mem)[i]);

	}

/*
	char* buf = (uint8_t*)malloc(sizeof(char) * size);
	memset(buf, 0, size);

	int rd = read(fd, buf, size);

	if (rd == -1)
	{
		printf("Read error \n");
		abort();
	}

	memcpy(mem, buf, size);

	printf("mem: 0x%x\n", *((uint16_t*)mem));

	free(buf);
*/
	return 0;
}



int vcpu_init(vcpu_t* vcpu, void* mem)
{
	printf("vcpu init\n");	
	vcpu->mem_entry = mem;
	vcpu->regs = (uint16_t*)((uint8_t*)mem + MEM_SPACE_SIZE);

	vcpu->regs[PC] = 0x0000;
//	vcpu->regs[SP];			// FIXME: Need to deal with it 

	vcpu->psw = (uint16_t*)((uint8_t*)mem + PS_ADDR);
	
	PS_INIT(vcpu);

	// FIXME: if to write LOAD_N() here 
	// SEGMENTATION FAULT WILL OCCUR						

	read_file("rom_file.txt", vcpu->mem_entry);

	return 0;
}

int emu_init(vcpu_t* vcpu)
{
	void* mem_entry = mmap(NULL, ADDR_SPACE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0); 

	if (mem_entry == (void*)-1)
	{
		printf("Mem allocation failed (mmap)\n");
		abort();
	}

	vcpu_init(vcpu, mem_entry);

	return 0;
}

uint16_t fetch_instr(vcpu_t* vcpu)
{
	printf("Instruction fetch \n");
		
	uint16_t op = 0;

	printf("PC: 0x%llx\n", vcpu->regs[PC]);

	memcpy(&op, (uint8_t*)(vcpu->mem_entry) + vcpu->regs[PC], sizeof(uint8_t) * 2);

//	printf("mem value: 0x%x\n", *(uint16_t*)((uint8_t*)(vcpu->mem_entry) + vcpu->regs[PC]));

	printf("op: 0x%x\n", op);

	int i = 0;
/*
	for (i = 0; i < 11; i++)
	{
		printf("mem entry: 0x%x\n", ((uint8_t*)vcpu->mem_entry)[i]);

	}
*/
	vcpu->regs[PC] += 2;

	return op;
}

void vcpu_print(vcpu_t* vcpu)
{
	printf("REG 0: 0x%x\n", vcpu->regs[REG0]);
	printf("REG 1: 0x%x\n", vcpu->regs[REG1]);
	printf("REG 2: 0x%x\n", vcpu->regs[REG2]);
	printf("REG 3: 0x%x\n", vcpu->regs[REG3]);
	printf("REG 4: 0x%x\n", vcpu->regs[REG4]);
	printf("REG 5: 0x%x\n", vcpu->regs[REG5]);
	printf("REG 6: 0x%x\n", vcpu->regs[SP]);
	printf("REG 7: 0x%x\n", vcpu->regs[PC]);						
}


exec_status_t cpu_exec(vcpu_t* vcpu)
{
	instr_t op = fetch_instr(vcpu);

	const instr_desc_t* instr = lookup_table(op);

	if (instr == NULL)
		return EXEC_UNDEFINED;		

	return instr->execute(vcpu, instr, op);
}

