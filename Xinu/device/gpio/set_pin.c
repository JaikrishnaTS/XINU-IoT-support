#include <xinu.h>

devcall set_pin(struct dentry *devptr, uint32 bank_id, uint32 pin_number, uint32 mode)
{
        // mode is 0 for output, 1 for output

	if(bank_id > 3 || bank_id < 0 || pin_number > 31 || pin_number < 0)
	{
		return SYSERR;
	}
	uint32 *pin_pointer;

	if(bank_id == 0)
	{
		pin_pointer =(uint32 *) GPIO0_BASE;
	}
	else if(bank_id == 1)
	{
		pin_pointer =(uint32 *) GPIO1_BASE;
	}
	else if(bank_id == 2)
	{
		pin_pointer = (uint32 *)GPIO2_BASE;
	}
	else if(bank_id == 3)
	{
		pin_pointer = (uint32 *)GPIO3_BASE;
	}	

	if(mode == 0)
	{
		pin_pointer[OE_ADDR / 4] &= 0xFFFFFFFF ^ (1 << pin_number);
	}
	else if(mode == 1)
	{
		pin_pointer[OE_ADDR / 4] &= 0xFFFFFFFF ^ (0 << pin_number);
	}

	return OK;
}
