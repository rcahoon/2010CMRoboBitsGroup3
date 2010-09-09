#include <fstream>
#include <ios>

#include "Vision/rcahoon/Vision.h"
#include "Agent/RobotState.h"
#include "Vision/VisionFeatures.h"
#include "Vision/VisionObject/VisionObject.h"
#include "Vision/VisionObject/GoalPostVisionObject.h"
#include "Vision/SegmentedImage/RGB.h"
#include "shared/Shape/Rectangle.h"
#include "shared/ConfigFile/ConfigFile.h"

#define COMPONENT VISION
#define CLASS_LOG_LEVEL LOG_LEVEL_TRACE
#include "Log/LogSettings.h"

namespace RCahoon {

int num_classes = 4;
enum ClassNames {
	Default = 0,
	Ball,
	Y_Goal,
	B_Goal
};
ColorClass classes[] = {
	//        class name     threshold values     label color  parameters
	//                    yl  yu  ul  uu  vl  vu  red grn blu  min size
	ColorClass("Bkgnd",    0,256,  0,256,  0,256,   0,255,  0,   0), // Default/Field
	ColorClass("Ball",    48,256, 80,136,166,256, 255,  0,  0,   8), // Ball
	ColorClass("Y Goal",  64,128, 72,108,144,166, 255,255,  0, 400), // Yellow Goal
	ColorClass("B Goal",  48,256,132,176, 72,166,   0,  0,255, 400)  // Blue Goal
};
static RGB colors[] = {
	classes[Default].color,
	classes[Ball].color,
	classes[Y_Goal].color,
	classes[B_Goal].color
};

Vision::Vision(ConfigFile & configFile, Log & _log)
	: imageWidth(configFile.getInt("vision/imageWidth")),
	  rowStep(imageWidth/2),
	  imageHeight(configFile.getInt("vision/imageHeight")),
	  processHeight(imageHeight-4),
	  frame(NULL),
	  labeled_image(new char[imageWidth*imageHeight+1]()),
	  seg_img(configFile),
	  rle(new pixel_run[imageWidth*processHeight/MIN_RUN_LENGTH]),
	  row_starts(new int[processHeight+1]()), // zero-initialized
	  log(_log)
{
	// Retrieve constants from the config file
	//std::string colors    = configFile.getPath("vision/colorsPath");
	//std::string threshold = configFile.getPath("vision/thresholdPath");
	
	printf("Size: %d bytes\n", sizeof(Vision) + sizeof(pixel_run)*imageWidth*processHeight/MIN_RUN_LENGTH + sizeof(int)*(processHeight+1));
	
	initMap();

	seg_img.setRGB(colors, num_classes);
}

void Vision::initMap()
{
	for(int y=0; y < Y_SIZE; y++)
	for(int u=0; u < U_SIZE; u++)
	for(int v=0; v < V_SIZE; v++)
	{
		for(int c=num_classes-1; c >= 0; c--)
		{
			if (classes[c].match(y,u,v))
			{
				Color_Map[y][u][v] = c;
				break;
			}
		}
	}
}

Vision::~Vision() {
	delete[] labeled_image;
	delete[] rle;
	delete[] row_starts;
}

static void clearFeaturesList(VisionFeatures & outputVisionFeatures)
{
	std::vector<VisionObject const *> features = outputVisionFeatures.getAllVisionObjects();
	
	for(std::vector<VisionObject const *>::iterator it = features.begin(); it != features.end(); it++)
		delete *it;
	
	outputVisionFeatures.clear();
}

bool Vision::run(const RobotState & robotState,
                 VisionFeatures & outputVisionFeatures) {
#ifdef LOG_TRACE_ACTIVE
	unsigned long startTime;
	unsigned long elapsed;
#endif

	LOG_TRACE("Vision run started.");
	
	clearFeaturesList(outputVisionFeatures);

	// Retrieve the frame from camera
	frame = (pixel *)robotState.getRawImage();

#ifdef LOG_TRACE_ACTIVE
	startTime = log.getTimestamp();
#endif
	// Threshold the image
	computeRLE();
#ifdef LOG_TRACE_ACTIVE
	elapsed = log.getTimestamp() - startTime;
#endif
	LOG_TRACE("Thresholding took %d ms.", elapsed);
	
	LOG_INFO("Found %d runs.", row_starts[processHeight]);

#ifdef LOG_TRACE_ACTIVE
	startTime = log.getTimestamp();
#endif
	// Segment the image
	segmentImage();
#ifdef LOG_TRACE_ACTIVE
	elapsed = log.getTimestamp() - startTime;
#endif
	LOG_TRACE("Segmenting took %d ms.", elapsed);

#ifdef LOG_TRACE_ACTIVE
	startTime = log.getTimestamp();
#endif
	// Retrieve the transformation matrix from the camera
	HMatrix const* transformationFromCamera = &(robotState.getTransformationFromCamera());
#ifdef LOG_TRACE_ACTIVE
	elapsed = log.getTimestamp() - startTime;
#endif
	LOG_TRACE("Retrieving transformation matrix from RobotState took %d ms.", elapsed);

#ifdef LOG_TRACE_ACTIVE
	startTime = log.getTimestamp();
#endif
	// Extract objects
	findObjects(transformationFromCamera, outputVisionFeatures);
#ifdef LOG_TRACE_ACTIVE
	elapsed = log.getTimestamp() - startTime;
#endif
	LOG_TRACE("Extraction took %d ms.", elapsed);

#ifdef LOG_TRACE_ACTIVE
	startTime = log.getTimestamp();
#endif
	// Log the segmented image
	LOG_SEGMENTEDIMAGE(getSegmentedImage());
#ifdef LOG_TRACE_ACTIVE
	elapsed = log.getTimestamp() - startTime;
#endif
	LOG_TRACE("Logging segmented image took %d ms.", elapsed);

	LOG_TRACE("Vision run ended.");
	return false;
}

void Vision::computeRLE()
{
	pixel* data = frame;

	int k = 0;
	for(int j=0; j < processHeight; j++, data += rowStep)
	{
		int t = 0;
		int last = 0;
		int start = 0;
		
		row_starts[j] = k;
		
		for(int i=0; i < imageWidth; )
		{
			for(; (t == last) && (i < imageWidth); i+=RUNSTEP) {
				pixel p = data[i/2];
				t = Color_Map[p.y>>(8-Y_BITS)][p.u>>(8-U_BITS)][p.v>>(8-V_BITS)];
				
#ifdef LOG_SEGMENTEDIMAGE_ENABLED
				labeled_image[i+j*imageWidth] = t;
				labeled_image[i+j*imageWidth+1] = t;
#endif
			}
			
			if (last && (i-start > MIN_RUN_LENGTH))
			{
				rle[k++].set(last, start-RUNSTEP, i-RUNSTEP, j);
				//printf("%d: (%d,%d) - (%d,%d)\n", last, start, j, i, j);
			}
			
			start = i;
			last = t;
		}
	}
	row_starts[processHeight] = k;
}

void Vision::segmentImage()
{
	for(int r = 1; r < processHeight; r++)
	{
		int i = row_starts[r-1];
		int j = row_starts[r];
		int endi = row_starts[r];
		int endj = row_starts[r+1];
		while((i < endi) && (j < endj))
		{
			if (rle[i].end < rle[j].start)
			{
				i++;
			}
			else if (rle[i].start > rle[j].end)
			{
				j++;
			}
			else
			{
				if (rle[i].ob_class == rle[j].ob_class)
					rle[i].doUnion(rle[j]);
				
				if (rle[i].end > rle[j].end)
					j++;
				else
					i++;
			}
		}
	}
	for(int k = 0; k < row_starts[processHeight]; k++)
	{
		if (rle[k].area < classes[rle[k].ob_class].min_size)
			rle[k].rank = -1;
	}
}

Vector2D cameraToWorld(const HMatrix cameraBodyTransform, const Vector2D& cameraCoords)
{
	return cameraCoords;
}

void Vision::findObjects(const HMatrix transform, VisionFeatures & outputVisionFeatures)
{
	for(int k = 0; k < row_starts[processHeight]; k++)
	{
		if (rle[k].rank >= 0)
		{
			VisionObject *obj;
		
			float cen_x = rle[k].wcen_x/rle[k].area;
			float cen_y = rle[k].wcen_y/rle[k].area;
			
			LOG_INFO("#%d %s: (%d,%d)-(%d,%d) c(%f,%f) a%d",
				k, classes[rle[k].ob_class].name,
				rle[k].x1, rle[k].y1, rle[k].x2, rle[k].y2,
				cen_x, cen_y, rle[k].area);
			LOG_SHAPE(Log::SegmentedImageScreen, Rectangle(Vector2D(rle[k].x1, rle[k].y1), Vector2D(rle[k].x2, rle[k].y2), 0x00FFFF, 1));
			
			switch(rle[k].ob_class)
			{
			case Ball: // Ball
				obj = new VisionObject(log, VisionObject::Ball);
			break;
			case Y_Goal: // Yellow Goal
			case B_Goal: // Blue Goal
				obj = new GoalPostVisionObject(log, rle[k].ob_class == 3);
			break;
			default:
				obj = new VisionObject(log, VisionObject::Unknown);
			}
			
			obj->setPosition(cameraToWorld(transform, Vector2D(cen_x, cen_y)));
			obj->setConfidence(rle[k].area);
			obj->setBoundingBox(rle[k].x1, rle[k].y1, rle[k].x2, rle[k].y2);
			
			outputVisionFeatures.addVisionObject(*obj);
		}
	}
}

const SegmentedImage & Vision::getSegmentedImage() {
	seg_img.setIndices(labeled_image);
	return seg_img;
}

void Vision::disableBallDetectionNextFrame() {
}

} // end namespace