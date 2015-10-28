/*
 * jpeg capturing example from UVC cam
 * requires: libjpeg-dev
 * build:
 *   gcc -std=c11 -Wall -Wextra -c capture.c -o capture.o
 *   gcc -std=c11 -Wall -Wextra -c capture-jpeg.c -o capture-jpeg.o
 *   gcc -Wall -Wextra capture.o capture-jpeg.o -ljpeg -o capture-jpeg

 gcc -o  armqueueyuv capture.c capture_jpeg.c sc/sc_frame.c sc/sc_queue.c -ljpeg -lpthread -std=c99 -I. -I./sc -g
 */

#include "capture.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include<pthread.h>

#include <jpeglib.h>

#include"sc_log.h"

#include"sc_sdf.h"

#define OUT_JPEG 0
#define SC_WIDTH 640
#define SC_HEIGHT 480

typedef enum
{
  IO_METHOD_READ,
  IO_METHOD_MMAP,
  IO_METHOD_USERPTR,
} io_method;


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

static void errno_exit(const char *s)
{
  fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
  exit(EXIT_FAILURE);
}



#if 0
bool sc_camera_capture(camera_t* camera) {
  struct v4l2_buffer buf;
  memset(&buf, 0, sizeof buf);
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  if (xioctl(camera->fd, VIDIOC_DQBUF, &buf) == -1) return false;
  /*内存导出*/
  //memcpy(camera->head.start, camera->buffers[buf.index].start, buf.bytesused);
   memcpy(frame_buf, cam->buffers[buf.index].start, cam->buffers[buf.index].length);
	*frame_length = cam->buffers[buf.index].length;

  camera->head.length = buf.bytesused;
  if (xioctl(camera->fd, VIDIOC_QBUF, &buf) == -1) return false;
  return true;
}
#endif
bool sc_camera_captureframe(camera_t* camera, struct timeval timeout,uint8 *frame_buf, int *frame_length) {
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(camera->fd, &fds);
  /*一旦有数据来就会写入，所以head可能会被覆盖掉*/
  int r = select(camera->fd + 1, &fds, 0, 0, &timeout);
  if (r == -1){ LOGD("exit"); exit(EXIT_FAILURE);}
  if (r == 0){
  	LOGD("timeout");
  	return false;
  }
//  LOGD("capture one yuv");
#if 0
  return sc_camera_capture(camera);
#else
  struct v4l2_buffer buf;
  memset(&buf, 0, sizeof buf);
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  if (xioctl(camera->fd, VIDIOC_DQBUF, &buf) == -1){
	LOGE("VIDIOC_DQBUF FAIL");
	return false;
  }
  /*内存导出*/
  //memcpy(camera->head.start, camera->buffers[buf.index].start, buf.bytesused);
   memcpy(frame_buf, camera->buffers[buf.index].start, camera->buffers[buf.index].length);
	*frame_length =buf.bytesused; //camera->buffers[buf.index].length;

 // camera->head.length = buf.bytesused;
 LOGD("##buf.bytesused[%d]  camera->buffers[buf.index].length[%d]",buf.bytesused,camera->buffers[buf.index].length);
  if (xioctl(camera->fd, VIDIOC_QBUF, &buf) == -1) return false;
  return true;
#endif

}


