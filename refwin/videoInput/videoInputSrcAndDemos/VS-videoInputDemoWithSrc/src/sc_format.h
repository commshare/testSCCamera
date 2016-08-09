/*
  支持mjpeg  bgr24  yuv420p
  MEDIASUBTYPE_I420;//MEDIASUBTYPE_RGB24;
当前的camera是     Logitech HD Pro Webcam C920


（2）  查询能力如下：
J:\media\XLIVE\windows\videoInput\videoInputSrcAndDemos\VS-videoInputDemoWithSrc\Debug>ffmpeg -f dshow -list_options true -i video="Logitech HD Pro Webcam C920"
ffmpeg version N-80129-ga1953d4 Copyright (c) 2000-2016 the FFmpeg developers
  built with gcc 5.3.0 (GCC)
  configuration: --enable-gpl --enable-version3 --disable-w32threads --enable-nvenc --enable-avisynth --enable-bzlib --enable-fontconfig --enable-frei0r --enable-gnutls --enable-iconv --enable-libass --enable-libbluray --enable-libbs2b --enable-libcaca --enable-libfreetype --enable-libgme --enable-libgsm --enable-libilbc --enable-libmodplug --enable-libmfx --enable-libmp3lame --enable-libopencore-amrnb --enable-libopencore-amrwb --enable-libopenjpeg --enable-libopus --enable-librtmp --enable-libschroedinger --enable-libsnappy --enable-libsoxr --enable-libspeex --enable-libtheora --enable-libtwolame --enable-libvidstab --enable-libvo-amrwbenc --enable-libvorbis --enable-libvpx --enable-libwavpack --enable-libwebp --enable-libx264 --enable-libx265 --enable-libxavs --enable-libxvid --enable-libzimg --enable-lzma --enable-decklink --enable-zlib
  libavutil      55. 24.100 / 55. 24.100
  libavcodec     57. 43.100 / 57. 43.100
  libavformat    57. 37.101 / 57. 37.101
  libavdevice    57.  0.101 / 57.  0.101
  libavfilter     6. 46.100 /  6. 46.100
  libswscale      4.  1.100 /  4.  1.100
  libswresample   2.  0.101 /  2.  0.101
  libpostproc    54.  0.100 / 54.  0.100
[dshow @ 0000000000cb6ee0] DirectShow video device options (from video devices)
[dshow @ 0000000000cb6ee0]  Pin "鎹曡幏" (alternative pin name "0")
[dshow @ 0000000000cb6ee0]   vcodec=mjpeg  min s=640x480 fps=5 max s=640x480 fps=30
[dshow @ 0000000000cb6ee0]   vcodec=mjpeg  min s=160x90 fps=5 max s=160x90 fps=30
[dshow @ 0000000000cb6ee0]   vcodec=mjpeg  min s=160x120 fps=5 max s=160x120 fps=30
[dshow @ 0000000000cb6ee0]   vcodec=mjpeg  min s=176x144 fps=5 max s=176x144 fps=30
[dshow @ 0000000000cb6ee0]   vcodec=mjpeg  min s=320x180 fps=5 max s=320x180 fps=30
[dshow @ 0000000000cb6ee0]   vcodec=mjpeg  min s=320x240 fps=5 max s=320x240 fps=30
[dshow @ 0000000000cb6ee0]   vcodec=mjpeg  min s=352x288 fps=5 max s=352x288 fps=30
[dshow @ 0000000000cb6ee0]   vcodec=mjpeg  min s=432x240 fps=5 max s=432x240 fps=30
[dshow @ 0000000000cb6ee0]   vcodec=mjpeg  min s=640x360 fps=5 max s=640x360 fps=30
[dshow @ 0000000000cb6ee0]   vcodec=mjpeg  min s=800x448 fps=5 max s=800x448 fps=30
[dshow @ 0000000000cb6ee0]   vcodec=mjpeg  min s=800x600 fps=5 max s=800x600 fps=30
[dshow @ 0000000000cb6ee0]   vcodec=mjpeg  min s=864x480 fps=5 max s=864x480 fps=30
[dshow @ 0000000000cb6ee0]   vcodec=mjpeg  min s=960x720 fps=5 max s=960x720 fps=30
[dshow @ 0000000000cb6ee0]   vcodec=mjpeg  min s=1024x576 fps=5 max s=1024x576 fps=30
[dshow @ 0000000000cb6ee0]   vcodec=mjpeg  min s=1280x720 fps=5 max s=1280x720 fps=30
[dshow @ 0000000000cb6ee0]   vcodec=mjpeg  min s=1600x896 fps=5 max s=1600x896 fps=30
[dshow @ 0000000000cb6ee0]   vcodec=mjpeg  min s=1920x1080 fps=5 max s=1920x1080 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=bgr24  min s=640x480 fps=5 max s=640x480 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=bgr24  min s=160x90 fps=5 max s=160x90 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=bgr24  min s=160x120 fps=5 max s=160x120 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=bgr24  min s=176x144 fps=5 max s=176x144 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=bgr24  min s=320x180 fps=5 max s=320x180 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=bgr24  min s=320x240 fps=5 max s=320x240 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=bgr24  min s=352x288 fps=5 max s=352x288 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=bgr24  min s=432x240 fps=5 max s=432x240 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=bgr24  min s=640x360 fps=5 max s=640x360 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=bgr24  min s=800x448 fps=5 max s=800x448 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=bgr24  min s=800x600 fps=5 max s=800x600 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=bgr24  min s=864x480 fps=5 max s=864x480 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=bgr24  min s=960x720 fps=5 max s=960x720 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=bgr24  min s=1024x576 fps=5 max s=1024x576 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=bgr24  min s=1280x720 fps=5 max s=1280x720 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=bgr24  min s=1600x896 fps=5 max s=1600x896 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=bgr24  min s=1920x1080 fps=5 max s=1920x1080 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=bgr24  min s=2304x1296 fps=2 max s=2304x1296 fps=2
[dshow @ 0000000000cb6ee0]   pixel_format=bgr24  min s=2304x1536 fps=2 max s=2304x1536 fps=2
[dshow @ 0000000000cb6ee0]   pixel_format=yuv420p  min s=640x480 fps=5 max s=640x480 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=yuv420p  min s=160x90 fps=5 max s=160x90 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=yuv420p  min s=160x120 fps=5 max s=160x120 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=yuv420p  min s=176x144 fps=5 max s=176x144 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=yuv420p  min s=320x180 fps=5 max s=320x180 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=yuv420p  min s=320x240 fps=5 max s=320x240 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=yuv420p  min s=352x288 fps=5 max s=352x288 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=yuv420p  min s=432x240 fps=5 max s=432x240 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=yuv420p  min s=640x360 fps=5 max s=640x360 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=yuv420p  min s=800x448 fps=5 max s=800x448 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=yuv420p  min s=800x600 fps=5 max s=800x600 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=yuv420p  min s=864x480 fps=5 max s=864x480 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=yuv420p  min s=960x720 fps=5 max s=960x720 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=yuv420p  min s=1024x576 fps=5 max s=1024x576 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=yuv420p  min s=1280x720 fps=5 max s=1280x720 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=yuv420p  min s=1600x896 fps=5 max s=1600x896 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=yuv420p  min s=1920x1080 fps=5 max s=1920x1080 fps=30
[dshow @ 0000000000cb6ee0]   pixel_format=yuv420p  min s=2304x1296 fps=2 max s=2304x1296 fps=2
[dshow @ 0000000000cb6ee0]   pixel_format=yuv420p  min s=2304x1536 fps=2 max s=2304x1536 fps=2
video=Logitech HD Pro Webcam C920: Immediate exit requested

J:\media\XLIVE\windows\videoInput\videoInputSrcAndDemos\VS-videoInputDemoWithSrc\Debug>


*/


