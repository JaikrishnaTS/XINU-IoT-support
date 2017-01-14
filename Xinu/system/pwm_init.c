#include <xinu.h>

void PWMSSTBClkEnable(unsigned int);
unsigned int EPWM2PinMuxSetup(void);
void EHRPWMClockEnable(unsigned int baseAdd);
void PWMSSModuleClkConfig(unsigned int instanceNum);
void EHRPWMTimebaseClkConfig(unsigned int baseAddr, unsigned int tbClk, unsigned int moduleClk);


void PWMSSTBClkEnable(unsigned int instance)
{
    switch(instance)
    {
    
         case 0:
               HWREG(SOC_CONTROL_REGS + CONTROL_PWMSS_CTRL) |=
                                             CONTROL_PWMSS_CTRL_PWMSS0_TBCLKEN;
               break;
 
         case 1:
               HWREG(SOC_CONTROL_REGS + CONTROL_PWMSS_CTRL) |=
                                             CONTROL_PWMSS_CTRL_PWMMS1_TBCLKEN;
               break;
  
         case 2:
               HWREG(SOC_CONTROL_REGS + CONTROL_PWMSS_CTRL) |=
                                             CONTROL_PWMSS_CTRL_PWMSS2_TBCLKEN;
               break;

         default:
         break;
    } 
}

unsigned int EPWM2PinMuxSetup(void)
{
    unsigned int profile = 0;
    unsigned int status = FALSE;

    profile = 0; 

    switch(profile)
    {
         case 4:
               HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_AD(9)) = CONTROL_CONF_MUXMODE(4);
         break;

         default:
         break;
    }
    return status;
}

void EHRPWMClockEnable(unsigned int baseAdd)
{
    HWREG(baseAdd + PWMSS_CLOCK_CONFIG) |= PWMSS_EHRPWM_CLK_EN_ACK;
}


