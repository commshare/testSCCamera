#include<stdio.h>
#include<stdlib.h>
#include"sc_process_image.h"

#include"sc_frame.h"
#include"sc_queue.h"
#include<unistd.h>
int frame_number=0;


void sc_write_onebyone(const void *buf,int size,int frame_number){

                 char filename[15];
      			  sprintf(filename, "frame-%d.yuv", frame_number);
      			  FILE *fp=fopen(filename,"wb");


                  fwrite(buf, size, 1, fp);

      			  fflush(fp);
      			  fclose(fp);
}
 void sc_process_image(const void *buf, int size,int cmd,camera_t *camera)
{
	switch(cmd){
		case CMD_OUT_FILE:
			{
       			 frame_number++;
				 sc_write_onebyone(buf,size,frame_number);

			}
		break;
		case CMD_OUT_BUF:
		{	sc_frame_t *f=new_frame(size);
			sc_queue_t *q=camera->q;
			int ret=queue_pushback(q, f);
			if(ret==QGNEL_FAIL || ret==QFULL_PUSHFAIL){
				//sleep(1);
				 usleep(500);
			}
			break;
		}
		default:
			printf("process image");
	}
}