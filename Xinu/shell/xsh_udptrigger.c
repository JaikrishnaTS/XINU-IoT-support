/* xsh_udptrigger.c - xsh_udptrigger */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*------------------------------------------------------------------------
 * xsh_udptrigger - shell command that acts as a UDP trigger server (is
 *			usually run in background)
 *------------------------------------------------------------------------
 */
shellcmd xsh_udptrigger(int nargs, char *args[])
{
	int32	retval;			/* return value from sys calls	*/
	uint32	remip;			/* remote sender's IP address	*/
	uint16	remport;		/* remote sender's UDP port	*/
	char	buff[1500];		/* buffer for incoming reply	*/
	int32	slot;			/* slot in UDP table 		*/
	uint16	locport= 7;	/* port number for UDP trigger	*/

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

	while (TRUE) {
		retval = udp_recvaddr(slot, &remip, &remport, buff,
						sizeof(buff), 600000);

		if (retval == TIMEOUT) {
			continue;
		} else if (retval == SYSERR) {
			fprintf(stderr, "%s: error receiving UDP\n",
				args[0]);
			return 1;
		}
        buff[retval] = '\0';

        printf("Received: %s\n", buff);
	}
	return 0;
}