/*   http://www.cnblogs.com/azraelly/archive/2013/01/13/2858858.html RGB与YUV----摘自《DirectShow实务精选》
按照这句话，RGB24就是bgr24啊
¨ RGB24使用24位来表示一个像素，RGB分量都用8位表示，取值范围为0-255。注意在内存中RGB各分量的排列顺序为：BGR BGR BGR
*/


/*  Logitech Webcam C930e

J:\MYSELF\testSCCamera\refwin\videoInput\videoInputSrcAndDemos\VS-videoInputDemoWithSrc\Debug>ffmpeg -f dshow -list_options true -i video="Logitech Webcam C930e"
ffmpeg version N-80129-ga1953d4 Copyright (c) 2000-2016 the FFmpeg developers
  built with gcc 5.3.0 (GCC)
  configuration: --enable-gpl --enable-version3 --disable-w32threads --enable-nvenc --enable-avisynth --enable-bzlib --enable-fontconfig --enable-frei0r --enable-gnutls --enable-iconv --enable-libass --enable-libbluray --enabl
e-libbs2b --enable-libcaca --enable-libfreetype --enable-libgme --enable-libgsm --enable-libilbc --enable-libmodplug --enable-libmfx --enable-libmp3lame --enable-libopencore-amrnb --enable-libopencore-amrwb --enable-libopenjpe
g --enable-libopus --enable-librtmp --enable-libschroedinger --enable-libsnappy --enable-libsoxr --enable-libspeex --enable-libtheora --enable-libtwolame --enable-libvidstab --enable-libvo-amrwbenc --enable-libvorbis --enable-
libvpx --enable-libwavpack --enable-libwebp --enable-libx264 --enable-libx265 --enable-libxavs --enable-libxvid --enable-libzimg --enable-lzma --enable-decklink --enable-zlib
  libavutil      55. 24.100 / 55. 24.100
  libavcodec     57. 43.100 / 57. 43.100
  libavformat    57. 37.101 / 57. 37.101
  libavdevice    57.  0.101 / 57.  0.101
  libavfilter     6. 46.100 /  6. 46.100
  libswscale      4.  1.100 /  4.  1.100
  libswresample   2.  0.101 /  2.  0.101
  libpostproc    54.  0.100 / 54.  0.100
[dshow @ 0000000000166f40] DirectShow video device options (from video devices)
[dshow @ 0000000000166f40]  Pin "鎹曡幏" (alternative pin name "0")
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=640x480 fps=5 max s=640x480 fps=30
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=640x480 fps=5 max s=640x480 fps=30
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=160x120 fps=5 max s=160x120 fps=30
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=160x120 fps=5 max s=160x120 fps=30
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=176x144 fps=5 max s=176x144 fps=30
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=176x144 fps=5 max s=176x144 fps=30
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=320x180 fps=5 max s=320x180 fps=30
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=320x180 fps=5 max s=320x180 fps=30
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=320x240 fps=5 max s=320x240 fps=30
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=320x240 fps=5 max s=320x240 fps=30
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=352x288 fps=5 max s=352x288 fps=30
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=352x288 fps=5 max s=352x288 fps=30
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=424x240 fps=5 max s=424x240 fps=30
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=424x240 fps=5 max s=424x240 fps=30
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=480x270 fps=5 max s=480x270 fps=30
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=480x270 fps=5 max s=480x270 fps=30
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=640x360 fps=5 max s=640x360 fps=30
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=640x360 fps=5 max s=640x360 fps=30
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=800x448 fps=5 max s=800x448 fps=30
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=800x448 fps=5 max s=800x448 fps=30
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=800x600 fps=5 max s=800x600 fps=24
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=800x600 fps=5 max s=800x600 fps=24
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=848x480 fps=5 max s=848x480 fps=30
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=848x480 fps=5 max s=848x480 fps=30
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=960x540 fps=5 max s=960x540 fps=15
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=960x540 fps=5 max s=960x540 fps=15
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=1024x576 fps=5 max s=1024x576 fps=15
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=1024x576 fps=5 max s=1024x576 fps=15
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=1280x720 fps=5 max s=1280x720 fps=10
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=1280x720 fps=5 max s=1280x720 fps=10  这个格式最多10fps
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=1600x896 fps=5 max s=1600x896 fps=7.5
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=1600x896 fps=5 max s=1600x896 fps=7.5
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=1920x1080 fps=5 max s=1920x1080 fps=5
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=1920x1080 fps=5 max s=1920x1080 fps=5
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=2304x1296 fps=2 max s=2304x1296 fps=2
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=2304x1296 fps=2 max s=2304x1296 fps=2
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=2304x1536 fps=2 max s=2304x1536 fps=2
[dshow @ 0000000000166f40]   pixel_format=yuyv422  min s=2304x1536 fps=2 max s=2304x1536 fps=2
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=640x480 fps=5 max s=640x480 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=640x480 fps=5 max s=640x480 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=160x120 fps=5 max s=160x120 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=160x120 fps=5 max s=160x120 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=176x144 fps=5 max s=176x144 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=176x144 fps=5 max s=176x144 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=320x180 fps=5 max s=320x180 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=320x180 fps=5 max s=320x180 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=320x240 fps=5 max s=320x240 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=320x240 fps=5 max s=320x240 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=352x288 fps=5 max s=352x288 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=352x288 fps=5 max s=352x288 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=424x240 fps=5 max s=424x240 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=424x240 fps=5 max s=424x240 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=480x270 fps=5 max s=480x270 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=480x270 fps=5 max s=480x270 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=640x360 fps=5 max s=640x360 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=640x360 fps=5 max s=640x360 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=800x448 fps=5 max s=800x448 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=800x448 fps=5 max s=800x448 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=800x600 fps=5 max s=800x600 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=800x600 fps=5 max s=800x600 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=848x480 fps=5 max s=848x480 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=848x480 fps=5 max s=848x480 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=960x540 fps=5 max s=960x540 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=960x540 fps=5 max s=960x540 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=1024x576 fps=5 max s=1024x576 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=1024x576 fps=5 max s=1024x576 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=1280x720 fps=5 max s=1280x720 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=1280x720 fps=5 max s=1280x720 fps=30  这个格式大啊
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=1600x896 fps=5 max s=1600x896 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=1600x896 fps=5 max s=1600x896 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=1920x1080 fps=5 max s=1920x1080 fps=30
[dshow @ 0000000000166f40]   vcodec=mjpeg  min s=1920x1080 fps=5 max s=1920x1080 fps=30
video=Logitech Webcam C930e: Immediate exit requested
*/


