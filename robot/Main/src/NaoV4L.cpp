#include "NaoV4L.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <cstring>
#include <sys/mman.h>
#include <bn/i2c/i2c-dev.h>
#include <Factory/Factory.h>

NaoV4L::NaoV4L()
: config(Factory::getDefaultFactory().getConfigFile()),
  log(Factory::getDefaultFactory().getLog()),
  NUM_FRAMES_TO_IGNORE_WHEN_SWITCHING_CAMERAS(config.getInt("camera/numFramesToIgnoreWhenSwitchingCameras", 1)),
  numFramesToIgnore(0)
{
    fdCamera = -1;
    fdI2C = -1;
    for ( int i = 0; i < NUM_FRAME_BUFFERS; i++ )
    {
        buffers[i] = MAP_FAILED;
        bufferLength[i] = -1;
    }
    lastBuffer = 0;
    frameCount = 0;
    memset(rawImage,0,IMAGESIZE);
}

NaoV4L::~NaoV4L() 
{
}

bool NaoV4L::initCam( bool bottomCam )
{
    bool ret = true;
    int result = -1;
    std::cout << "Opening camera device " << cameraDevice << " ......";
    fdCamera = open( cameraDevice.c_str(), O_RDWR );
    if ( fdCamera != -1 )
    {
        std::cout << "DONE" << std::endl;
    }
    else
    {
        std::cout << "ERROR" << std::endl;
        ret = false;
    }
    
    std::cout << "Setting the resolution..................";

    struct v4l2_control control;
    memset(&control, 0, sizeof(control));
    /*
    if ( !bottomCam )
    {
        control.id = 0x980919;
    }
    else
    {
        control.id = 0x9963809;
    }
    */
    control.id = V4L2_CID_CAM_INIT;
    control.value = 0;
    result = !ioctl(fdCamera, VIDIOC_S_CTRL, &control);    
     
    //set resolution
    v4l2_std_id esid0 = ( imageWidth == 320 ? 0x04000000UL : 0x08000000UL); // should be  0x08000000UL for 640x480
    result &= !ioctl(fdCamera, VIDIOC_S_STD, &esid0);
    
    if ( result )
    {
        std::cout << "DONE" << std::endl;
    }
    else
    {
        std::cout << "ERROR" << std::endl;
        ret = false;
    }

    //set image format
    std::cout << "Setting the image format(" << imageWidth << "X" << imageHeight << ").......";
    struct v4l2_format fmt;
    memset(&fmt, 0, sizeof(struct v4l2_format));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = imageWidth;
    fmt.fmt.pix.height = imageHeight;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;
    result = ioctl(fdCamera, VIDIOC_S_FMT, &fmt);
    if ( fmt.fmt.pix.sizeimage == imageSize )
    {
        std::cout << "DONE" << std::endl;
    }
    else
    {
        std::cout << "ERROR" << std::endl;
        ret = false;
    }

    //set frame rate
    std::cout << "Setting the frame rate(" << fps << ")..............";
    struct v4l2_streamparm sFps;
    memset(&sFps, 0, sizeof(struct v4l2_streamparm));
    sFps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(fdCamera, VIDIOC_G_PARM, &sFps);
    sFps.parm.capture.timeperframe.numerator = 1;
    sFps.parm.capture.timeperframe.denominator = fps;    
    sFps.parm.capture.capability = V4L2_CAP_TIMEPERFRAME;
    result = ioctl(fdCamera, VIDIOC_S_PARM, &sFps);

    if ( result != -1 )
    {
        std::cout << "DONE" << std::endl;
    }
    else
    {
        std::cout << "ERROR" << std::endl;
        ret = false;
    }

    if ( bottomCam )
    {
        setCameraParams(bottomParams);
    }
    else
    {
        setCameraParams(topParams);
    }
    std::cout << "Closing camera device " << cameraDevice << " ......";
    result = close(fdCamera);
    
    if ( result != -1 )
    {
        std::cout << "DONE" << std::endl;
        fdCamera = -1;
    }
    else
    {
        std::cout << "ERROR" << std::endl;
        ret = false;
    }

    return ret;
}

