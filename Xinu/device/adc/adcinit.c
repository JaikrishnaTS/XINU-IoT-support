#include <xinu.h>

sid32 readSem;			// Read semaphore

devcall	adcinit (struct	dentry *devptr)
{
	struct adc_csreg* csreg = (struct adc_csreg*)devptr->dvcsr; // Pointer to Control Status Register of ADC
	uint32 stepSelect = 0;

	// Semaphore to monitor register that stores ADC value on interrupt and the function that reads the register
	readSem = semcreate(0);										
	
	// Initializing ADC Clock
	HWREG(CM_WKUP_CONTROL_CLKCTRL) |= 0x02;                             //enbale wkup control clk
	while(( HWREG(CM_WKUP_CONTROL_CLKCTRL) & 0x03 ) != 0x02 );          //wait 

	HWREG(CM_WKUP_CLKSTCTRL) |= 0x02;                                   //enbale wkup st clk
	while(( HWREG(CM_WKUP_CLKSTCTRL) & 0x03 ) != 0x02 );                //wait 

	HWREG(CM_L3_AON_CLKSTCTRL) |= 0x02;                                 //enbale wkup st clk
	while(( HWREG(CM_L3_AON_CLKSTCTRL) & 0x03 ) != 0x02 );              //wait

	HWREG(CM_WKUP_ADC_TSC_CLKCTRL) |= 0x02;                             //enbale wkup st clk
	while(( HWREG(CM_WKUP_ADC_TSC_CLKCTRL) & 0x03 ) != 0x02 );          //wait

	while(( HWREG(CM_WKUP_CONTROL_CLKCTRL) & (0x03<<16) ) != 0x00 );            //wait wkup control clk fully functional
	while(( HWREG(CM_L3_AON_CLKSTCTRL) & (0x01<<3) ) == 0x00 );                 //wait L3 AON gclk active
	while(( HWREG(CM_WKUP_L4WKUP_CLKCTRL) & (0x03<<16) ) != 0x00 );             //wait L4 WKUP CLK fully functional
	while(( HWREG(CM_WKUP_CLKSTCTRL) & (0x01<<2) ) == 0x00 );                   //wait L4 WKUP GCLK active
	while(( HWREG(CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL) & (0x01<<2) ) == 0x00 );    //wait L4 WKUP aon CLK active
	while(( HWREG(CM_WKUP_CLKSTCTRL) & (0x01<<14) ) == 0x00 );                  //wait ADC fclk active
	while(( HWREG(CM_WKUP_ADC_TSC_CLKCTRL) & (0x03<<16) ) != 0x00 );            //wait ADC module fully functinoal

	// Setting ADC Interrupt	
	struct	intc_csreg *csrptr = (struct intc_csreg *)INTERRUPT_CONTROLLER;   // Setting to Interrupt Control Status Register
	set_evec(devptr->dvirq, (uint32)devptr->dvintr);                // set handler for IRQ 40
	
        //average samples
        csreg->step[stepSelect].stepConfig |= 0x04<<2;
	
	//Clear all the interrupts
	//csreg->irqStatus |= 0x7FFF;

	//Enable ADC IRQ
	csreg->irqEnableSet |=ADC_INTR_EN;

	csreg->ctrl |= 0x01;

	return OK;
}

