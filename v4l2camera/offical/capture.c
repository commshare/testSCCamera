/*
 *  V4L2 video capture example
 *
 *  This program can be used and distributed without restrictions.


 root@Ubuntu32:/home/zhangbin/streaming/live/v4l2/loalPC# man getopt_long
 root@Ubuntu32:/home/zhangbin/streaming/live/v4l2/loalPC# ./capture -h
###c= 104
 Usage: ./capture [options]

 Options:
 -d | --device name   Video device name [/dev/video]
 -h | --help          Print this message
 -m | --mmap          Use memory mapped buffers
 -r | --read          Use read() calls
 -u | --userp         Use application allocated buffers
 root@Ubuntu32:/home/zhangbin/streaming/live/v4l2/loalPC#

./capture -d /dev/video0 -m
 */

#include"capture.h"

#define CLEAR(x) memset (&(x), 0, sizeof (x))

typedef enum {
	IO_METHOD_READ,
	IO_METHOD_MMAP,
	IO_METHOD_USERPTR,
} io_method;

struct buffer {
        void *                  start;
        size_t                  length;
};

static char *           dev_name        = NULL;
static io_method	io		= IO_METHOD_MMAP;
static int              fd              = -1;
struct buffer *         buffers         = NULL;
static unsigned int     n_buffers       = 0;

 void
errno_exit                      (const char *           s)
{
        fprintf (stderr, "%s error %d, %s\n",
                 s, errno, strerror (errno));

        exit (EXIT_FAILURE);
}

static int
xioctl                          (int                    fd,
                                 int                    request,
                                 void *                 arg)
{
        int r;

        do r = ioctl (fd, request, arg);
        while (-1 == r && EINTR == errno);

        return r;
}

static void
process_image                   (const void *           p)
{
    static int count=0;
    #if 0
        fputc ('.', stdout);
        fflush (stdout);
    #endif
    printf("###process one image [%d]\n",++count); //from 1 to ..
}

int
capture_read_frame			(void)
{
        struct v4l2_buffer buf;
	unsigned int i;

	switch (io) {
	case IO_METHOD_READ:
    		if (-1 == read (fd, buffers[0].start, buffers[0].length)) {
            		switch (errno) {
            		case EAGAIN:
                    		return 0;

			case EIO:
				/* Could ignore EIO, see spec. */

				/* fall through */

			default:
				errno_exit ("read");
			}
		}

    		process_image (buffers[0].start);

		break;

	case IO_METHOD_MMAP:
	//printf("###USE MMAP FOR CAPTURE ######\n"); //YES
		CLEAR (buf);

            	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            	buf.memory = V4L2_MEMORY_MMAP;

    		if (-1 == xioctl (fd, VIDIOC_DQBUF, &buf)) {
            		switch (errno) {
            		case EAGAIN:
                    		return 0;

			case EIO:
				/* Could ignore EIO, see spec. */

				/* fall through */

			default:
				errno_exit ("VIDIOC_DQBUF");
			}
		}

                assert (buf.index < n_buffers);

	        process_image (buffers[buf.index].start);

		if (-1 == xioctl (fd, VIDIOC_QBUF, &buf))
			errno_exit ("VIDIOC_QBUF");

		break;

	case IO_METHOD_USERPTR:
		CLEAR (buf);

    		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    		buf.memory = V4L2_MEMORY_USERPTR;

		if (-1 == xioctl (fd, VIDIOC_DQBUF, &buf)) {
			switch (errno) {
			case EAGAIN:
				return 0;

			case EIO:
				/* Could ignore EIO, see spec. */

				/* fall through */

			default:
				errno_exit ("VIDIOC_DQBUF");
			}
		}

		for (i = 0; i < n_buffers; ++i)
			if (buf.m.userptr == (unsigned long) buffers[i].start
			    && buf.length == buffers[i].length)
				break;

		assert (i < n_buffers);

    		process_image ((void *) buf.m.userptr);

		if (-1 == xioctl (fd, VIDIOC_QBUF, &buf))
			errno_exit ("VIDIOC_QBUF");

		break;
	}

	return 1;
}
#if 0
static void
mainloop                        (void)
{
	printf("#IN##Mainloop BEGIN ######\n");
	//ADD BY ME BEGIN
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

			if (read_frame ())
                    		break;

			/* EAGAIN - continue select loop. */
                }
        }
	printf("##IN#Mainloop END ######\n");
}
#endif
static void
stop_capturing                  (void)
{
        enum v4l2_buf_type type;

	switch (io) {
	case IO_METHOD_READ:
		/* Nothing to do. */
		break;

	case IO_METHOD_MMAP:
	case IO_METHOD_USERPTR:
		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

		if (-1 == xioctl (fd, VIDIOC_STREAMOFF, &type))
			errno_exit ("VIDIOC_STREAMOFF");

		break;
	}
}

