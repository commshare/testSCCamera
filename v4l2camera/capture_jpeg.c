/*
 * jpeg capturing example from UVC cam
 * requires: libjpeg-dev
 * build:
 *   gcc -std=c11 -Wall -Wextra -c capture.c -o capture.o
 *   gcc -std=c11 -Wall -Wextra -c capture-jpeg.c -o capture-jpeg.o
 *   gcc -Wall -Wextra capture.o capture-jpeg.o -ljpeg -o capture-jpeg
 */

#include "capture.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include<pthread.h>

#include <jpeglib.h>

#include"sc_log.h"

#include"sc_sdf.h"

#define OUT_JPEG 1
#define SC_WIDTH 640
#define SC_HEIGHT 480

bool camera_frame(camera_t* camera, struct timeval timeout) {
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(camera->fd, &fds);
  /*一旦有数据来就会写入，所以head可能会被覆盖掉*/
  int r = select(camera->fd + 1, &fds, 0, 0, &timeout);
  if (r == -1) exit(EXIT_FAILURE);
  if (r == 0) return false;
//  LOGD("capture one yuv");
  return camera_capture(camera);
}




//read a frame from the opened camera device
int read_frame_from_camera(camera_t *cam, uint8 *frame_buf, int *frame_length)
{
	struct v4l2_buffer buf;
	CLEAR(buf);
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;

	if (-1 == ioctl(cam->fd, VIDIOC_DQBUF, &buf))
	{
		switch(errno)
		{
		case EAGAIN:
			return 0;
		case EIO:
		default:/*遇到这种情况咋办*/
			LOGD("Read frame from camera Wrong\n");
			return -1;//BASICERROR;
		}
	}

	memcpy(frame_buf, cam->buffers[buf.index].start, cam->buffers[buf.index].length);
	*frame_length = cam->buffers[buf.index].length;

	if (-1 == ioctl(cam->fd, VIDIOC_QBUF, &buf))
	{
		printf("read frame frame camera put buf into the frame queue error");
		return -1;//BASICERROR;
	}

	return 0;//SUCCESS;
}



//file write
int file_write(uint8 *src, int length, FILE *fd)
{
	int ret_status = BASICERROR;

	do{
		//check parameter
		if (fd == NULL)
		{
			printf("wrong file operator in file write function\n");
			ret_status = PARAMERROR;
			break;
		}

		if (NULL == src || length < 1)
		{
			printf("wrong source input into file write function\n");
			ret_status = PARAMERROR;
			break;
		}

		//write to file
		if (0 == fwrite(src, length, 1, fd))
		{
			printf("write file wrong\n");
			ret_status = FILEERROR;
			break;
		}

		ret_status = SUCCESS;
	}while (0);

	return ret_status;
}

//save YUV format video
int	yuv_write(uint8 *src, int length,FILE *yuv_fd)
{
	return file_write(src, length, yuv_fd);
}

int get_and_compress_pic(camera_t *cam,FILE* fp,int width,int height,int bpp)
{
	int count = 1;
	fd_set fds;
	struct timeval tv;
	int	ret;
	int length=0;
	uint8 *pic;
	uint8 *pic_out;
	int out_length=0;

    int pic_size=width * height * bpp;
	pic = (int8 *)malloc(pic_size); //内存在这里分配
	if (NULL == pic)
	{
		printf("can't get enough memory in main function\n");
		exit(EXIT_FAILURE);
	}

	pic_out = (int8 *)malloc(pic_size);
	if (NULL == pic_out)
	{
		printf("can't get enough memory in main function\n");
		exit(EXIT_FAILURE);
	}

	while(!cam->stopcap_flag)
	{
		length = 0;
		out_length = 0;
		memset(pic, 0, pic_size);
		memset(pic_out, 0, pic_size);
		printf("\n\n this is the %d th frame\n", count);
		if (count++ > 100)
		{
			printf("exit\n");
			break; /*一个breadk应该就可以调成while吧*/
		}

		FD_ZERO(&fds);
		FD_SET(cam->fd, &fds);

		tv.tv_sec = 2;
		tv.tv_usec = 0;

		ret = select(cam->fd + 1, &fds, NULL, NULL, &tv);

		switch(ret)
		{
		case -1:
			break;
		case 0:
			printf("time out \n");
			break;
		default:
			read_frame_from_camera(cam, pic, &length);
            /*从pic拿到数据*/

			yuv_write(pic, length,fp);

			if (pic[0] == '\0')
			{
				break;
			}
		//	out_length = compress_frame(-1, pic, pic_out);
		//	h264_write(pic_out, out_length);
		}
	}
}