bool NaoV4L::initialize()
{
    bool ret = true;
    int result = -1;

    cameraDevice = config.getString("camera/deviceName");
    i2cDevice = config.getString("camera/i2cdeviceName");
    imageWidth = config.getInt("camera/imageWidth",320);
    imageHeight = config.getInt("camera/imageHeight",240);
    imageSize = imageWidth*imageHeight*2;
    fps = config.getInt("camera/fps",30);

    bottomCamera = config.getBool("camera/defaultCamera",true);

    topParams.autoGain = config.getInt("camera/top/autoGain",0);
    topParams.autoWhiteBalance = config.getInt("camera/top/autoWhiteBalance",0);
    topParams.autoExposure = config.getInt("camera/top/autoExposure",0);
    topParams.exposure = config.getInt("camera/top/exposure",100);
    topParams.gain = config.getInt("camera/top/gain",150);
    topParams.blueChroma = config.getInt("camera/top/blueChroma",128);
    topParams.redChroma = config.getInt("camera/top/redChroma",101);
    topParams.brightness = config.getInt("camera/top/brightness",81);
    topParams.contrast = config.getInt("camera/top/contrast",50);
    topParams.saturation = config.getInt("camera/top/saturation",82);
    topParams.hue = config.getInt("camera/top/hue",0);
    topParams.exposureCorrection = config.getInt("camera/top/exposureCorrection",0);
    topParams.hFlip = config.getInt("camera/top/hFlip",1);
    topParams.vFlip = config.getInt("camera/top/vFlip",1);


    bottomParams.autoGain = config.getInt("camera/bottom/autoGain",topParams.autoGain);
    bottomParams.autoWhiteBalance = config.getInt("camera/bottom/autoWhiteBalance",topParams.autoWhiteBalance);
    bottomParams.autoExposure = config.getInt("camera/bottom/autoExposure",topParams.autoExposure);
    bottomParams.exposure = config.getInt("camera/bottom/exposure",topParams.exposure);
    bottomParams.gain = config.getInt("camera/bottom/gain",topParams.gain);
    bottomParams.blueChroma = config.getInt("camera/bottom/blueChroma",topParams.blueChroma);
    bottomParams.redChroma = config.getInt("camera/bottom/redChroma",topParams.redChroma);
    bottomParams.brightness = config.getInt("camera/bottom/brightness",topParams.brightness);
    bottomParams.contrast = config.getInt("camera/bottom/contrast",topParams.contrast);
    bottomParams.saturation = config.getInt("camera/bottom/saturation",topParams.saturation);
    bottomParams.hue = config.getInt("camera/bottom/hue",topParams.hue);
    bottomParams.exposureCorrection = config.getInt("camera/bottom/exposureCorrection",topParams.exposureCorrection);
    bottomParams.hFlip = config.getInt("camera/bottom/hFlip",topParams.hFlip);
    bottomParams.vFlip = config.getInt("camera/bottom/vFlip",topParams.vFlip);


    std::cout << "==================================================" << std::endl;
    std::cout << "=              NaoV4L::initialize()              =" << std::endl;
    std::cout << "==================================================" << std::endl;
    
    std::cout << "Opening I2C device " << i2cDevice << " ..........";
    fdI2C = open( i2cDevice.c_str(), O_RDWR );
    if ( fdI2C != -1 )
    {
        std::cout << "DONE" << std::endl;
    }
    else
    {
        std::cout << "ERROR" << std::endl;
        ret = false;
    }
    
    
    std::cout << "Initializing the BOTTOM camera" << std::endl;
    setCamera(true);
    initCam(true);
    
    std::cout << "Initializing the TOP camera" << std::endl;
    setCamera(false);
    initCam(false);


    std::cout << "Opening camera device " << cameraDevice << " ......";
    fdCamera = open( cameraDevice.c_str(), O_RDWR );
    if ( fdCamera != -1 )
    {
        std::cout << "DONE" << std::endl;
    }
    else
    {
        std::cout << "ERROR" << std::endl;
        ret = false;
    }


    /*
    setCamera(true);
    std::cout << "Setting BOTTOM camera paramaters...............";
    setCameraParams(bottomParams);
    setCamera(false);
    std::cout << "Setting TOP camera paramaters...............";
    setCameraParams(topParams);
    */

    
    // request buffers
    std::cout << "Initializing buffers....................";
    struct v4l2_requestbuffers rb;
    memset(&rb, 0, sizeof(struct v4l2_requestbuffers));
    rb.count = NUM_FRAME_BUFFERS;
    rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    rb.memory = V4L2_MEMORY_MMAP;
    result = ioctl(fdCamera, VIDIOC_REQBUFS, &rb);

    // map the buffers
    buffer = static_cast<struct v4l2_buffer*>(calloc(1, sizeof(struct v4l2_buffer)));
    for(int i = 0; i < NUM_FRAME_BUFFERS; i++)
    {
        buffer->index = i;
        buffer->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buffer->memory = V4L2_MEMORY_MMAP;
        result = ioctl(fdCamera, VIDIOC_QUERYBUF, buffer);
        bufferLength[i] = buffer->length;
        //buffers[i] = mmap(0, buffer->length, PROT_READ | PROT_WRITE, MAP_SHARED, fdCamera, buffer->m.offset);
        buffers[i] = mmap(0, buffer->length, PROT_READ, MAP_SHARED, fdCamera, buffer->m.offset);
        result = ioctl(fdCamera, VIDIOC_QBUF, buffer);  
    }
  
    if ( result != -1 )
    {
        std::cout << "DONE" << std::endl;
    }
    else
    {
        std::cout << "ERROR" << std::endl;
        ret = false;
    }


    std::cout << "Enabling streaming......................";
    // enable streaming
    int streaming = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    result = ioctl(fdCamera, VIDIOC_STREAMON, &streaming);
    if ( result != -1 )
    {
        std::cout << "DONE" << std::endl;
    }
    else
    {
        std::cout << "ERROR" << std::endl;
        ret = false;
    }


    setCamera(bottomCamera);

    std::cout << "Creating image mutex....................";
    // Create the speech mutex
    if (pthread_mutex_init(&imageMutex, NULL) < 0) 
    {
        std::cout << "ERROR" << std::endl;
        return false;
    }
    std::cout << "DONE" << std::endl;

    std::cout << "Starting camera thread..................";
    if (pthread_create(&cameraThread, NULL, &NaoV4L::startThread,this) < 0) 
    {
        std::cout << "ERROR" << std::endl;
        return false;
    }
    std::cout << "DONE" << std::endl;

    std::cout << "==================================================" << std::endl;
    return ret;
}