/*
在 pthread_create 的第三个参数，是这么定义的:
    void *(* _start_routine) (void *)

http://outofmemory.cn/code-snippet/34450/linux-create-function-pthread-create-mode
  返回值是void *, 用  return ((void *) 0);
*/
 int xioctl(int fd, unsigned long int request, void* arg)
{
  for (int i = 0; i < 100; i++) {
    int r = ioctl(fd, request, arg);
    if (r != -1 || errno != EINTR) return r;
  }
  return -1;
}
/*http://www.saillard.org/linux/pwc/files/capture.c*/
 int xioctl0(int fd, int request, void *arg)
{
  int r;

  do
    r = ioctl(fd, request, arg);
  while (-1 == r && EINTR == errno);

  return r;
}
static io_method io = IO_METHOD_MMAP;
static int read_frame(camera_t *camera)
{
  struct v4l2_buffer buf;
  unsigned int i;
  ssize_t read_bytes;
  unsigned int total_read_bytes;
  int fd=camera->fd;

  switch (io)
    {
		#if 0
    case IO_METHOD_READ:
      total_read_bytes = 0;
      do
       {
	 read_bytes = read(fd, buffers[0].start, buffers[0].length);
	 if (read_bytes < 0)
	  {
	    switch (errno)
	     {
	      case EIO:
	      case EAGAIN:
		continue;
	      default:
		errno_exit("read");
	     }
	  }
	 total_read_bytes += read_bytes;

       } while (total_read_bytes < buffers[0].length);
      process_image(buffers[0].start, buffers[0].length);

      break;
#endif
    case IO_METHOD_MMAP: //这个
      CLEAR(buf);

      buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      buf.memory = V4L2_MEMORY_MMAP;

      if (-1 == xioctl0(fd, VIDIOC_DQBUF, &buf))
	{
	  switch (errno)
	    {
	    case EAGAIN:
			LOGE("EAGAIN");
	      return 0; //这是失败啊

	    case EIO:			LOGE("EIO");

	      /* Could ignore EIO, see spec. */

	      /* fall through */
		 // break;//add by me 20151026 to go out of switch

	    default:
	      errno_exit("VIDIOC_DQBUF");
	    }
	}
	  int n_buffers=camera->buffer_count;
    //  assert(buf.index < n_buffers);

   //   process_image(buffers[buf.index].start, buf.length);

      if (-1 == xioctl(camera->fd, VIDIOC_QBUF, &buf))
	errno_exit("VIDIOC_QBUF");

      break;

    case IO_METHOD_USERPTR:
		LOGD("IO_METHOD_USERPTR");
		#if 0
      CLEAR(buf);

      buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      buf.memory = V4L2_MEMORY_USERPTR;

      if (-1 == xioctl0(fd, VIDIOC_DQBUF, &buf))
	{
	  switch (errno)
	    {
	    case EAGAIN:
	      return 0;

	    case EIO:
	      /* Could ignore EIO, see spec. */

	      /* fall through */

	    default:
	      errno_exit("VIDIOC_DQBUF");
	    }
	}

      for (i = 0; i < n_buffers; ++i)
	if (buf.m.userptr == (unsigned long) buffers[i].start
	    && buf.length == buffers[i].length)
	  break;

      assert(i < n_buffers);

   //  process_image((void *) buf.m.userptr, buf.length);

      if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
	errno_exit("VIDIOC_QBUF");
#endif

      break;
    }

  return 1;
}
 int count=10;
//static void mainloop(void)
void *sc_get_frame_loop(void *cam)
{
   camera_t * camera=(camera_t*)cam;
	int fd=camera->fd;
  while (count-- > 0)
 {
      for (;;)
	{
	  fd_set fds;
	  struct timeval tv;
	  int r;

	  FD_ZERO(&fds);
	  FD_SET(fd, &fds);

	  /* Timeout. */
	  tv.tv_sec = 2;
	  tv.tv_usec = 0;

	  r = select(fd + 1, &fds, NULL, NULL, &tv);

	  if (-1 == r)
	    {
	      if (EINTR == errno)
		   continue;

	      errno_exit("select");
	    }

	  if (0 == r)
	    {
	      fprintf(stderr, "select timeout\n");
	      exit(EXIT_FAILURE);
	    }

	  if (read_frame(camera))
	  { /*如果read_frame成功了，那么break跳出for循环*/
	  	break;
	  }
      /*如果read_frame失败，继续本次for*/
	  /* EAGAIN - continue select loop. */
	}
    }
  	 return ((void *) 0);
}
#if 0


void *sc_get_frame00(void *camera)//,int width,int height,int bpp)
{
	camera_t *cam=(void *)camera;
	int width=SC_WIDTH;
	int height=SC_HEIGHT;
	int bpp=2;
	//int count = 1;
	fd_set fds;
	struct timeval tv;
	int	ret;
	int length=0;
	uint8 *pic;
	uint8 *pic_out;
	int out_length=0;
    int pic_size=width * height * bpp;
  struct timeval timeout;
  timeout.tv_sec = 2;
  timeout.tv_usec = 0;
  struct v4l2_buffer buf;

while(!cam->stopcap_flag)
{
		length = 0;
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(cam->fd, &fds);
//AGAIN:
  int r = select(cam->fd + 1, &fds, 0, 0, &timeout);
  if (r == -1){ LOGD("SELECT exit"); exit(EXIT_FAILURE);}
  if (r == 0){
  	LOGD("timeout");

	//goto AGAIN; /*以后改成continue算了*/
	continue;
  }

  memset(&buf, 0, sizeof buf);
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
 // if (xioctl(cam->fd, VIDIOC_DQBUF, &buf) == -1){
	//LOGE("VIDIOC_DQBUF FAIL");
   // goto AGAIN;
  // continue;
  /*http://www.saillard.org/linux/pwc/files/capture.c*/
   if (-1 == xioctl(cam->fd, VIDIOC_DQBUF, &buf))
	{
	  switch (errno)
	  {
	    case EAGAIN:
          LOGD("EAGAIN");
	      //return 0;

	    case EIO:
	        LOGD("EIO");

	      /* Could ignore EIO, see spec. */

	      /* fall through */

	    default:
	      errno_exit("VIDIOC_DQBUF");
	    }
	}
  }
  /*内存导出*/
  //memcpy(camera->head.start, camera->buffers[buf.index].start, buf.bytesused);
   sc_frame_t *frame=new_frame(pic_size);
  if(frame==NULL){
	// return
	LOGE("alloc frame fail...");
	//goto AGAIN;
	continue;
  }
   memcpy(frame->data, cam->buffers[buf.index].start, cam->buffers[buf.index].length);
  frame->len =buf.bytesused; //camera->buffers[buf.index].length;

 // camera->head.length = buf.bytesused;
 LOGD("##buf.bytesused[%d]  camera->buffers[buf.index].length[%d]",buf.bytesused,cam->buffers[buf.index].length);
 /*
VIDIOC_QBUF：把数据从缓存中读取出来
VIDIOC_DQBUF：把数据放回缓存队列
 */
  if (xioctl(cam->fd, VIDIOC_QBUF, &buf) == -1) {//return false;
	LOGE("VIDIOC_QBUF FAIL");
  		//goto AGAIN;
	continue;

  }
////
static int count=0;

			LOGD("pushback [%d]th frame",count++);
            int ret=queue_pushback(cam->q,frame);
			if(ret==-1){
				LOGD("pushback queue FAIL because of FULL");
				//sleep(1); /*不sleep的原因是要继续select吧*/
					//goto AGAIN;
	continue;
		}


	 return ((void *) 0);
}
#endif

