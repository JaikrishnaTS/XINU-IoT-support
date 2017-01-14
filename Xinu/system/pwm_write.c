#include <xinu.h>
bool EHRPWMLoadCMPA(unsigned int baseAddr,
                    unsigned int CMPAVal,
                    bool enableShadowWrite,
                    unsigned int ShadowToActiveLoadTrigger,
                    bool OverwriteShadowFull);
void EHRPWMConfigureAQActionOnB(unsigned int baseAddr,
                                unsigned int zero,
                                unsigned int period,
                                unsigned int CAUp,
                                unsigned int CADown,
                                unsigned int CBUp,
                                unsigned int CBDown,
                                unsigned int SWForced);	


bool EHRPWMLoadCMPA(unsigned int baseAddr,
                    unsigned int CMPAVal,
                    bool enableShadowWrite,
                    unsigned int ShadowToActiveLoadTrigger,
                    bool OverwriteShadowFull)
{
    bool status = FALSE;

    if((OverwriteShadowFull) ||
        ((HWREGH(baseAddr+EHRPWM_CMPCTL) & EHRPWM_CMPCTL_SHDWAFULL) ==
                             EHRPWM_SHADOW_A_EMPTY))
    {
        HWREGH(baseAddr + EHRPWM_CMPCTL) = (HWREGH(baseAddr + EHRPWM_CMPCTL) &
            (~EHRPWM_CMPCTL_SHDWAMODE)) | ((enableShadowWrite <<
            EHRPWM_CMPCTL_SHDWAMODE_SHIFT) & EHRPWM_CMPCTL_SHDWAMODE);

        HWREGH(baseAddr + EHRPWM_CMPCTL) = (HWREGH(baseAddr + EHRPWM_CMPCTL) &
            (~EHRPWM_COMPA_LOAD_MASK)) |((ShadowToActiveLoadTrigger <<
            EHRPWM_CMPCTL_LOADAMODE_SHIFT) & EHRPWM_COMPA_LOAD_MASK);

        //printf("CMPA value %04x\n", CMPAVal);

        HWREGH(baseAddr + EHRPWM_CMPA) = CMPAVal & EHRPWM_CMPA_CMPA;

        status = TRUE;
    }

    return status;
}

void EHRPWMConfigureAQActionOnB(unsigned int baseAddr,
                                unsigned int zero,
                                unsigned int period,
                                unsigned int CAUp,
                                unsigned int CADown,
                                unsigned int CBUp,
                                unsigned int CBDown,
                                unsigned int SWForced)
{
    HWREGH(baseAddr + EHRPWM_AQCTLB) =
        ((CBDown << EHRPWM_AQCTLB_CBD_SHIFT) & EHRPWM_AQCTLB_CBD) |
        ((CBUp << EHRPWM_AQCTLB_CBU_SHIFT) & EHRPWM_AQCTLB_CBU) |
        ((CADown << EHRPWM_AQCTLB_CAD_SHIFT) & EHRPWM_AQCTLB_CAD) |
        ((CAUp << EHRPWM_AQCTLB_CAU_SHIFT) & EHRPWM_AQCTLB_CAU) |
        ((period << EHRPWM_AQCTLB_PRD_SHIFT) & EHRPWM_AQCTLB_PRD) |
        ((zero << EHRPWM_AQCTLB_ZRO_SHIFT) & EHRPWM_AQCTLB_ZRO);


    HWREGH(baseAddr + EHRPWM_AQSFRC) =
        (HWREGH(baseAddr + EHRPWM_AQSFRC) & (~EHRPWM_AQSFRC_ACTSFB)) |
     ((SWForced << EHRPWM_AQSFRC_ACTSFB_SHIFT) & EHRPWM_AQSFRC_ACTSFB);
}

syscall pwm_write(int ss, uint8 ip)
{
	intmask	mask;			/* Saved interrupt mask		*/

	mask = disable();

	uint32 epwm_reg,dutycycle;
	//dutycycle = (ip*128)/100;
    dutycycle = ip/2;
	if(ss == 0)
	{
		epwm_reg = SOC_EPWM_0_REGS;
	}
	else if(ss == 1)
	{
		epwm_reg = SOC_EPWM_1_REGS;		
	}
	else if(ss == 2)
	{
		epwm_reg = SOC_EPWM_2_REGS;
	}	

	EHRPWMLoadCMPA(epwm_reg,
                   dutycycle,
                   (bool)EHRPWM_SHADOW_WRITE_DISABLE,
                   (unsigned int)EHRPWM_COMPA_NO_LOAD,
                   (bool)EHRPWM_CMPCTL_OVERWR_SH_FL);


    EHRPWMConfigureAQActionOnB(epwm_reg,
                               EHRPWM_AQCTLB_ZRO_DONOTHING,
                               EHRPWM_AQCTLB_PRD_EPWMXBHIGH,
                               EHRPWM_AQCTLB_PRD_EPWMXBLOW,
                               EHRPWM_AQCTLB_CAD_DONOTHING,
                               EHRPWM_AQCTLB_CBU_DONOTHING,
                               EHRPWM_AQCTLB_CBD_DONOTHING,
                               EHRPWM_AQSFRC_ACTSFB_DONOTHING);
	
	restore(mask);		/* Restore interrupts */
    return OK;
}

