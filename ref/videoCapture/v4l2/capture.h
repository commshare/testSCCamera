#ifndef CAPTURE_H
#define CAPTURE_H

/* c270 support format :
capture_open: using MEMORY_MMAP mode, buf cnt=2
	 support YUV 4:2:2 (YUYV)
	 support MJPEG

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>          /* for videodev2.h */

#include <linux/videodev2.h>

#define DEV_NAME "/dev/video0" //ADD BY ME
#define CAPTURE_WIDTH 640
#define CAPTURE_HEIGHT 480

/*
void *capture_open (const char *dev_name, int v_width, int v_height, PixelFormat fmt);
int capture_get_picture (void *id, Picture *pic);
int capture_close (void *id);

int capture_get_output_ptr (void *id, unsigned char ***ptr, int **stride);
*/
void errno_exit                      (const char *           s);
int capture_open(void);
void capture_start(void);
void capture_stop(void);
int capture_read_frame(void);
#endif
