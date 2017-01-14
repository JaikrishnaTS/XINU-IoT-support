#include <xinu.h>

extern sid32 readSem;

devcall	adcread(struct dentry* devptr,char* adc_data, int32 count)
{
	struct adc_csreg* csreg = (struct adc_csreg*)devptr->dvcsr; // Pointer to Control Status Register 

	csreg->stepEnable |= (0x01<<1);

	//wait for semaphore signaled by ADC interrupt handler
	wait(readSem);

	unsigned int data = 0;
	int i;
	data = csreg->fifoData0 &(0xFFF) ;

	for(i=0;i < count; i++)
	{
		adc_data[i] = (data&(0x000000ff << (i*8))) >> (i*8); 
	}
	
	return OK;
}
