#ifndef ADC_H
#define ADC_H

struct adc_csreg
{
	volatile uint32 revison;
	volatile uint32 reserve1[3];
	volatile uint32 sysconfig;
	volatile uint32 reserve2[4];
	volatile uint32 irqStatusRaw;
	volatile uint32 irqStatus;
	volatile uint32 irqEnableSet;	
	volatile uint32 irqEnableClr;
	volatile uint32 irqWakeup;
	volatile uint32 dmaEnableSet;
	volatile uint32 dmaEnableClr;
	volatile uint32 ctrl;
	volatile uint32 adcStat;
	volatile uint32 adcRange;
	volatile uint32 adcClkDiv;
	volatile uint32 adcMisc;
	volatile uint32 stepEnable;
	volatile uint32 idleConfig;
	volatile uint32 tsChargeStepconfig;
	volatile uint32 tsChargeDelay;
	struct 
	{
		volatile uint32 stepConfig;
		volatile uint32 stepDelay;
	}step[16];
	struct 
	{
		volatile uint32 fifoCount;
		volatile uint32 fifoThreshold;
		volatile uint32 dmaReq;
	}fifoInfo[2];
	volatile uint32 reserve3;
	volatile uint32 fifoData0;
	volatile uint32 reserve4[63];
	volatile uint32 fifoData1;
};

devcall	adcread(struct dentry* devptr, char* adc_data, int32 count);
devcall	adcinit (struct	dentry *devptr);
void adchandler(uint32 xnum);

//CLOCK domain register (part)
#define CM_PER                                (0x44E00000)              
#define CM_PER_L3S_CLKSTCTRL                  (0x44E00004)
#define CM_PER_L3_CLKSTCTRL                   (0x44E0000C)
#define CM_PER_L3_INSTR_CLKCTRL               (0x44E000DC)
#define CM_PER_L3_CLKCTRL                     (0x44E000E0)
#define CM_PER_OCPWP_L3_CLKSTCTRL             (0x44E0012C)

#define CM_WKUP                               (0x44E00400)              
#define CM_WKUP_CLKSTCTRL                     (0x44E00400)
#define CM_WKUP_CONTROL_CLKCTRL               (0x44E00404)
#define CM_WKUP_L4WKUP_CLKCTRL                (0x44E0040C)
#define CM_L3_AON_CLKSTCTRL                   (0x44E00418)
#define CM_WKUP_ADC_TSC_CLKCTRL               (0x44E004BC)
#define CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL      (0x44E004CC)

//Interrupt controller register (part)
#define INTERRUPT_CONTROLLER                  (0x48200000)

//MACRO to read and write register
#define HWREG(n)        (*((volatile unsigned int *)(n)))

//ADC channel
#define ADC_CHANNEL1            	(0)		//PIN AN0
#define ADC_CHANNEL2            	(1) 		//PIN AN1
#define ADC_CHANNEL3            	(2)   		//PIN AN2
#define ADC_CHANNEL4            	(3)   		//PIN AN3
#define ADC_CHANNEL5            	(4)   		//PIN AN4
#define ADC_CHANNEL6            	(5) 		//PIN AN5
#define ADC_CHANNEL7            	(6) 		//PIN AN6
#define ADC_CHANNEL8            	(7)		//PIN AN7

//ADC interrupt bit set
#define ADC_INTR_EN				(0x01<<1)	

#endif
