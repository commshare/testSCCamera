#include <stdlib.h> //for malloc free


#include"sc_frame.h"
#include"sc_log.h"

/*http://bbs.csdn.net/topics/340248549
函数中的内存在栈中分配，而malloc在堆中申请内存
一般的strcpy是不检测内存重叠的，但memcpy检测。

http://www.zhihu.com/question/26053909
malloc申请的内存空间是在堆上的，堆由程序员分配和释放（或者程序结束后由操作系统回收）。

程序中你返回temp说明你可能在函数外使用到函数中申请的内存空间，所以不需要释放。
*/
static sc_frame_t
*init_frame(int data_size){
     sc_frame_t * frame=NULL;
	 frame=(uint8_t *)malloc(sizeof(sc_frame_t));
	 if(frame){
		 uint8_t *data=NULL;
		 data=(uint8_t *)malloc(sizeof(uint8_t)*data_size);
		 if(data){
			 frame->data=data;
		 	 frame->len=data_size;
		 }else{
		     LOGE("malloc frame data fail");
		     frame->data=NULL;
			 frame->len=0;
		 }
	 }else
	 {
	 	LOGE("malloc frame fail");
	 }
	 return frame;
}


 sc_frame_t *new_frame(int  size){
     sc_frame_t *frame=init_frame(size);
	 if(frame==NULL){
		LOGE("NEW FRAME FAIL");
	 }
	// LOGD("new frame ok");
	 return frame;
}

/* http://zhidao.baidu.com/question/517834746.html
程序结束时，它只会销毁指针所占用的内存地址，而指针所指向的堆内存的地址并不会随着程序结束而销毁，这就是所谓的“内存泄漏”，所以一定要手动的free掉。
*/
int delete_frame(sc_frame_t *frame){
	LOGD("delete frame");
	if(frame){
	    if(frame->data)
		{
			free(frame->data);
			frame->data=NULL;
    	}
		free(frame);
		frame=NULL;
	}

}
