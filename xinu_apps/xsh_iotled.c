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
#define PIR_BANK 1
#define PIR_PIN 12

void initrgb() {
    // set_pin(RED_SS, RED_PIN, 'o');
    // set_pin(GREEN_SS, GREEN_PIN, 'o');
    // set_pin(BLUE_SS, BLUE_PIN, 'o');
    pwm_init(RED_SS);
    pwm_init(GREEN_SS);
    pwm_init(BLUE_SS);
}

void setrgb(uint32 color) {
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
 * xsh_iotled - shell command that acts as a UDP command socket (is
 *			usually run in background)
 *------------------------------------------------------------------------
 */
shellcmd xsh_iotled(int nargs, char *args[])
{
	int32	retval;			/* return value from sys calls	*/
	uint32	remip;			/* remote sender's IP address	*/
	uint16	remport;		/* remote sender's UDP port	*/
	char	buff[1500];		/* buffer for incoming reply	*/
	int32	slot;			/* slot in UDP table 		*/
	uint16	locport;	    /* port number for UDP trigger	*/
    uint32  msgType, msgVal;
    uint32  color=0;
    uint8   esave=0;
    uint8   pirval;

	/* For argument '--help', emit a help message	*/

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Use: %s\n\n", args[0]);
		printf("Description:\n");
		printf("\tBecome a UDP trigger server\n");
		printf("Options:\n");
		printf("\tLOCALPORT:\tLocal port\n");
		printf("\t--help\t display this help and exit\n");
		return 0;
	}

	/* Check for valid IP address argument */

	if (nargs != 2) {
		fprintf(stderr, "%s: invalid argument(s)\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
				args[0]);
		return 1;
	}

	if ((locport = atoi(args[1])) > 65535) {
		fprintf(stderr, "%s: invalid port argument\r\n",
			args[0]);
		return 1;
	}

	/* register local UDP port */
    printf("%s: registering local port %u \n", args[0], locport);

	slot = udp_register(0, 0, locport);
	if (slot == SYSERR) {
		fprintf(stderr, "%s: could not reserve UDP port %d\n",
				args[0], locport);
		return 1;
	}

	/* Do forever: read an incoming datagram and send it back */
    // TODO 
    //set_pin(PIR_BANK, PIR_PIN, 'i');
    control(GPIO, PIR_BANK, PIR_PIN, 1);
    initrgb();

	while (TRUE) {
		retval = udp_recvaddr(slot, &remip, &remport, buff,
						sizeof(buff), 6000);

		if (retval == TIMEOUT) {
            if(esave) {
                read(GPIO, &pirval, (PIR_BANK << 16) | PIR_PIN);
                if(pirval == 0) {
                    printf("nobody in room, switching off led\n");
                    setrgb(0);
                } else {
                    printf("somebody in room, switching on led\n");
                    setrgb(color);
                }
            }
			continue;
		} else if (retval == SYSERR) {
			fprintf(stderr, "%s: error receiving UDP\n",
				args[0]);
			return 1;
		}
        buff[retval] = '\0';
        printf("Received: %s\n", buff);
        int sp = 0;
        while(buff[sp] != ' ') sp++;
        buff[sp] = '\0';
        //sscanf(buff, "%u %u", 2, &msgType, &msgVal);
        msgType = atoi(buff);
        msgVal = atoi(&buff[sp+1]);
        if(msgType == 1) {
            color = msgVal;
            if(!esave)
                setrgb(color);
        } else if (msgType == 0) {
            esave = msgVal;
        }
        printf("color : %u\nesave: %d\n", color, esave);
        // update led with color 1-13
        // write(RGB, color);
        // if esave == 1, do a esaveupdate - pir on 1-12
	}
	return 0;
}