void NaoV4L::deinitialize()
{

    int streaming = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(fdCamera, VIDIOC_STREAMOFF, &streaming );

    for ( int i = 0; i < NUM_FRAME_BUFFERS; i++ )
    {
        munmap( buffers[i], bufferLength[i] );
    }

    if ( fdCamera != -1 )
    {
        close(fdCamera);
    }

    if ( fdI2C != -1 )
    {
        close(fdI2C);
    }
}

void* NaoV4L::startThread(void* obj)
{
        reinterpret_cast<NaoV4L*>(obj)->run();
            return 0;
}

void NaoV4L::run()
{
    while(1)
    {
        if (!grabFrame())
        {
            std::cout << "NaoV4L::run() - grabFrame failed!" << std::endl;
        }
        usleep(1000);
    }
}



void NaoV4L::saveFrame( const char* fileName )
{
    std::ofstream ofile(fileName,std::ios::binary);
    if(!ofile.good())
    {
        std::cout<<"NaoV4L::saveFrame() - The file " << fileName << " could not be opened for writing" << std::endl;
        return;
    }
    {
        ofile.write((char*)getImage() , imageSize);
    }
    ofile.close();
    
}

const unsigned char* NaoV4L::getImage()
{
    
    if (numFramesToIgnore > 0) {
        numFramesToIgnore--;
        //std::cout << "NaoV4L::getImage() - switched cameras recently" << std::endl;
        return rawImage;
    }
    else if ( lastBuffer )
    {
        return static_cast<unsigned char*>(buffers[lastBuffer->index]);
    }
    else
    {
        std::cout << "NaoV4L::getImage() - lastBuffer is empty" << std::endl;
        return rawImage;
    }
    
   
    //unsigned int t1 = log.getTimestamp();
    
    if ( lastBuffer )
    {
        //memcpy(rawImage,static_cast<unsigned char*>(buffers[lastBuffer->index]),IMAGESIZE);
        bcopy(static_cast<unsigned char*>(buffers[lastBuffer->index]),rawImage,IMAGESIZE);
    }

    //std::cout << "image copy took " << log.getTimestamp() - t1 << " milliseconds" << std::endl;
    
    return rawImage;
    
}