static void
start_capturing                 (void)
{
        unsigned int i;
        enum v4l2_buf_type type;

	switch (io) {
	case IO_METHOD_READ:
		/* Nothing to do. */
		break;

	case IO_METHOD_MMAP:
        printf("#### n_buffers is [%d] \n",n_buffers);
		for (i = 0; i < n_buffers; ++i) {
            		struct v4l2_buffer buf;

        		CLEAR (buf);

        		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        		buf.memory      = V4L2_MEMORY_MMAP;
        		buf.index       = i;

        		if (-1 == xioctl (fd, VIDIOC_QBUF, &buf))
                    		errno_exit ("VIDIOC_QBUF");
                printf("####video io QBUF IS OK \n");
		}

		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

		if (-1 == xioctl (fd, VIDIOC_STREAMON, &type))
			errno_exit ("VIDIOC_STREAMON");
        printf("####video camera Stream ON OK ! \n");
		break;

	case IO_METHOD_USERPTR:
		for (i = 0; i < n_buffers; ++i) {
            		struct v4l2_buffer buf;

        		CLEAR (buf);

        		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        		buf.memory      = V4L2_MEMORY_USERPTR;
			buf.index       = i;
			buf.m.userptr	= (unsigned long) buffers[i].start;
			buf.length      = buffers[i].length;

			if (-1 == xioctl (fd, VIDIOC_QBUF, &buf))
                    		errno_exit ("VIDIOC_QBUF");
		}

		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

		if (-1 == xioctl (fd, VIDIOC_STREAMON, &type))
			errno_exit ("VIDIOC_STREAMON");

		break;
	}
}

static void
uninit_device                   (void)
{
        unsigned int i;

	switch (io) {
	case IO_METHOD_READ:
		free (buffers[0].start);
		break;

	case IO_METHOD_MMAP:
		for (i = 0; i < n_buffers; ++i)
			if (-1 == munmap (buffers[i].start, buffers[i].length))
				errno_exit ("munmap");
		break;

	case IO_METHOD_USERPTR:
		for (i = 0; i < n_buffers; ++i)
			free (buffers[i].start);
		break;
	}

	free (buffers);
}

static void
init_read			(unsigned int		buffer_size)
{
        buffers = calloc (1, sizeof (*buffers));

        if (!buffers) {
                fprintf (stderr, "Out of memory\n");
                exit (EXIT_FAILURE);
        }

	buffers[0].length = buffer_size;
	buffers[0].start = malloc (buffer_size);

	if (!buffers[0].start) {
    		fprintf (stderr, "Out of memory\n");
            	exit (EXIT_FAILURE);
	}
}