//read a frame from the opened camera device
int read_frame_from_camera(camera_t *cam, uint8 *frame_buf, int *frame_length)
{
	struct v4l2_buffer buf;
	CLEAR(buf);
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;

/*
执行成功时，函数返回值为 0；函数执行成功后，
相应的内核视频缓冲区中保存有当前拍摄到的视频数据，
应用程序可以通过访问用户空间来读取该视频数据。（前面已经通过调用函数mmap做了用户空间和内核空间的内存映射）.
*/
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
			LOGD("=1=wrong file operator in file write function\n");
			ret_status = PARAMERROR;
			break;
		}

		if (NULL == src || length < 1) /*这里*/
		{
			LOGD("=2=wrong source input into file write function\n");
			if(NULL==src){
				LOGD("SRC IS NULL");
			}
			if(length<1){
				LOGD("LENGTH < 1");
			}
			ret_status = PARAMERROR;
			break;
		}

		//write to file
		if (0 == fwrite(src, length, 1, fd))
		{
			LOGD("write file wrong\n");
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

http://outofmemory.cn/code-snippet/34450/linux-create-function-pthread-create-mode
  返回值是void *, 用  return ((void *) 0);
*/

void *sc_get_frame0(void *camera)//,int width,int height,int bpp)
{
	camera_t *cam=(void *)camera;
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

	while(!cam->stopcap_flag)
	{
		length = 0;
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
				LOGD("time out \n");
				break;
			default:
			{
				sc_frame_t *frame=new_frame(pic_size);
				int rr=read_frame_from_camera(cam, frame->data, &frame->len);
				if(rr==-1)
				{
					LOGE("READ FROM CAM Wrong,delete frame the select agin");
				//sleep(2);
				//delete_frame(frame);
				break;//go out of switch not while ,wait for next
			    }

            /*从pic拿到数据*/
			LOGD("pushback [%d]th frame",count);
            int ret=queue_pushback(cam->q,frame);
			if(ret==-1){
				LOGD("pushback queue fail for FULL");
				sleep(1);
			}
			/*//add by me 20151023
			go out of switch ,not while
			*/
			break;
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
	 return ((void *) 0);
}
void *sc_writeyuv(void *camera){
    camera_t* cam=(camera_t *)camera;

	FILE *fp=cam->fpyuv;
	sc_frame_t *pkt=NULL;
	while(!cam->stopcap_flag){
		pkt=NULL;
		int ret=queue_popfront(cam->q, &pkt);
		if(ret==-1){
			LOGD("pop front fail");
			sleep(1);
		}else{
		    if(pkt!=NULL){
				LOGD("write frame to yuv fp len[%d]",pkt->len);
				if(yuv_write(pkt->data,pkt->len,fp)!=SUCCESS)
				{
					LOGD("YUVWRITE FAIL");
				}
				else{
					LOGD("write yuv ok");
					delete_frame(pkt);
				}
	    	}else{
	    		LOGE("ERROR popfront GET NULL PKT");
			}
	   }
   }
   return ((void *) 0);
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

  #if 0
  /* skip 5 frames for booting a cam */
  for ( i = 0; i < 5; i++) {
  	LOGD("skip");
    camera_frame(camera, timeout);
  }
  #endif


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
  pthread_create(&pid_getframe,NULL,sc_get_frame_loop,camera);
//  pthread_create(&pid_writeyuv,NULL,sc_writeyuv,camera);

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
