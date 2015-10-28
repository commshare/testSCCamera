#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <string.h> //for memset ,strerror
#include<errno.h>

struct Buffer
{
	void *start;
	size_t length;
};
typedef struct Buffer Buffer;

struct Ctx
{
	int vid;
	int width, height;	// 杈撳嚭鍥惧儚澶у皬
	//struct SwsContext *sws;	// 鐢ㄤ簬杞崲
	int rows;	// 鐢ㄤ簬 sws_scale()
	int bytesperrow; // 鐢ㄤ簬cp鍒?pic_src
	//AVPicture pic_src, pic_target;	// 鐢ㄤ簬 sws_scale
	Buffer bufs[2];		// 用于 mmap
       // PixelFormat fmt;
};
typedef struct Ctx Ctx;



void *capture_open (const char *dev_name, int t_width, int t_height)
{
	int id = open(dev_name, O_RDWR | O_NONBLOCK,0);
	if (id < 0){ 
		fprintf(stderr,"cannot open device %s :%d, %s \n", dev_name,errno,strerror(errno));
		return 0;
	}

	Ctx *ctx = new Ctx;
	ctx->vid = id;
 
	// to query caps
	v4l2_capability caps;
	ioctl(id, VIDIOC_QUERYCAP, &caps);

	if (caps.capabilities & V4L2_CAP_VIDEO_CAPTURE) {
	//	if (caps.capabilities & V4L2_CAP_READWRITE) {
	//		// TODO: ...
	//	}
		if (caps.capabilities & V4L2_CAP_STREAMING) {
			      /* Select video input, video standard and tune here. */
			// 妫€鏌ユ槸鍚︽敮鎸?MMAP, 杩樻槸 USERPTR
			/*
 			*	M E M O R Y - M A P P I N G   B U F F E R S
 			*/
			v4l2_requestbuffers bufs;
			memset(&bufs, 0, sizeof(bufs));
			bufs.count = 2;
			bufs.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			bufs.memory = V4L2_MEMORY_MMAP;
			if (ioctl(id, VIDIOC_REQBUFS, &bufs) < 0) {
				fprintf(stderr, "%s: don't support MEMORY_MMAP mode!\n", __func__);
				close(id);
				delete ctx;
				return 0;
			}

			fprintf(stderr, "%s: using MEMORY_MMAP mode, buf cnt=%d\n", __func__, bufs.count);

			// mmap,用俩v4l2_buffer支持MMAP，为啥呢?
			for (int i = 0; i < 2; i++) {
				v4l2_buffer buf; //视频
				memset(&buf, 0, sizeof(buf));
				buf.type = bufs.type; //是capture
				buf.memory = bufs.memory; //是MMAP
				if (ioctl(id, VIDIOC_QUERYBUF, &buf) < 0) {
					fprintf(stderr, "%s: VIDIOC_QUERYBUF ERR\n", __func__);
					close(id);
					delete ctx;
					return 0;
				}

				ctx->bufs[i].length = buf.length;//
				ctx->bufs[i].start = mmap(0, buf.length, PROT_READ|PROT_WRITE,
						MAP_SHARED, id, buf.m.offset);
			}
		}
		else {
			fprintf(stderr, "%s: can't support read()/write() mode and streaming mode\n", __func__);
			close(id);
			delete ctx;
			return 0;
		}
	}
	else {
		fprintf(stderr, "%s: can't support video capture!\n", __func__);
		close(id);
		delete ctx;
		return 0;
	}

	int rc;

	// enum all support image fmt
	v4l2_fmtdesc fmt_desc;
	uint32_t index = 0;   
	
	 // 看起来, 不支持 plane fmt, 直接使用 RGB 吧, 然后使用 libswscale
#if 1
	do {
		fmt_desc.index = index;
		fmt_desc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		rc = ioctl(id, VIDIOC_ENUM_FMT, &fmt_desc);

		if (rc >= 0) {
			fprintf(stderr, "\t support %s\n", fmt_desc.description);
		}
		index++;
	} while (rc >= 0);
#endif // 0
	v4l2_format fmt;
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	rc = ioctl(id, VIDIOC_G_FMT, &fmt);
	if (rc < 0) {
		fprintf(stderr, "%s: can't VIDIOC_G_FMT...\n", __func__);
		return 0;
	}

//	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
//	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
//	rc = ioctl(id, VIDIOC_S_FMT, &fmt);
//	if (rc < 0) {
//		fprintf(stderr, "%s: can't support V4L2_PIX_FMT_MJPG\n", __func__);
//		return 0;
//	}
//	if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_MJPEG) {
//		fprintf(stderr, "%s: can't support V4L2_PIX_FMT_MJPG\n", __func__);
//		return 0;
//	}
#if 0
	PixelFormat pixfmt = PIX_FMT_NONE;
	switch (fmt.fmt.pix.pixelformat) {
	case V4L2_PIX_FMT_YUYV:
		pixfmt = PIX_FMT_YUYV422;
		break;

	case V4L2_PIX_FMT_MJPEG:
		// pixfmt = PIX_FMT_YUVJ422P;
		// 浣跨敤 mjpeg 搴旇鑳藉婊¤冻 640x480x25, 浣嗘槸闇€瑕佽В鐮?mjpeg
		
		break;
	}

	if (pixfmt == PIX_FMT_NONE) {
		fprintf(stderr, "%s: can't support %4s\n", __func__, (char*)&fmt.fmt.pix.pixelformat);
		return 0;
	}

	// 鏋勯€犺浆鎹㈠櫒
	fprintf(stderr, "capture_width=%d, height=%d, stride=%d\n", fmt.fmt.pix.width, fmt.fmt.pix.height,
			fmt.fmt.pix.bytesperline);
	ctx->width = t_width;
	ctx->height = t_height;
	ctx->sws = sws_getContext(fmt.fmt.pix.width, fmt.fmt.pix.height, pixfmt,
                        ctx->width, ctx->height, tarfmt, 	// PIX_FMT_YUV420P 瀵瑰簲 X264_CSP_I420
			SWS_FAST_BILINEAR, 0, 0, 0);

	ctx->rows = fmt.fmt.pix.height;
	ctx->bytesperrow = fmt.fmt.pix.bytesperline;

        avpicture_alloc(&ctx->pic_target, tarfmt, ctx->width, ctx->height);

	// queue buf
	for (int i = 0; i < sizeof(ctx->bufs)/sizeof(Buffer); i++) {
		v4l2_buffer buf;
		memset(&buf, 0, sizeof(buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		if (ioctl(id, VIDIOC_QBUF, &buf) < 0) {
			fprintf(stderr, "%s: VIDIOC_QBUF err\n", __func__);
			exit(-1);
		}
	}

	int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (ioctl(id, VIDIOC_STREAMON, &type) < 0) {
		fprintf(stderr, "%s: VIDIOC_STREAMON err\n", __func__);
		exit(-1);
	}

        ctx->fmt = tarfmt;
#endif
	return ctx;
}


int capture_close (void *id)
{
	Ctx *ctx = (Ctx*)id;
	for (int i = 0; i < sizeof(ctx->bufs)/sizeof(Buffer); i++) {
		munmap(ctx->bufs[i].start, ctx->bufs[i].length);
	}
	//avpicture_free(&ctx->pic_target);
	//sws_freeContext(ctx->sws);
	delete ctx;

	return 1;
}



#define VIDEO_WIDTH 320
#define VIDEO_HEIGHT 240
#define VIDEO_FPS 10.0
#define DEVICE_NAME "/dev/video0"

int main (int argc, char **argv)
{
    unsigned short target_port = 0;
    sscanf(argv[2],"%hd",&target_port);
    printf("Client_ip = %s, port = %hd\n", argv[1], target_port);
    
	void *capture = capture_open(DEVICE_NAME, VIDEO_WIDTH, VIDEO_HEIGHT);
	if (!capture) {
		fprintf(stderr, "ERR: can't open '/dev/video0'\n");
		exit(-1);
	}
    capture_close(capture);
    return 0;
}
