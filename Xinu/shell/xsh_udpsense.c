/* xsh_udpsense.c - xsh_udpsense */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REFRESH_DELAY 2000

/*------------------------------------------------------------------------
 * xsh_udpsense - shell command that acts as a UDP sense daemon (is
 *			usually run in background)
 *------------------------------------------------------------------------
 */
shellcmd xsh_udpsense(int nargs, char *args[])
{
	int	retval;			/* return value			*/
	char	msg[] = "Xinu testing UDP echo"; /* message to send	*/
	int32	slot;			/* UDP slot to use		*/
	int32	msglen;			/* length of outgoing message	*/
	uint32	remoteip;		/* remote IP address to use	*/
	uint16	remport;		/* port number for UDP echo	*/
	uint16	locport	= 52743;	/* local port to use		*/
    char  adcchar[4];
    uint32 adcval;

	/* For argument '--help', emit a help message	*/

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Use: %s REMOTEIP REMOTEPORT\n\n", args[0]);
		printf("Description:\n");
		printf("\tSend sensor values to a remote UDP server\n");
		printf("\tUsually Run in Background\n");
		printf("Options:\n");
		printf("\tREMOTEIP:\tIP address in dotted decimal\n");
		printf("\tREMOTEPORT:\tRemote port\n");
		printf("\t--help\t display this help and exit\n");
		return 0;
	}

	/* Check for valid IP address argument */

	if (nargs != 3) {
		fprintf(stderr, "%s: invalid argument(s)\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
				args[0]);
		return 1;
	}

	if (dot2ip(args[1], &remoteip) == SYSERR) {
		fprintf(stderr, "%s: invalid IP address argument\r\n",
			args[0]);
		return 1;
	}

	if ((remport = atoi(args[2])) > 65535) {
		fprintf(stderr, "%s: invalid port argument\r\n",
			args[0]);
		return 1;
	}

	/* register local UDP port */
    printf("%s: registering local port %u remote port %u\n", args[0], locport, remport);

	slot = udp_register(remoteip, remport, locport);
	if (slot == SYSERR) {
		fprintf(stderr, "%s: could not reserve UDP port %d\n",
				args[0], locport);
		return 1;
	}

	/* Retry sending outgoing datagram and getting response */

    while (TRUE) {
        read(ADC, (char *)adcchar, 4);
        adcval = *(unsigned int*)adcchar;
        sprintf(msg, "%u", adcval);
        msglen = strnlen(msg, 1200);
        //printf("Sending %s\n", msg);
        retval = udp_send(slot, msg, msglen);
        if (retval == SYSERR) {
            fprintf(stderr, "%s: error sending UDP \n",
                args[0]);
            return 1;
        }
        sleepms(REFRESH_DELAY);
    }

	udp_release(slot);
	if (retval == TIMEOUT) {
		fprintf(stderr, "%s: retry limit exceeded\n",
			args[0]);
		return 1;
	}

	return 0;
}