bool NaoV4L::grabFrame()
{
    bool ret = true;
    int result;
    
    if ( lastBuffer )
    {
        result = ioctl(fdCamera, VIDIOC_QBUF, lastBuffer );
        if ( result == -1 )
        {
            ret = false;
        }
    }


    result = ioctl(fdCamera, VIDIOC_DQBUF, buffer );
    if ( result != -1 )
    {
        if ( buffer->bytesused == imageSize )
        {
            lastBuffer = buffer;
            frameCount++;
        }
        else
        {
            ret = false;
        }
    }
    else
    {
        ret = false;
    }

    return ret;
}

bool NaoV4L::setCameraParams( CameraParams& params )
{
    bool ret = true;

    //std::cout << "Setting the camera parameters...........";
    ret &= setCameraSetting( V4L2_CID_AUTOGAIN, params.autoGain );
    ret &= setCameraSetting( V4L2_CID_AUTO_WHITE_BALANCE, params.autoWhiteBalance );
    ret &= setCameraSetting( V4L2_CID_AUTOEXPOSURE, params.autoExposure );
    ret &= setCameraSetting( V4L2_CID_EXPOSURE, params.exposure );
    ret &= setCameraSetting( V4L2_CID_GAIN, params.gain );
    ret &= setCameraSetting( V4L2_CID_BLUE_BALANCE, params.blueChroma );
    ret &= setCameraSetting( V4L2_CID_RED_BALANCE, params.redChroma );
    ret &= setCameraSetting( V4L2_CID_BRIGHTNESS, params.brightness );
    ret &= setCameraSetting( V4L2_CID_CONTRAST, params.contrast );
    ret &= setCameraSetting( V4L2_CID_SATURATION, params.saturation );
    ret &= setCameraSetting( V4L2_CID_HUE, params.hue );
    ret &= setCameraSetting( V4L2_CID_EXPOSURE_CORRECTION, params.exposureCorrection );
    ret &= setCameraSetting( V4L2_CID_HFLIP,params.hFlip );
    ret &= setCameraSetting( V4L2_CID_VFLIP,params.vFlip );

    /*
    if ( ret )
    {
        std::cout << "DONE" << std::endl;
    }
    else
    {
        std::cout << "ERROR" << std::endl;
    }
    */
    return ret;
}


bool NaoV4L::setCameraSetting( unsigned int setting, int value )
{
    bool ret = true;
    struct v4l2_control ctrl;
    ctrl.id = setting;
    ctrl.value = value;
    if( fdCamera && ( ioctl(fdCamera, VIDIOC_S_CTRL, &ctrl) < 0 ) )
    {
        ret = false;
    }
    return ret;
}

int NaoV4L::getCameraSetting( unsigned int setting )
{
    int ret = -1;
    struct v4l2_control ctrl;
    ctrl.id = setting;

    if( fdCamera && ( ioctl(fdCamera, VIDIOC_G_CTRL, &ctrl) < 0 ) )
    {
        ret = -1;
    }
    else
    {
        ret = ctrl.value;
    }
    return ret;
}

bool NaoV4L::setCamera( bool bottomCamera )
{

    bool ret = true;
    if ( fdI2C != -1 )
    {
        ret &= (!ioctl(fdI2C, 0x703, 0x8));
        unsigned char cmd[2] = { ( bottomCamera ? 2 : 1 )  , 0};
        ret &= (!i2c_smbus_write_block_data(fdI2C, 220, 1, cmd)); 

        numFramesToIgnore = NUM_FRAMES_TO_IGNORE_WHEN_SWITCHING_CAMERAS;

        /*
        if ( bottomCamera )
        {
            setCameraParams(bottomParams);
        }
        else
        {
            setCameraParams(topParams);
        }
        */
        //std::cout << "Camera switch took" << log.getTimestamp() - t1  << " ms" << std::endl;
    }
    else
    {
        ret = false;
    }
    return ret;
}

int NaoV4L::getImageSize()
{
    return imageSize;
}

int NaoV4L::getImageWidth()
{
    return imageWidth;
}

int NaoV4L::getImageHeight()
{
    return imageHeight;
}


