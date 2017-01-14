
        uint8   pirval;
        control(GPIO, 1, 12, 1);
	while (TRUE) {
            read(GPIO, &pirval, (1 << 16) | 12);
            if(pirval == 0) {
                set_rgb(16711680);
            } else {
                set_rgb(16449280);
              }
            sleep(5);
	}
       