#ifndef RCAHOON_VISION_H_
#define RCAHOON_VISION_H_

#define Y_BITS  4
#define Y_SIZE  16
#define U_BITS  6
#define U_SIZE  64
#define V_BITS  6
#define V_SIZE  64

#define RUNSTEP  4
#define MIN_RUN_LENGTH  4

#include "Vision/Vision.h"
#include "Vision/rcahoon/vis_types.h"
#include "Vision/SegmentedImage/SegmentedImage.h"
#include "shared/Vector/Vector2D.h"

class RobotState;
class ConfigFile;
class Log;
class VisionFeatures;
class HMatrix;

namespace RCahoon {

class Vision : public ::Vision {
public:
	Vision(ConfigFile & configFile, Log & log);
	~Vision();

	bool run(const RobotState & robotState,
		   VisionFeatures & outputVisionFeatures);

	const SegmentedImage & getSegmentedImage();

	virtual void disableBallDetectionNextFrame();

private:
	PREVENT_COPY_AND_ASSIGNMENT(Vision);

	void initMap(ConfigFile & configFile);
	void computeRLE();
	void segmentImage();
	void findObjects(const HMatrix* transformationFromCamera, VisionFeatures & outputVisionFeatures);
	Vector2D cameraToWorld(const HMatrix* cameraBodyTransform, const Vector2D& imageCoords);
	inline int classify(pixel *p);

	int imageWidth;
	int rowStep;
	int imageHeight;
	int processHeight;

	pixel* frame; // camera frame
	
	int num_classes;
	ColorClass* classes;
	RGB* colors;
	int Color_Map[Y_SIZE][U_SIZE][V_SIZE];

	char* labeled_image;
	SegmentedImage seg_img;

	pixel_run *rle;
	int *row_starts;

	Log & log;
};

} // end namespace

#endif /* RCAHOON_VISION_H_ */
