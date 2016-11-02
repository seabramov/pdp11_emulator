#include <stdio.h>
#include "cpu/cpu.h"
#include <stdlib.h>	// for malloc
#include <string.h>

/* TODO:
	Implement sp addressing
	Check byte operations
*/

int main()
{
	vcpu_t* vcpu = (vcpu_t*)malloc(sizeof(vcpu_t));

	emu_init(vcpu);	
	
	exec_status_t exec_st = EXEC_SUCCESS;
	
	vcpu_print(vcpu);

	while (exec_st == EXEC_SUCCESS)
	{
		exec_st = cpu_exec(vcpu);			
		vcpu_print(vcpu);
	}


	return 0;
}