#include "triangleApp.h"
#include "tictoc.h"

#include<iostream>
#include"sc_config.h"
using namespace std;

//The first device we want to open
int dev = 0;

triangleApp::~triangleApp(){
	if (ctimer){
        ctimer->EndTimer();
		delete ctimer;
	}
	else
		printf("CTimer already null\n");
}
//empty constructor
triangleApp::triangleApp(){
	ctimer = new CTimer();
	if (ctimer == NULL){
		std::cout << "error to create CTimer " << std::endl;
	}

}

void triangleApp::init(){
	tic();  printf("[init @%f]\n", toc(0));
	int test_w = 640;
	int test_h = 480;
	int test_fps = 25;
	//uncomment for silent setup
	//videoInput::setVerbose(false);

	//uncomment for multithreaded setup
	//videoInput::setComMultiThreaded(true);

	//optional static function to list devices
	//for silent listDevices use listDevices(true);
	int numDevices = videoInput::listDevices();

	//you can also now get the device list as a vector of strings
	std::vector <std::string> list = videoInput::getDeviceList();
	for (int i = 0; i < list.size(); i++){
		printf("[%i] device is %s\n", i, list[i].c_str());
	}

	//we allocate our openGL texture objects
	//we give them a ma size of 1024 by 1024 pixels
	IT = new imageTexture(2048, 2048, GL_RGB);
	if(DEV2)IT2 = new imageTexture(2048, 2048, GL_RGB);

	//by default we use a callback method
	//this updates whenever a new frame
	//arrives if you are only ocassionally grabbing frames
	//you might want to set this to false as the callback caches the last
	//frame for performance reasons.
	VI.setUseCallback(true);

	//try and setup device with id 0 and id 1
	//if only one device is found the second
	//setupDevice should return false

	//if you want to capture at a different frame rate (default is 30)
	//specify it here, you are not guaranteed to get this fps though.
	VI.setIdealFramerate(dev, test_fps);

	//we can specifiy the dimensions we want to capture at
	//if those sizes are not possible VI will look for the next nearest matching size
	//VI.setRequestedMediaSubType((int)MEDIASUBTYPE_MJPG);
	VI.setupDevice(dev, test_w, test_h, VI_COMPOSITE);
    if(DEV2)	VI.setupDevice(dev + 1, test_w, test_h, VI_COMPOSITE);
    /*/使用widows的对话框来设置摄像头的格式等。可
        http://blog.csdn.net/jia_zhengshen/article/details/9980495
    */
    VI.showSettingsWindow(dev);//
	//once the device is setup you can try and
	//set the format - this is useful if your device
	//doesn't remember what format you set it to
	//VI.setFormat(dev, VI_NTSC_M);					//optional set the format

	//we allocate our buffer based on the number
	//of pixels in each frame - this will be width * height * 3
	frame = new unsigned char[VI.getSize(dev)];
	if(DEV2)frame2 = new unsigned char[VI.getSize(dev + 1)];

}
int test_count = 0;
int first = 1;

TicTocTimer      clock = tic();
double test_time = 0;
void triangleApp::idle(){
    /*
#if 1
	if (first){
#if 0
		test_time  = toc(&clock);
		printf("start time %f seconds.\n",test_time);
#else
		ctimer->StartTimer(1000);//1000ms
#endif
		first = 0;
	}
#endif
*/
	//check to see if we have got a new frame
	if (VI.isFrameNew(dev))
	{
        #if 0
#if 0
		printf("got [%d]",test_count++);
#else /*定时器*/
#if 0
		//printf("Elapsed time %f seconds.\n",toc(&clock)); // prints time since last tic()
		test_count++;
		double now=toc(&clock) ;
		printf("now [%f] last[%f] rate[%f]\n",now,clock.last,clock.rate);
		if((now- test_time) >= 1.000000){
			printf("###got [%d]\n",test_count);
			test_count=0;
			first=1;
		}
#else /*CTimer*/
		ctimer->addcount();
#endif

#endif/*定时器*/
        #endif
        //we get the pixels by passing in out buffer which gets filled
		VI.getPixels(dev, frame, true);

		//we then load them into our texture
		IT->loadImageData(frame, VI.getWidth(dev), VI.getHeight(dev), GL_RGB);
	}
	/*这个不要会不会有什么影响啊*/
	//check to see if we have got a new frame
	if(DEV2)
	if (VI.isFrameNew(dev + 1))
	{
		//here we are directly return the pixels into our texture
		//use VI.getWidth getHeight etc so that you don't get a crash
		IT2->loadImageData(VI.getPixels(dev + 1, true), VI.getWidth(dev + 1), VI.getHeight(dev + 1), GL_RGB);
	}
}


void triangleApp::draw(){

	setupScreen();
	IT->renderTexture(0, 0, VI.getWidth(dev), VI.getHeight(dev));
	if(DEV2)IT2->renderTexture(VI.getWidth(dev), 0, VI.getWidth(dev + 1), VI.getHeight(dev + 1));
}

void triangleApp::keyDown(char c){

	//some options hooked up to key commands
	if (c == 'S')VI.showSettingsWindow(dev);
	if(DEV2)if (c == 'D')VI.showSettingsWindow(dev + 1);

	if (c == 'R')VI.restartDevice(dev);
	if(DEV2)if (c == 'T')VI.restartDevice(dev + 1);

	if (c == '1')VI.setVideoSettingCameraPct(0, VI.propExposure, 0.1, 2);
	if (c == '2')VI.setVideoSettingCameraPct(0, VI.propExposure, 0.9, 2);

	if (c == '5')VI.setVideoSettingFilterPct(0, VI.propWhiteBalance, 0.12, 2);
	if (c == '6')VI.setVideoSettingFilterPct(0, VI.propWhiteBalance, 0.88, 2);

	if (c == 'Q')
	{
		VI.stopDevice(dev);
		if(DEV2)VI.stopDevice(dev + 1);
	}
}

void triangleApp::mouseMove(float x, float y){


}

void triangleApp::mouseDrag(float x, float y){

}

void triangleApp::mouseDown(float x, float y, int button){

}

void triangleApp::mouseUp(float x, float y, int button){

}


