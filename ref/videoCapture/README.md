videoCapture
============

1. capture video from some device BY V4L2
2. add a project to test Android MediaRecorder 20140928,thanks to open source!
3. 20151030 三个线程（定时、采集和保存）齐备，使用了回调函数。需要增加一个突然要退出，清空队列缓冲，释放分配的frame内存的函数。

学习笔记
