/**

	File:		hw.c

	Project:	DCPU-16 Tools
	Component:	LibDCPU-vm

	Authors:	José Manuel Díez

	Description:	Handles opcode instructions in the
			virtual machine.

**/

#define PRIVATE_VM_ACCESS

#if 0 /* Charge */
#include <stdlib.h>
#include <stdio.h>
#include <debug.h>
#endif
#include "dcpubase.h"
#include "hw.h"

#define HW_MAX 0x1000

int vm_hw_connected[HW_MAX];
hw_t vm_hw_list[HW_MAX];

uint16_t vm_hw_register(vm_t* vm, hw_t hardware)
{
	uint16_t id = 0;

	while (vm_hw_connected[id] != 0 && id < HW_MAX)
		id++;

	if (id >= HW_MAX)
	{
		vm_halt(vm, "unable to register hardware, maximum reached!");
		return 0;
	}
	
#if 0 /* Charge */
	printd(LEVEL_DEBUG, "assigned id %d: 0x%08X\n", id, hardware.id);
#endif
	vm_hw_connected[id] = 1;
	vm_hw_list[id] = hardware;

	return id;
}

void vm_hw_unregister(vm_t* vm, uint16_t id)
{
	vm_hw_connected[id] = 0;
}

void vm_hw_interrupt(vm_t* vm, uint16_t index)
{
	hw_t device = vm_hw_list[index];

#if 0 /* Charge */
	if (vm->debug) printd(LEVEL_DEBUG, "\nInterrupting device 0x%04X (0x%08X): %p\n", index, device.id, device.handler);
#endif

	if (device.handler != NULL)
		device.handler(vm, device.userdata);
}

uint16_t vm_hw_count(vm_t* vm)
{
	uint16_t i = 0;

	for (i = 0; i < HW_MAX; i++)
	{
		if (vm_hw_connected[i] == 0)
			return i;
	}

	return HW_MAX;
}

hw_t vm_hw_get_device(vm_t* vm, uint16_t index)
{
	hw_t device = vm_hw_list[index];
	return device;
}
