gcc -o armrec camera.c CameraSource.c V4L2.c base/utils.c  media/videorecorder.c recorder.c -I. -I./vencoder/ -lpthread
