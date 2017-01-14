devcall set_pin(struct dentry *, uint32, uint32, uint32);
devcall digital_read(struct dentry *, char*, uint32);
devcall digital_write(struct dentry *, uint8 *, uint32);

#define MMAP_OFFSET (0x44C00000)
#define MMAP_SIZE   (0x481AEFFF-MMAP_OFFSET)

#define GPIO0_BASE 0x44E07000
#define GPIO1_BASE 0x4804C000
#define GPIO2_BASE 0x481AC000
#define GPIO3_BASE 0x481AE000

#define OE_ADDR 0x134			// Defines the behaviour of the pin (Input/Output)
#define GPIO_DATAOUT 0x13C		// If pin defined as output : sets the pin as 1 or 0
#define GPIO_DATAIN 0x138		// If pin defined as input : reads high or low
#define GPIO_CLEARDATAOUT 0x190 // Clears the corresponding pin if 1 is written to it
#define GPIO_SETDATAOUT 0x194	// Sets the corresponding pin if 1 is written to it

// ADC control registers // 

#define ADC_TSC (0x44E0D000)

#define ADC_CTRL (ADC_TSC+0x40)
#define ADC_STEPCONFIG_WRITE_PROTECT_OFF (0x01<<2)
#define ADC_STEPENABLE (ADC_TSC+0x54)

#define ADCSTEPCONFIG1 (ADC_TSC+0x64)
#define ADCSTEPDELAY1  (ADC_TSC+0x68)
#define ADCSTEPCONFIG2 (ADC_TSC+0x6C)
#define ADCSTEPDELAY2  (ADC_TSC+0x70)
#define ADCSTEPCONFIG3 (ADC_TSC+0x74)
#define ADCSTEPDELAY3  (ADC_TSC+0x78)
#define ADCSTEPCONFIG4 (ADC_TSC+0x7C)
#define ADCSTEPDELAY4  (ADC_TSC+0x80)
#define ADCSTEPCONFIG5 (ADC_TSC+0x84)
#define ADCSTEPDELAY5  (ADC_TSC+0x88)
#define ADCSTEPCONFIG6 (ADC_TSC+0x8C)
#define ADCSTEPDELAY6  (ADC_TSC+0x90)
#define ADCSTEPCONFIG7 (ADC_TSC+0x94)
#define ADCSTEPDELAY7  (ADC_TSC+0x98)
#define ADCSTEPCONFIG8 (ADC_TSC+0x9C)
#define ADCSTEPDELAY8  (ADC_TSC+0xA0)

#define ADC_AVG0  (0x000)
#define ADC_AVG2  (0x001)
#define ADC_AVG4  (0x010)
#define ADC_AVG8  (0x011)
#define ADC_AVG16 (0x100) 

#define ADC_FIFO0DATA (ADC_TSC+0x100)
#define ADC_FIFO_MASK (0xFFF)

// clock module registers // 

#define CM_WKUP (0x44E00400)
#define CM_WKUP_ADC_TSC_CLKCTRL (CM_WKUP+0xBC)
#define CM_WKUP_MODULEMODE_ENABLE (0x02)
#define CM_WKUP_IDLEST_DISABLED (0x03<<16)

#define CM_PER (0x44E00000)
#define CM_PER_EPWMSS1_CLKCTRL (CM_PER+0xCC)
#define CM_PER_EPWMSS0_CLKCTRL (CM_PER+0xD4)
#define CM_PER_EPWMSS2_CLKCTRL (CM_PER+0xD8)

