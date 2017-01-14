#include <xinu.h>

devcall digital_write(struct dentry *devptr, uint8 *invalue, uint32 pin_info)
{
        uint8 value = (*invalue > 0);

        //pin_info = bank_id(higher 16 bits)+pin_number(lower 16 bits)
        uint16 bank_id = pin_info>>16;
        uint16 pin_number = pin_info & 0xFFFF;

	if(bank_id > 3 || bank_id < 0 || pin_number > 31 || pin_number < 0 || value > 1 || value < 0)
	{
		return SYSERR;
	}
	uint32 *pin_pointer;

	if(bank_id == 0)
	{
		pin_pointer = (uint32 *)GPIO0_BASE;
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

	if(value == 1)
	{
		pin_pointer[GPIO_DATAOUT/4]  |= (1 << pin_number);
	}
	else if(value == 0)
	{
		pin_pointer[GPIO_DATAOUT/4]  &= (0 << pin_number);
	}

	return OK;

} 
