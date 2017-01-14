/* xsh_iotled.c - xsh_iotled */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gpio.h>
#include <pwm.h>

#define RED_SS 1
#define GREEN_SS 2
#define BLUE_SS 0

void init_rgb() {
    pwm_init(RED_SS);
    pwm_init(GREEN_SS);
    pwm_init(BLUE_SS);
}

void set_rgb(uint32 color) {
    uint8 r, g, b;
    r = (color >> 16);
    g = (color >> 8) & 0xFF;
    b = (color & 0xFF);
    // setpwm with these vals
    pwm_write(RED_SS, r);
    pwm_write(GREEN_SS, g);
    pwm_write(BLUE_SS, b);
    //digital_write(RED_BANK, RED_PIN, color > 0);
}

/*------------------------------------------------------------------------
 * xsh_obdemo - shell command that runs an onboard demonstration app (is
 *			usually run in background)
 *------------------------------------------------------------------------
 */
shellcmd xsh_obdemo(int nargs, char *args[])
{
    int32   retval;			/* return value from sys calls	*/
    uint32  color=0;
    uint8   esave=0;
    

	/* For argument '--help', emit a help message	*/

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Use: %s\n\n", args[0]);
		printf("Description:\n");
		printf("\tExecute an onboard demonstration application\n");
		printf("\t--help\t display this help and exit\n");
		return 0;
	}

    init_rgb();

    #include "obdemo.inc"

    return 0;
}
