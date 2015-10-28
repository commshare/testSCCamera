#ifndef __SC_PROCESS_IMAGE_H_
#define __SC_PREOCESS_IMAGE_H_

#include"capture.h"
 void sc_process_image(const void *p, int size,int cmd,camera_t *camera);

enum cmd{

  CMD_OUT_FILE=0x11,
  CMD_OUT_BUF
};
#endif
