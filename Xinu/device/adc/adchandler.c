#include <xinu.h>

extern sid32 readSem;

void adchandler(uint32 xnum)
{
	struct	dentry	*devptr;            // Pointer to ADC device entry	
	struct	adc_csreg *csrptr;          // Poniter to Control Status Register for ADC 
	volatile uint32	irqStatus = 0;      // Interrupt Status
	
	// Point to ADC entry in device table
	devptr = (struct dentry *) &devtab[ADC];
	csrptr = (struct adc_csreg *) devptr->dvcsr; // Point to Control Status Register 
	irqStatus = csrptr->irqStatus;      // Update interrupt status
	csrptr->irqStatus = irqStatus;      // Restrore interrupt status

	//csrptr->stepEnable &= ~(0x01<<0);
	semcount(readSem);                  // Add semaphore count
	signal(readSem);                    // signal read semaphore	
}
