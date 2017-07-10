C++ Video capture and encoding library
====================================

As I remembered that video encoding using FFMPEG wasn't easy near year 2010 due to compiling FFMPEG library on Windows platform was not easy.
Now it is easy to utilize ffmpeg along with OpenCV.  
This C++ library is to encoding webcam input to mpeg file.

## Build Environment
* Visual Studio 2013
* ffmpeg-3.3.2-win64-dev.zip(for build)
* ffmpeg-3.3.2-win64-shared.zip(for run)
* opencv-3.2.0-vc14.exe


This repository only includes source code, you need to download library by yourself.

## Setup Build Environment
The project file references ffmpeg and opencv library through environ variables.  
* Set the environment like following:
```cmd
setx -m OPENCV_DIR <opencv installation directory> 
setx -m FFMPEG_DIR <ffmpeg installation directory>
```
* My environments is following:
```cmd
setx -m OPENCV_DIR C:\opencv\build\x64\vc14 
setx -m FFMPEG_DIR C:\ffmpeg\ffmpeg-3.3.2-win64-dev 
```

### Compiling projects
* select x84-bit build-environment

## License

This software is free to use.  

Copyright(c) 2017 hillkim7@gmail.com
