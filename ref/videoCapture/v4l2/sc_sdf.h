#ifndef _SC_SDF_H_
#define _SC_SDF_H_


#define CLEAR(x) memset(&(x), 0, sizeof(x))

//define the function execute status
#define BASICERROR			1
#define SUCCESS				0
#define PARAMERROR			BASICERROR<<1
#define MEMORYERROR			BASICERROR<<2
#define FILEERROR			BASICERROR<<3

#define	FPS		15
#define FRAME_WIDTH		640
#define FRAME_HEIGHT	480

#define FILELENGTH		255
#define YUVFILENAME		"video.yuv"
#define H264FILENAME	"video.h264"

#define MTU 1400

typedef unsigned char		uint8;
typedef unsigned short int 	uint16;
typedef	unsigned int 		uint32;
typedef unsigned long long	uint64;
typedef char				int8;
typedef short int			int16;
typedef int					int32;
typedef long long			int64;

typedef unsigned char uint8_t;

#endif