/*
表2.3 常见的RGB和YUV格式

GUID 格式描述
MEDIASUBTYPE_RGB1 2色，每个像素用1位表示，需要调色板
MEDIASUBTYPE_RGB4 16色，每个像素用4位表示，需要调色板
MEDIASUBTYPE_RGB8 256色，每个像素用8位表示，需要调色板
MEDIASUBTYPE_RGB565 每个像素用16位表示，RGB分量分别使用5位、6位、5位
MEDIASUBTYPE_RGB555 每个像素用16位表示，RGB分量都使用5位（剩下的1位不用）
MEDIASUBTYPE_RGB24 每个像素用24位表示，RGB分量各使用8位
MEDIASUBTYPE_RGB32 每个像素用32位表示，RGB分量各使用8位（剩下的8位不用）
MEDIASUBTYPE_ARGB32 每个像素用32位表示，RGB分量各使用8位（剩下的8位用于表示Alpha通道值）
MEDIASUBTYPE_YUY2 YUY2格式，以4:2:2方式打包
MEDIASUBTYPE_YUYV YUYV格式（实际格式与YUY2相同）
MEDIASUBTYPE_YVYU YVYU格式，以4:2:2方式打包
MEDIASUBTYPE_UYVY UYVY格式，以4:2:2方式打包
MEDIASUBTYPE_AYUV 带Alpha通道的4:4:4 YUV格式
MEDIASUBTYPE_Y41P Y41P格式，以4:1:1方式打包
MEDIASUBTYPE_Y411 Y411格式（实际格式与Y41P相同）
MEDIASUBTYPE_Y211 Y211格式
MEDIASUBTYPE_IF09 IF09格式
MEDIASUBTYPE_IYUV IYUV格式
MEDIASUBTYPE_YV12 YV12格式
MEDIASUBTYPE_YVU9 YVU9格式

*/



