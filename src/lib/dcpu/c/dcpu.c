/**

	File:		dcpu.c

	Project:	DCPU-16 Tools
	Component:	LibDCPU-vm

	Authors:	James Rhodes

	Description:	Handles high-level operations performed
			on the virtual machine (such as creation).

**/

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#if 0 /* Charge */
#include <libtcod.h>
#endif
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#if 0 /* Charge */
#include <debug.h>
#endif
#include "dcpu.h"
#include "dcpubase.h"
#include "dcpuhook.h"
#if 0 /* Charge */
#include "hwio.h"
#include "hwtimer.h"
#endif
#include "hw.h"

void vm_init(vm_t* vm, bool init_memory)
{
	unsigned int i;

	for (i = 0; i < 0x8; i++)
		vm->registers[i] = 0x0;

	vm->pc = 0x0;
	vm->sp = 0x0;
	vm->ex = 0x0;
	vm->ia = 0x0;

	if (init_memory)
	{
		for (i = 0; i < 0x10000; i++)
			vm->ram[i] = 0x0;
	}

	vm->sleep_cycles = 0;
	vm->dummy = 0x0;
	vm->halted = false;
	vm->skip = false;
#if 0 /* Charge */
	printd(LEVEL_DEBUG, "turning off interrupt queue\n");
#endif
	vm->queue_interrupts = false;
	vm->irq_count = 0;
	vm->dump = NULL;
	for (i = 0; i < 256; i++)
		vm->irq[i] = 0x0;

	return;
}

vm_t* vm_create()
{
	// Define variables.
	vm_t* new_vm;

	// Allocate and wipe vm memory.
	new_vm = (vm_t*)malloc(sizeof(vm_t));
	vm_init(new_vm, true);
	new_vm->debug = false;

	// Return.
	return new_vm;
}

void vm_free(vm_t* vm)
{
#if 0 /* Charge */
	// Shutdown components.
	vm_hw_io_free(vm);
	vm_hw_timer_free(vm);
#endif

	// Free the memory.
	free(vm);
}

void vm_flash(vm_t* vm, uint16_t memory[0x10000])
{
	// Flash the VM's memory from the specified array.
	unsigned int i;
	vm_init(vm, false);

	for (i = 0; i < 0x10000; i++)
		vm->ram[i] = memory[i];
}

#ifdef __EMSCRIPTEN__
vm_t* __emscripten_vm;

void __emscripten_vm_cycle()
{
	if (__emscripten_vm->halted)
		emscripten_cancel_main_loop();
	
	vm_cycle(__emscripten_vm);
}

void vm_execute(vm_t* vm, const char* execution_dump)
{
	__emscripten_vm = vm;

	// FIXME: Emscripten ignores execution dump option.
	emscripten_set_main_loop(__emscripten_vm_cycle, 60);
}
#else
void vm_execute(vm_t* vm, const char* execution_dump)
{
	double cycles = 0;
	int start = clock();
	
	if (execution_dump != NULL)
		vm->dump = fopen(execution_dump, "w");

	// Execute the memory using DCPU-16 specifications.
	while (!vm->halted)
	{
		vm_cycle(vm);
		cycles++;

		if (cycles >= 100000.f / CLOCKS_PER_SEC)
		{
			//printd(LEVEL_DEFAULT, "clock is %u, will wait until %u\n", clock(), start + (1.0f / CLOCKS_PER_SEC));
			while (clock() < start + 1) ;
			start += 1;
			cycles -= 100000.f / CLOCKS_PER_SEC;
			//printd(LEVEL_DEFAULT, "executed %f cycles (waited %u clocks)", (100000.f / CLOCKS_PER_SEC), 1);
		}
	}
	
	if (vm->dump != NULL)
	{
		fclose(vm->dump);
		vm->dump = NULL;
	}
}
#endif
