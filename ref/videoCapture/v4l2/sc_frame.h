#ifndef __SC_FRAME_H_
#define __SC_FRAME_H_


#include"sc_sdf.h"

typedef struct SCFrame{
uint8_t *data;
int len;
}sc_frame_t;

 sc_frame_t *new_frame(int  size);

int delete_frame(sc_frame_t *frame);


#endif