void PWMSSModuleClkConfig(unsigned int instanceNum)
{
    HWREG(SOC_PRCM_REGS + CM_PER_L3S_CLKSTCTRL) |= 
                             CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L3S_CLKSTCTRL) & 
     CM_PER_L3S_CLKSTCTRL_CLKTRCTRL) != CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKSTCTRL) |= 
                             CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKSTCTRL) & 
     CM_PER_L3_CLKSTCTRL_CLKTRCTRL) != CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_PRCM_REGS + CM_PER_L3_INSTR_CLKCTRL) |= 
                             CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L3_INSTR_CLKCTRL) & 
                               CM_PER_L3_INSTR_CLKCTRL_MODULEMODE) != 
                                   CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE);

    HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKCTRL) |= 
                             CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKCTRL) & 
        CM_PER_L3_CLKCTRL_MODULEMODE) != CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE);

    HWREG(SOC_PRCM_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) |= 
                             CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_PRCM_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) & 
                              CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL) != 
                                CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKSTCTRL) |= 
                             CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKSTCTRL) & 
                             CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL) != 
                               CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKCTRL) |= 
                             CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKCTRL) & 
      CM_PER_L4LS_CLKCTRL_MODULEMODE) != CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE);

    if(0 == instanceNum)
    {
        HWREG(SOC_PRCM_REGS + CM_PER_EPWMSS0_CLKCTRL) |=
            CM_PER_EPWMSS0_CLKCTRL_MODULEMODE_ENABLE;

        while(CM_PER_EPWMSS0_CLKCTRL_MODULEMODE_ENABLE !=
              (HWREG(SOC_PRCM_REGS + CM_PER_EPWMSS0_CLKCTRL) &
               CM_PER_EPWMSS0_CLKCTRL_MODULEMODE));

        while((CM_PER_EPWMSS0_CLKCTRL_IDLEST_FUNC <<
               CM_PER_EPWMSS0_CLKCTRL_IDLEST_SHIFT) !=
              (HWREG(SOC_PRCM_REGS + CM_PER_EPWMSS0_CLKCTRL) &
               CM_PER_EPWMSS0_CLKCTRL_IDLEST));

    }
    else if(1 == instanceNum)
    {
        HWREG(SOC_PRCM_REGS + CM_PER_EPWMSS1_CLKCTRL) |=
            CM_PER_EPWMSS1_CLKCTRL_MODULEMODE_ENABLE;

        while(CM_PER_EPWMSS1_CLKCTRL_MODULEMODE_ENABLE !=
              (HWREG(SOC_PRCM_REGS + CM_PER_EPWMSS1_CLKCTRL) &
               CM_PER_EPWMSS1_CLKCTRL_MODULEMODE));

        while((CM_PER_EPWMSS1_CLKCTRL_IDLEST_FUNC <<
               CM_PER_EPWMSS1_CLKCTRL_IDLEST_SHIFT) !=
               (HWREG(SOC_PRCM_REGS + CM_PER_EPWMSS1_CLKCTRL) &
               CM_PER_EPWMSS1_CLKCTRL_IDLEST));

    }
    else if(2 == instanceNum)
    {
        HWREG(SOC_PRCM_REGS + CM_PER_EPWMSS2_CLKCTRL) |=
            CM_PER_EPWMSS2_CLKCTRL_MODULEMODE_ENABLE;

        while(CM_PER_EPWMSS2_CLKCTRL_MODULEMODE_ENABLE !=
              (HWREG(SOC_PRCM_REGS + CM_PER_EPWMSS2_CLKCTRL) &
               CM_PER_EPWMSS2_CLKCTRL_MODULEMODE));

        while((CM_PER_EPWMSS2_CLKCTRL_IDLEST_FUNC <<
               CM_PER_EPWMSS2_CLKCTRL_IDLEST_SHIFT) !=
               (HWREG(SOC_PRCM_REGS + CM_PER_EPWMSS2_CLKCTRL) &
                CM_PER_EPWMSS2_CLKCTRL_IDLEST));
    }
    else
    {

    }

    while(!(HWREG(SOC_PRCM_REGS + CM_PER_L3S_CLKSTCTRL) & 
            CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK));

    while(!(HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKSTCTRL) & 
            CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK));

    while(!(HWREG(SOC_PRCM_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) & 
           (CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK | 
            CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L4_GCLK)));

    while(!(HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKSTCTRL) & 
           (CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_L4LS_GCLK )));
    
}

void EHRPWMTimebaseClkConfig(unsigned int baseAddr,
                             unsigned int tbClk,
                             unsigned int moduleClk)

{
    unsigned int clkDiv = moduleClk/tbClk;
    unsigned int hspClkDiv;
    unsigned int lspClkDiv, lspClkDivSetting = 0;

    if(clkDiv > EHRPWM_TBCTL_HSPCLKDIV_14)
    {
        hspClkDiv = EHRPWM_TBCTL_HSPCLKDIV_DIVBY14; /* reg setting */
        lspClkDiv = clkDiv/EHRPWM_TBCTL_HSPCLKDIV_14; /* divider */
        /* reg setting */
        while(lspClkDiv > 1)
        {
           lspClkDiv = lspClkDiv >> 1;
           lspClkDivSetting++;
        }
    }
    else
    {
        hspClkDiv = clkDiv/2; /* reg setting */
        /* divide by 1 */
        lspClkDivSetting = EHRPWM_TBCTL_HSPCLKDIV_DIVBY1;
    }

    HWREGH(baseAddr + EHRPWM_TBCTL) = (HWREGH(baseAddr + EHRPWM_TBCTL) &
            (~EHRPWM_TBCTL_CLKDIV)) | ((lspClkDivSetting <<
            EHRPWM_TBCTL_CLKDIV_SHIFT) & EHRPWM_TBCTL_CLKDIV);

    HWREGH(baseAddr + EHRPWM_TBCTL) = (HWREGH(baseAddr + EHRPWM_TBCTL) &
            (~EHRPWM_TBCTL_HSPCLKDIV)) | ((hspClkDiv <<
            EHRPWM_TBCTL_HSPCLKDIV_SHIFT) & EHRPWM_TBCTL_HSPCLKDIV);
}

