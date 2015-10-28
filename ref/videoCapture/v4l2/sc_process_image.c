#include<stdio.h>
#include<stdlib.h>
#include"sc_process_image.h"

int frame_number=0;
 void sc_process_image(const void *p, int size,int cmd)
{
	switch(cmd){
		case CMD_OUT_FILE:
			{
       			 frame_number++;
      			  char filename[15];
      			  sprintf(filename, "frame-%d.yuv", frame_number);
      			  FILE *fp=fopen(filename,"wb");


                  fwrite(p, size, 1, fp);

      			  fflush(fp);
      			  fclose(fp);
			}
		break;
		default:
			printf("process image");
	}
}