#include <xinu.h>

devcall digital_read(struct dentry* devptr, char* buff, uint32 pin_info)
{
        //pin_info = bank_id(higher 16 bits)+pin_number(lower 16 bits)

        uint16 bank_id = pin_info>>16;
        uint16 pin_number = pin_info & 0xFFFF;

	if(bank_id > 3 || bank_id < 0 || pin_number > 31 || pin_number < 0)
	{
		return SYSERR;
	}
	static uint32 l;
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
		pin_pointer =(uint32 *) GPIO2_BASE;
	}
	else if(bank_id == 3)
	{
		pin_pointer =(uint32 *) GPIO3_BASE;
	}	
	*buff = ((pin_pointer[GPIO_DATAIN/4] & (1 << pin_number)) >> pin_number);

	return 1;
	
} 
