#ifndef __SC_PROCESS_IMAGE_H_
#define __SC_PREOCESS_IMAGE_H_

 void sc_process_image(const void *p, int size,int cmd);

enum cmd{
  CMD_OUT_FILE=0x11
};
#endif