/* 不得不爱

J:\MYSELF\testSCCamera\refwin\videoInput\SI>ffmpeg -list_devices true -f dshow -i dummy
ffmpeg version N-80129-ga1953d4 Copyright (c) 2000-2016 the FFmpeg developers
  built with gcc 5.3.0 (GCC)
  configuration: --enable-gpl --enable-version3 --disable-w32threads --enable-nvenc --enable-avisynth --enable-bzlib --enable-fontconfig --enable-frei0r --enable-gnutls --enable-iconv --enable-libass --enable-libbluray --enable-libbs2b --enable-libcaca --enable-libfreetype --enable-libgme --enable-libgsm --enable-libilbc --enable-libmodplug --enable-libmfx --enable-libmp3lame --enable-libopencore-amrnb --enable-libopencore-amrwb --enable-libopenjpeg --enable-libopus --enable-librtmp --enable-libschroedinger --enable-libsnappy --enable-libsoxr --enable-libspeex --enable-libtheora --enable-libtwolame --enable-libvidstab --enable-libvo-amrwbenc --enable-libvorbis --enable-libvpx --enable-libwavpack --enable-libwebp --enable-libx264 --enable-libx265 --enable-libxavs --enable-libxvid --enable-libzimg --enable-lzma --enable-decklink --enable-zlib
  libavutil      55. 24.100 / 55. 24.100
  libavcodec     57. 43.100 / 57. 43.100
  libavformat    57. 37.101 / 57. 37.101
  libavdevice    57.  0.101 / 57.  0.101
  libavfilter     6. 46.100 /  6. 46.100
  libswscale      4.  1.100 /  4.  1.100
  libswresample   2.  0.101 /  2.  0.101
  libpostproc    54.  0.100 / 54.  0.100
[dshow @ 0000000001076f20] DirectShow video devices (some may be both video and audio devices)
[dshow @ 0000000001076f20]  "BudeBuai-6Plus"
[dshow @ 0000000001076f20]     Alternative name "@device_pnp_\\?\usb#vid_058f&pid_0688&mi_00#6&2dbf07c9&0&0000#{65e8773d-8f56-11d0-a3b9-00a0c9223196}\global"
[dshow @ 0000000001076f20] DirectShow audio devices
[dshow @ 0000000001076f20]  "楹﹀厠椋?(Realtek High Definition Audio)"
[dshow @ 0000000001076f20]     Alternative name "@device_cm_{33D9A762-90C8-11D0-BD43-00A0C911CE86}\wave_{4F4D2A85-C542-4AFF-8E54-935785B76519}"
[dshow @ 0000000001076f20]  "Realtek Digital Input (Realtek High Definition Audio)"
[dshow @ 0000000001076f20]     Alternative name "@device_cm_{33D9A762-90C8-11D0-BD43-00A0C911CE86}\wave_{88B242C3-5D3C-44AA-ADC0-9477C42C6620}"
dummy: Immediate exit requested

J:\MYSELF\testSCCamera\refwin\videoInput\SI>ffmpeg -f dshow -list_options true -i video="BudeBuai-6Plus"
ffmpeg version N-80129-ga1953d4 Copyright (c) 2000-2016 the FFmpeg developers
  built with gcc 5.3.0 (GCC)
  configuration: --enable-gpl --enable-version3 --disable-w32threads --enable-nvenc --enable-avisynth --enable-bzlib --enable-fontconfig --enable-frei0r --enable-gnutls --enable-iconv --enable-libass --enable-libbluray --enable-libbs2b --enable-libcaca --enable-libfreetype --enable-libgme --enable-libgsm --enable-libilbc --enable-libmodplug --enable-libmfx --enable-libmp3lame --enable-libopencore-amrnb --enable-libopencore-amrwb --enable-libopenjpeg --enable-libopus --enable-librtmp --enable-libschroedinger --enable-libsnappy --enable-libsoxr --enable-libspeex --enable-libtheora --enable-libtwolame --enable-libvidstab --enable-libvo-amrwbenc --enable-libvorbis --enable-libvpx --enable-libwavpack --enable-libwebp --enable-libx264 --enable-libx265 --enable-libxavs --enable-libxvid --enable-libzimg --enable-lzma --enable-decklink --enable-zlib
  libavutil      55. 24.100 / 55. 24.100
  libavcodec     57. 43.100 / 57. 43.100
  libavformat    57. 37.101 / 57. 37.101
  libavdevice    57.  0.101 / 57.  0.101
  libavfilter     6. 46.100 /  6. 46.100
  libswscale      4.  1.100 /  4.  1.100
  libswresample   2.  0.101 /  2.  0.101
  libpostproc    54.  0.100 / 54.  0.100
[dshow @ 0000000000026f20] DirectShow video device options (from video devices)
[dshow @ 0000000000026f20]  Pin "鎹曡幏" (alternative pin name "0")
[dshow @ 0000000000026f20]   vcodec=mjpeg  min s=640x480 fps=30 max s=640x480 fps=30
[dshow @ 0000000000026f20]   vcodec=mjpeg  min s=640x480 fps=30 max s=640x480 fps=30
[dshow @ 0000000000026f20]   vcodec=mjpeg  min s=1280x720 fps=30 max s=1280x720 fps=30
[dshow @ 0000000000026f20]   vcodec=mjpeg  min s=1280x720 fps=30 max s=1280x720 fps=30
[dshow @ 0000000000026f20]   vcodec=mjpeg  min s=320x240 fps=30 max s=320x240 fps=30
[dshow @ 0000000000026f20]   vcodec=mjpeg  min s=320x240 fps=30 max s=320x240 fps=30
[dshow @ 0000000000026f20]   vcodec=mjpeg  min s=800x600 fps=30 max s=800x600 fps=30
[dshow @ 0000000000026f20]   vcodec=mjpeg  min s=800x600 fps=30 max s=800x600 fps=30
[dshow @ 0000000000026f20]   pixel_format=yuyv422  min s=640x480 fps=30 max s=640x480 fps=30
[dshow @ 0000000000026f20]   pixel_format=yuyv422  min s=640x480 fps=30 max s=640x480 fps=30
[dshow @ 0000000000026f20]   pixel_format=yuyv422  min s=320x240 fps=30 max s=320x240 fps=30
[dshow @ 0000000000026f20]   pixel_format=yuyv422  min s=320x240 fps=30 max s=320x240 fps=30
[dshow @ 0000000000026f20]   pixel_format=yuyv422  min s=1280x720 fps=10 max s=1280x720 fps=10
[dshow @ 0000000000026f20]   pixel_format=yuyv422  min s=1280x720 fps=10 max s=1280x720 fps=10
video=BudeBuai-6Plus: Immediate exit requested

J:\MYSELF\testSCCamera\refwin\videoInput\SI>


*/
