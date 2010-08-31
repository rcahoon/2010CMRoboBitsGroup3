#ifndef NAOV4L_H_
#define NAOV4L_H_

#include <linux/videodev.h>
#include <linux/videodev2.h>
#include <shared/ConfigFile/ConfigFile.h>
#include <Log/Log.h>
#include <pthread.h>

#define NUM_FRAME_BUFFERS 3

#define V4L2_CID_SHARPNESS              (V4L2_CID_BASE+27)
#define V4L2_CID_AUTOEXPOSURE           (V4L2_CID_BASE+32)
#define V4L2_CID_CAM_INIT               (V4L2_CID_BASE+33)
#define V4L2_CID_EXPOSURE_CORRECTION    (V4L2_CID_BASE+34)
#define V4L2_CID_AEC_ALGORITHM          (V4L2_CID_BASE+35)
#define IMAGESIZE 153600

struct CameraParams
{
    int autoGain;
    int autoWhiteBalance;
    int autoExposure;
    int exposure;
    int gain; 
    int blueChroma;
    int redChroma;
    int brightness;
    int contrast;
    int saturation;
    int hue;
    int exposureCorrection;
    int hFlip;
    int vFlip;
};


class NaoV4L 
{
    public:
    NaoV4L();
    virtual ~NaoV4L();

    bool initialize();
    void deinitialize();
    bool setCamera(bool bottomCamera);
    bool setCameraSetting(unsigned int setting, int value);
    int  getCameraSetting(unsigned int setting);
    bool grabFrame();
    void saveFrame(const char* fileName);
    const unsigned char* getImage();
    int getImageSize();
    int getImageWidth();
    int getImageHeight();

    static void* startThread(void *ptr);
    void run();

    unsigned frameCount;
    
    private:
    bool setCameraParams(CameraParams& params);
    bool initCam(bool bottomCamera);
    int fdCamera; /**< the file descriptor for the camera */
    int fdI2C; /**< the file descriptor for the I2C camera switch device */

    void* buffers[NUM_FRAME_BUFFERS]; /**< the number of frame buffers for memory mapped image retrieval */
    int bufferLength[NUM_FRAME_BUFFERS]; /**< the length of frame buffers */
    struct v4l2_buffer* buffer;
    struct v4l2_buffer* lastBuffer;

    ConfigFile& config;
    Log& log;

    const int NUM_FRAMES_TO_IGNORE_WHEN_SWITCHING_CAMERAS;

    std::string cameraDevice;
    std::string i2cDevice;
    
    unsigned int imageWidth;
    unsigned int imageHeight;
    unsigned int imageSize;
    int fps;

    unsigned char rawImage[IMAGESIZE];

    bool bottomCamera;
   
    CameraParams topParams;
    CameraParams bottomParams;

    pthread_mutex_t imageMutex;
    pthread_t cameraThread;

    int numFramesToIgnore;
};

#endif /* NAOV4L_H_ */
