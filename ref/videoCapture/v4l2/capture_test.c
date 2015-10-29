#include"capture.h"
#include"pthread.h"
#include<stdio.h>
#include<unistd.h>

//file write
int file_write(uint8 *src, int length, FILE *fd)
{
	int ret_status = 0;

	do{
		//check parameter
		if (fd == NULL)
		{
			LOGD("=1=wrong file operator in file write function\n");
			ret_status = -1;
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
			ret_status = -1;
			break;
		}

		//write to file
		if (0 == fwrite(src, length, 1, fd))
		{
			LOGD("write file wrong\n");
			ret_status = -1;//FILEERROR;
			break;
		}

		ret_status = 0;//SUCCESS;
	}while (0);

	return ret_status;
}

//save YUV format video
int	yuv_write(uint8 *src, int length,FILE *yuv_fd)
{
	return file_write(src, length, yuv_fd);
}
FILE *zbfp;
void *thread_pop(void * camera){
	 camera_t *cam=(camera_t *)camera;
	 sc_frame_t *f;
	 int count=0;
	 zbfp=fopen("zbzb.yuv","wb");


	 slogi("pop ");
	 while(!cam->stopflag){
	 	if(count==0){
	 		sc_lock(&cam->q->mutex);
			slogi("wait for signal");
			pthread_cond_wait(&cam->q->cond,&cam->q->mutex);/*等待*/
			slogi("wake up");
			sc_unlock(&cam->q->mutex);
	 	}
	 	count++;
	 	int ret=queue_popfront(cam->q,&f);
		 if(ret==0 && f!=NULL){
		 	// slogi("pop ok delete it");
			  if(zbfp)
			  {
			  	 yuv_write(f->data, f->len, zbfp);
                // sc_write_onebyone(f->data, f->len, count);

			  }else{
				slogi("#####zbzb.yuv open fail#####");
			  }
			 delete_frame(f);}
		 if(ret==QEMPTY_POPFAIL || ret==QGNEL_FAIL ){//宁愿popfail，也不能让push数据放不进去而丢帧
				usleep(300);
		 }
	 }

}

//void mainloop(int fd)
void *thread_mainloop(void * camera)
{
	 camera_t *cam=(camera_t *)camera;
	 int	fd=cam->fd;
	//unsigned int count;
       // count = 500;
        while (!cam->stopflag) {
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

			if (capture_read_frame (cam))
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

	camera_t *cam=malloc(sizeof(camera_t));
	cam->fd=fd;
	cam->stopflag=0;
    pthread_t cam_pid;
	pthread_t pop_pid;
    sc_queue_t *q=queue_new(50);
	if(q)
		cam->q=q;
  // mainloop(fd);
  pthread_create(&cam_pid,NULL,thread_mainloop,cam);
  pthread_create(&pop_pid,NULL,thread_pop,cam);

 //capture_stop();

	//printf("#IN##Mainloop BEGIN ######\n");
	//ADD BY ME BEGIN
  pthread_join (cam_pid, NULL);
  pthread_join (pop_pid, NULL);
  if(zbfp)
  	fclose(zbfp);
  queue_destroy(cam->q);
  capture_stop();

return 0;

}
