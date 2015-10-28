#include"capture.h"

void mainloop(int fd)
{
	unsigned int count;
        count = 100;
        while (count-- > 0) {
                for (;;) {
                        fd_set fds;
                        struct timeval tv;
                        int r;

                        FD_ZERO (&fds);
                        FD_SET (fd, &fds);

                        /* Timeout. */ //2秒的超时等待
                        tv.tv_sec = 2;
                        tv.tv_usec = 0;
						//等待摄像头设备可读
                        r = select (fd + 1, &fds, NULL, NULL, &tv);

                        if (-1 == r) {
                                if (EINTR == errno)
                                 {
								 	printf("####  select CONTINUE \n");
								 	continue;
								 }
                                errno_exit ("select");
                        }

                        if (0 == r) {
                                fprintf (stderr, "select timeout, EXIT \n");
                                exit (EXIT_FAILURE);
                        }

			if (capture_read_frame ())
                    		break;

			/* EAGAIN - continue select loop. */
                }
        }
	//printf("##IN#Mainloop END ######\n");
}

int main()
{

	 int fd=capture_open();
	if(fd==-1)
	 {
	 	printf("###open fail \n");
		exit (EXIT_FAILURE);
	 }
capture_start();
   mainloop(fd);
 capture_stop();

	//printf("#IN##Mainloop BEGIN ######\n");
	//ADD BY ME BEGIN



}
