#ifndef RCAHOON_VISION_H_
#define RCAHOON_VISION_H_

#define Y_BITS  4
#define U_BITS  6
#define V_BITS  6
#define Y_SIZE  (1<<Y_BITS)
#define U_SIZE  (1<<U_BITS)
#define V_SIZE  (1<<V_BITS)

#define PIXEL_CHANGE_THRESH  0x03000000  /* yuyv */

#define LINE_PROXIMITY_THRESH  27
#define SCAN_TOP_DECLINATION  /*-0.025*/ 0

#define GOAL_DENSITY_THRESH  1.3f

#define LINE_BLOCK_SIZE  30
#define LINE_SAMP_WIDTH  0.003
#define LINE_SAMP_HEIGHT 0.06

#define RUNSTEP  4
#define MIN_RUN_LENGTH  4

#include "Vision/Vision.h"
#include "Vision/rcahoon/vis_types.h"
#include "Vision/SegmentedImage/SegmentedImage.h"
#include "shared/Vector/Vector2D.h"
#include <list>

class RobotState;
class ConfigFile;
class Log;
class VisionFeatures;
class HMatrix;

using namespace std;

namespace RCahoon {

Vector2D cameraToWorld(const HMatrix* cameraBodyTransform, const Vector2D& imageCoords);

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
	void segmentImage(const HMatrix* transform);
	void findObjects(const HMatrix* transformationFromCamera, VisionFeatures & outputVisionFeatures);
	inline int classify(pixel p);
	VisionObject* addVisionObject(VisionObject::Type type, float area, int x1, int y1, int x2, int y2, float center,
		const HMatrix* transform, VisionFeatures & outputVisionFeatures);

	int imageWidth;
	int rowStep;
	int imageHeight;
	int processHeight;

	pixel* frame; // camera frame
	
	int num_classes;
	ColorClass* classes;
	RGB* colors;
	uchar Color_Map[Y_SIZE][U_SIZE][V_SIZE];
	
	list<pixel_run*> balls;
	list<pixel_run*> b_goals;
	list<pixel_run*> y_goals;
	list<pixel_run*> lines;

	char* labeled_image;
	SegmentedImage seg_img;

	pixel_run* rle;
	int *row_starts;

	Log & log;
};

} // end namespace

#endif /* RCAHOON_VISION_H_ */