/*
在 pthread_create 的第三个参数，是这么定义的:
    void *(* _start_routine) (void *)
*/

void sc_get_frame(camera_t *cam)//,int width,int height,int bpp)
{
	int width=SC_WIDTH;
	int height=SC_HEIGHT;
	int bpp=2;
	int count = 1;
	fd_set fds;
	struct timeval tv;
	int	ret;
	int length=0;
	uint8 *pic;
	uint8 *pic_out;
	int out_length=0;

    int pic_size=width * height * bpp;
	sc_frame_t *frame=new_frame(pic_size);
	//pic = (int8 *)malloc(pic_size); //内存在这里分配
	//if (NULL == pic)
	//	{
	//	printf("can't get enough memory in main function\n");
	//	exit(EXIT_FAILURE);
	//}

	//pic_out = (int8 *)malloc(pic_size);
	//if (NULL == pic_out)
	//{
	//	printf("can't get enough memory in main function\n");
	//	exit(EXIT_FAILURE);
	//}

	while(!cam->stopcap_flag)
	{
		length = 0;
	//	out_length = 0;
	//	memset(pic, 0, pic_size);
	//	memset(pic_out, 0, pic_size);
		printf("\n\n this is the %d th frame\n", count);
		if (count++ > 100)
		{
			printf("exit\n");
			break; /*一个breadk应该就可以调成while吧*/
		}

		FD_ZERO(&fds);
		FD_SET(cam->fd, &fds);

		tv.tv_sec = 2;
		tv.tv_usec = 0;

		ret = select(cam->fd + 1, &fds, NULL, NULL, &tv);

		switch(ret)
		{
		case -1:
			break;
		case 0:
			printf("time out \n");
			break;
		default:
			int r=read_frame_from_camera(cam, frame->data, &frame->len);
			if(r==-1)
			{
				LOGE("READ FROM CAM Wrong");
				sleep(2);
			}
            /*从pic拿到数据*/
            int ret=queue_pushback(cam->q,frame);
			if(ret==-1){
				LOGD("pushback queue fail for FULL");
				sleep(1);
			}
			//yuv_write(pic, length,fp);

		//	if (pic[0] == '\0')
		//	{
		//		break;
		//	}
		//	out_length = compress_frame(-1, pic, pic_out);
		//	h264_write(pic_out, out_length);
		}
	}
}
void sc_writeyuv(camera_t *cam){

	FILE *fp=cam->fpyuv;
	sc_frame_t *pkt;
	while(!cam->stopcap_flag){
		pkt=NULL;
	int ret=queue_popfront(cam->q, &pkt);
	if(ret==-1){
		LOGD("pop front fail");
		sleep(1);
	}else{
		LOGD("write frame to yuv fp len[%d]",pkt->len);
		yuv_write(pkt->data,pkt->len,fp);
		delete_frame(pkt);
	}
	}
}


void
jpeg(FILE* dest, uint8_t* rgb, uint32_t width, uint32_t height, int quality)
{
  JSAMPARRAY image = calloc(height, sizeof (JSAMPROW));
  for (size_t i = 0; i < height; i++) {
    image[i] = calloc(width * 3, sizeof (JSAMPLE));
    for (size_t j = 0; j < width; j++) {
      image[i][j * 3 + 0] = rgb[(i * width + j) * 3 + 0];
      image[i][j * 3 + 1] = rgb[(i * width + j) * 3 + 1];
      image[i][j * 3 + 2] = rgb[(i * width + j) * 3 + 2];
    }
  }

  struct jpeg_compress_struct compress;
  struct jpeg_error_mgr error;
  compress.err = jpeg_std_error(&error);
  jpeg_create_compress(&compress);
  jpeg_stdio_dest(&compress, dest);
  compress.image_width = width;
  compress.image_height = height;
  compress.input_components = 3;
  compress.in_color_space = JCS_RGB;
  jpeg_set_defaults(&compress);
  jpeg_set_quality(&compress, quality, TRUE);
  jpeg_start_compress(&compress, TRUE);
  jpeg_write_scanlines(&compress, image, height);
  jpeg_finish_compress(&compress);
  jpeg_destroy_compress(&compress);

  for (size_t i = 0; i < height; i++) {
    free(image[i]);
  }
  free(image);
}