void EHRPWMPWMOpFreqSet(unsigned int baseAddr,
                        unsigned int tbClk,
                        unsigned int pwmFreq,
                        unsigned int counterDir,
                        bool enableShadowWrite)
{
     unsigned int tbPeriodCount = tbClk/pwmFreq;

     HWREGH(baseAddr + EHRPWM_TBCTL) = (HWREGH(baseAddr + EHRPWM_TBCTL) &
             (~EHRPWM_PRD_LOAD_SHADOW_MASK)) | ((enableShadowWrite <<
            EHRPWM_TBCTL_PRDLD_SHIFT) & EHRPWM_PRD_LOAD_SHADOW_MASK);

     HWREGH(baseAddr + EHRPWM_TBCTL) = (HWREGH(baseAddr + EHRPWM_TBCTL) &
             (~EHRPWM_COUNTER_MODE_MASK)) | ((counterDir <<
            EHRPWM_TBCTL_CTRMODE_SHIFT) &  EHRPWM_COUNTER_MODE_MASK);

     if(EHRPWM_COUNT_UP_DOWN == counterDir)
     {
         HWREGH(baseAddr + EHRPWM_TBPRD) = (unsigned short)tbPeriodCount/2;
     }
     else
     {
         HWREGH(baseAddr + EHRPWM_TBPRD) = (unsigned short)tbPeriodCount;
     }
     //printf("periodcount is %04x\n", tbPeriodCount);

}

syscall pwm_init(int ss)
{	
	intmask	mask;			/* Saved interrupt mask		*/

	mask = disable();

	uint32 pin_data,pwm_reg_ss,epwm_reg;
	uint32 *pwm_pointer;
	if(ss == 0)
	{
		pin_data = 0x0013;
		pwm_pointer = (uint32 *)P9_21;
		pwm_reg_ss = SOC_PWMSS0_REGS;
		epwm_reg = SOC_EPWM_0_REGS;
	}
	else if(ss == 1)
	{
		pin_data = 0x0016;
		pwm_pointer = (uint32 *)P9_16;
		pwm_reg_ss = SOC_PWMSS1_REGS;
		epwm_reg = SOC_EPWM_1_REGS;		
	}
	else if(ss == 2)
	{
		pin_data = 0x0014;
		pwm_pointer = (uint32 *)P8_13;
		pwm_reg_ss = SOC_PWMSS2_REGS;
		epwm_reg = SOC_EPWM_2_REGS;
	}	

    *pwm_pointer = pin_data;
    //printf("pointer ADDR %04x\n", pwm_pointer);
    //printf("pointer VALUE %04x\n", *pwm_pointer);

    PWMSSModuleClkConfig(ss);

    EPWM2PinMuxSetup();

    /* Enable Clock for EHRPWM in PWM sub system */
    EHRPWMClockEnable(pwm_reg_ss);

    /* Enable Timer Base Module Clock in control module */
    PWMSSTBClkEnable(ss);

    EHRPWMTimebaseClkConfig(epwm_reg, SOC_EHRPWM_2_MODULE_FREQ/CLOCK_DIV_VAL, SOC_EHRPWM_2_MODULE_FREQ);

    /* EHRPWM is configured to generate PWM waveform on EPWMBxB Pin*/
     
    EHRPWMPWMOpFreqSet(epwm_reg,
                       SOC_EHRPWM_2_MODULE_FREQ/CLOCK_DIV_VAL,
                       (unsigned int)(SOC_EHRPWM_2_MODULE_FREQ/CLOCK_DIV_VAL)/0x7F,
                       (unsigned int)EHRPWM_COUNT_UP, (bool)EHRPWM_SHADOW_WRITE_DISABLE);
    restore(mask);
    return OK;
}