static void
init_mmap			(void)
{
	struct v4l2_requestbuffers req;

        CLEAR (req);

        req.count               = 4;
        req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory              = V4L2_MEMORY_MMAP;

	if (-1 == xioctl (fd, VIDIOC_REQBUFS, &req)) {
                if (EINVAL == errno) {
                        fprintf (stderr, "%s does not support "
                                 "memory mapping\n", dev_name);
                        exit (EXIT_FAILURE);
                } else {
                        errno_exit ("VIDIOC_REQBUFS");
                }
        }

        if (req.count < 2) {
                fprintf (stderr, "Insufficient buffer memory on %s\n",
                         dev_name);
                exit (EXIT_FAILURE);
        }

        buffers = calloc (req.count, sizeof (*buffers));

        if (!buffers) {
                fprintf (stderr, "Out of memory\n");
                exit (EXIT_FAILURE);
        }

        for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
                struct v4l2_buffer buf;

                CLEAR (buf);

                buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory      = V4L2_MEMORY_MMAP;
                buf.index       = n_buffers;

                if (-1 == xioctl (fd, VIDIOC_QUERYBUF, &buf))
                        errno_exit ("VIDIOC_QUERYBUF");

                buffers[n_buffers].length = buf.length;
                buffers[n_buffers].start =
                        mmap (NULL /* start anywhere */,
                              buf.length,
                              PROT_READ | PROT_WRITE /* required */,
                              MAP_SHARED /* recommended */,
                              fd, buf.m.offset);

                if (MAP_FAILED == buffers[n_buffers].start)
                        errno_exit ("mmap");
        }
}

static void
init_userp			(unsigned int		buffer_size)
{
	struct v4l2_requestbuffers req;
        unsigned int page_size;

        page_size = getpagesize ();
        buffer_size = (buffer_size + page_size - 1) & ~(page_size - 1);

        CLEAR (req);

        req.count               = 4;
        req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory              = V4L2_MEMORY_USERPTR;

        if (-1 == xioctl (fd, VIDIOC_REQBUFS, &req)) {
                if (EINVAL == errno) {
                        fprintf (stderr, "%s does not support "
                                 "user pointer i/o\n", dev_name);
                        exit (EXIT_FAILURE);
                } else {
                        errno_exit ("VIDIOC_REQBUFS");
                }
        }

        buffers = calloc (4, sizeof (*buffers));

        if (!buffers) {
                fprintf (stderr, "Out of memory\n");
                exit (EXIT_FAILURE);
        }

        for (n_buffers = 0; n_buffers < 4; ++n_buffers) {
                buffers[n_buffers].length = buffer_size;
                buffers[n_buffers].start = memalign (/* boundary */ page_size,
                                                     buffer_size);

                if (!buffers[n_buffers].start) {
    			fprintf (stderr, "Out of memory\n");
            		exit (EXIT_FAILURE);
		}
        }
}

static void
init_device                     (void)
{
        struct v4l2_capability cap;
        struct v4l2_cropcap cropcap;
        struct v4l2_crop crop;
        struct v4l2_format fmt;
	unsigned int min;

        if (-1 == xioctl (fd, VIDIOC_QUERYCAP, &cap)) {
                if (EINVAL == errno) {
                        fprintf (stderr, "%s is no V4L2 device\n",
                                 dev_name);
                        exit (EXIT_FAILURE);
                } else {
                        errno_exit ("VIDIOC_QUERYCAP");
                }
        }

        if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
                fprintf (stderr, "%s is no video capture device\n",
                         dev_name);
                exit (EXIT_FAILURE);
        }

	switch (io) {
	case IO_METHOD_READ:
		if (!(cap.capabilities & V4L2_CAP_READWRITE)) {
			fprintf (stderr, "%s does not support read i/o\n",
				 dev_name);
			exit (EXIT_FAILURE);
		}

		break;

	case IO_METHOD_MMAP:
	case IO_METHOD_USERPTR:
		if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
			fprintf (stderr, "%s does not support streaming i/o\n",
				 dev_name);
			exit (EXIT_FAILURE);
		}

		break;
	}


        /* Select video input, video standard and tune here. */


	CLEAR (cropcap);

        cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        if (0 == xioctl (fd, VIDIOC_CROPCAP, &cropcap)) {
                crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                crop.c = cropcap.defrect; /* reset to default */

                if (-1 == xioctl (fd, VIDIOC_S_CROP, &crop)) {
                        switch (errno) {
                        case EINVAL:
                                /* Cropping not supported. */
                                break;
                        default:
                                /* Errors ignored. */
                                break;
                        }
                }
        } else {
                /* Errors ignored. */
        }


        CLEAR (fmt);

        fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE; //buf类型
        fmt.fmt.pix.width       = CAPTURE_WIDTH; //specified by me
        fmt.fmt.pix.height      = CAPTURE_HEIGHT; //ditto
        fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV; //c270 support this
        fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;

        if (-1 == xioctl (fd, VIDIOC_S_FMT, &fmt))
                errno_exit ("VIDIOC_S_FMT");

        /* Note VIDIOC_S_FMT may change width and height. */

	/* Buggy driver paranoia. */
	min = fmt.fmt.pix.width * 2;
	if (fmt.fmt.pix.bytesperline < min)
		fmt.fmt.pix.bytesperline = min;
	min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
	if (fmt.fmt.pix.sizeimage < min)
		fmt.fmt.pix.sizeimage = min;

	switch (io) {
	case IO_METHOD_READ:
		init_read (fmt.fmt.pix.sizeimage);
		break;

	case IO_METHOD_MMAP:
		init_mmap ();
		break;

	case IO_METHOD_USERPTR:
		init_userp (fmt.fmt.pix.sizeimage);
		break;
	}

    printf("###Init Device OK \n");
}