int main(int argc, char* argv[])
{
  char* device = argc > 1 ? argv[1] : "/dev/video0";
  uint32_t width = argc > 2 ? atoi(argv[2]) : SC_WIDTH;
  uint32_t height = argc > 3 ? atoi(argv[3]) : SC_HEIGHT;
  char* output = argc > 4 ? argv[4] : "result.jpg";


  FILE *fpyuv;
  char *yuv_file="zb.yuv";
  fpyuv=fopen(yuv_file,"w");
  if(fpyuv==NULL){
	LOGD("fopen yuvfile[%s] fail [%s]",yuv_file,strerror(errno));
	return -1;
  }

  camera_t* camera = camera_open(device);
  if (!camera) {
    fprintf(stderr, "[%s] %s\n", device, strerror(errno));
    return EXIT_FAILURE;
  }
  /*FILE *fpyuv */
  camera->fpyuv=fpyuv;
  LOGD("config with width[%d] height[%d]",width,height);
  camera_format_t config = {0, width, height, {0, 0}};
  if (!camera_config_set(camera, &config)) goto error;
  if (!camera_start(camera)) goto error;

  struct timeval timeout;
  timeout.tv_sec = 1;
  timeout.tv_usec = 0;
  int i;
  /* skip 5 frames for booting a cam */
  for ( i = 0; i < 5; i++) {
  	LOGD("skip");
    camera_frame(camera, timeout);
  }


#if OUT_JPEG
LOGD("out a jpeg w[%d] h[%d]",camera->width, camera->height);
  /*视频帧导出*/
  camera_frame(camera, timeout);
  FILE *oneyuv=fopen("one422.yuv","rw");
  if(oneyuv!=NULL)
  {
  	int ilen=camera->head.length;
  	LOGD("write len[%d] yuv to file",ilen);
    int r=fwrite(camera->head.start,ilen,1,oneyuv);
	LOGD("fwrite to yuv ,ret[%d]",r);

    char *onejpegname="onejpeg.jpg";
    FILE *onejpeg=fopen(onejpegname,"w");
	if(onejpeg!=NULL){
		uint8_t *outbuf=(uint8_t *)malloc(ilen*sizeof(uint8_t));
		int ret=fread(outbuf,ilen,1,oneyuv);
		LOGD("after fread from oneyuv to outrgb ,return[%d]",ret);
		uint8_t * onergb =yuyv2rgb(outbuf, camera->width, camera->height);
        jpeg(onejpeg,onergb, camera->width, camera->height, 90);

		 free(outbuf);
		 free(onergb);
		 fclose(onejpeg);
	}else
	   LOGD("onejpeg fopen fail");
	fclose(oneyuv);

  }else
  	LOGD("fopen oneyuv fail");
  /*YUYV转为rgb*/
  unsigned char* rgb =
    yuyv2rgb(camera->head.start, camera->width, camera->height);
  FILE* out = fopen(output, "w");
  /*jpeg是对rgb编码么?*/
  jpeg(out, rgb, camera->width, camera->height, 100);
 #endif

 #if 0
 LOGD("write 100 yuv422 to file");
  for(i=0;i<100;i++)
 {
    camera_frame(camera, timeout);
 	fwrite(camera->head.start,camera->head.length,1,fpyuv);
  }
  #else
  // get_and_compress_pic(camera,fpyuv,width,height,2);
  /*typedef unsigned long int pthread_t*/
  pthread_t pid_getframe,pid_writeyuv;
  pthread_create(&pid_getframe,NULL,sc_get_frame,camera);
  pthread_create(&pid_writeyuv,NULL,sc_writeyuv,camera);

  #endif


 #if OUT_JPEG
  fclose(out);
  free(rgb);
 #endif
  fclose(fpyuv);
  fpyuv=NULL;

  camera_stop(camera);
  camera_close(camera);
  return 0;
 error:
  camera_close(camera);
  return EXIT_FAILURE;
}