static void
close_device                    (void)
{
        if (-1 == close (fd))
	        errno_exit ("close");

        fd = -1;
}

//pass dev_name to open
 int
open_device                     (void)
{
	   // char *dev_name=(char *)device_name;
        struct stat st;

        if (-1 == stat (dev_name, &st)) {
                fprintf (stderr, "Cannot identify '%s': %d, %s\n",
                         dev_name, errno, strerror (errno));
                exit (EXIT_FAILURE);
        }

        if (!S_ISCHR (st.st_mode)) {
                fprintf (stderr, "%s is no device\n", dev_name);
                exit (EXIT_FAILURE);
        }

        fd = open (dev_name, O_RDWR /* required */ | O_NONBLOCK, 0);

        if (-1 == fd) {
                fprintf (stderr, "Cannot open '%s': %d, %s\n",
                         dev_name, errno, strerror (errno));
				//exit (EXIT_FAILURE);//removed by me

        }
		return fd; //add by me
        printf("##open device[%d] camera [%s] OK !\n",fd,DEV_NAME);
}

static void
usage                           (FILE *                 fp,
                                 int                    argc,
                                 char **                argv)
{
        fprintf (fp,
                 "Usage: %s [options]\n\n"
                 "Options:\n"
                 "-d | --device name   Video device name [/dev/video]\n"
                 "-h | --help          Print this message\n"
                 "-m | --mmap          Use memory mapped buffers\n"
                 "-r | --read          Use read() calls\n"
                 "-u | --userp         Use application allocated buffers\n"
                 "",
		 argv[0]);
}

static const char short_options [] = "d:hmru";

static const struct option
long_options [] = {
        { "device",     required_argument,      NULL,           'd' },
        { "help",       no_argument,            NULL,           'h' },
        { "mmap",       no_argument,            NULL,           'm' },
        { "read",       no_argument,            NULL,           'r' },
        { "userp",      no_argument,            NULL,           'u' },
        { 0, 0, 0, 0 }
};

/*
        dev_name = DEV_NAME;
        //io = IO_METHOD_MMAP; //ADD BY ME
        open_device ();
        init_device ();
        start_capturing ();
		//////////PROCESS//BEGIN/////////////////
        mainloop ();
		//////////PROCESS END//////////////////////////
        stop_capturing ();
        uninit_device ();
        close_device ();
        exit (EXIT_SUCCESS);
        return 0;
*/

int  capture_open(void )
{
	    dev_name = DEV_NAME;
	    int fd=open_device ();
		if(fd==-1)
			goto FAIL;
        init_device ();
	FAIL:
		return fd;
}
void capture_start(void)
{
	   start_capturing ();
}
void capture_stop(void)
{
	 stop_capturing ();
	 uninit_device ();
     close_device ();
}
