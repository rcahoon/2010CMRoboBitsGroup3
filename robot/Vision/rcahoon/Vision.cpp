#include <fstream>
#include <ios>

#include "Vision/rcahoon/Vision.h"
//#include "Vision/rcahoon/shared/cameraconstants.h"
#include "Agent/RobotState.h"
#include "Vision/VisionFeatures.h"
#include "Vision/SegmentedImage/RGB.h"
#include "shared/Shape/Rectangle.h"
#include "shared/ConfigFile/ConfigFile.h"

#define COMPONENT VISION
#define CLASS_LOG_LEVEL LOG_LEVEL_TRACE
#include "Log/LogSettings.h"

namespace RCahoon {

Vision::Vision(ConfigFile & configFile, Log & _log)
	: imageWidth(configFile.getInt("vision/imageWidth")),
	  rowStep(imageWidth/2),
	  imageHeight(configFile.getInt("vision/imageHeight")),
	  processHeight(imageHeight-4),
	  frame(NULL),
	  num_classes(configFile.getInt("vision/numClasses")),
	  classes(new ColorClass[num_classes]),
	  colors(new RGB[num_classes]),
	  labeled_image(new char[imageWidth*imageHeight+1]),
	  seg_img(configFile),
	  rle(new pixel_run[imageWidth*processHeight/MIN_RUN_LENGTH]),
	  row_starts(new int[processHeight+1]()), // zero-initialized
	  log(_log)
{
	//LOG_INFO("Vision Memory Usage: %d bytes\n", sizeof(Vision) + sizeof(ColorClass)*num_classes + size(RGB)*num_classes + sizeof(pixel_run)*imageWidth*processHeight/MIN_RUN_LENGTH + sizeof(int)*(processHeight+1));
	
	initMap(configFile);

	seg_img.setIndices(labeled_image);
	seg_img.setRGB(colors, num_classes);
}

void Vision::initMap(ConfigFile & configFile)
{
	for(int i=0; i < num_classes; i++)
	{
		std::string path = "vision/classes/"; path += (char)(i+'0');
		classes[i] = ColorClass(
			configFile.getInt(path + "/yl"),
			configFile.getInt(path + "/yu"),
			configFile.getInt(path + "/ul"),
			configFile.getInt(path + "/uu"),
			configFile.getInt(path + "/vl"),
			configFile.getInt(path + "/vu"),
			configFile.getInt(path + "/red"),
			configFile.getInt(path + "/green"),
			configFile.getInt(path + "/blue"),
			configFile.getInt(path + "/minSize"),
			(VisionObject::Type)configFile.getInt(path + "/type")
		);
		printf("%s  Type: %d   (%d,%d,%d)-(%d,%d,%d)  (%d,%d,%d)  %d\n", path.c_str(), classes[i].vobj, classes[i].yl, classes[i].ul, classes[i].vl, classes[i].yu,
																	classes[i].uu, classes[i].vu, classes[i].color.getRed(), classes[i].color.getGreen(),
																	classes[i].color.getBlue(), classes[i].min_size);
		colors[i] = classes[i].color;
	}
	
	if (configFile.getInt("vision/thresholdYBits")!=Y_BITS ||
	    configFile.getInt("vision/thresholdUBits")!=U_BITS ||
	    configFile.getInt("vision/thresholdVBits")!=V_BITS )
	{
		LOG_ERROR("Threshold bitsize mismatch");
	}
	
	std::string tFile = configFile.getPath("vision/thresholdFile");
	FILE* fid = fopen(tFile.c_str(), "rb");
	if (fread(Color_Map, sizeof(uchar), Y_SIZE*U_SIZE*V_SIZE, fid) < Y_SIZE*U_SIZE*V_SIZE)
	{
		LOG_ERROR("Threshold file is wrong size.");
	}
	else
	{
		LOG_INFO("Threshold file loaded");
	}
	fclose(fid);
}

Vision::~Vision() {
	delete[] classes;
	delete[] colors;
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

	//LOG_INFO("Head: %f %f", robotState.getHeadPan(), robotState.getHeadTilt());

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

inline int Vision::classify(pixel *p)
{
	return Color_Map[p->y>>(8-Y_BITS)][p->u>>(8-U_BITS)][p->v>>(8-V_BITS)];
}

void Vision::computeRLE()
{
	pixel* data = frame;

	int k = 0;
	for(int j=0; j < processHeight; j++, data += rowStep)
	{
		int last = 0;
		int start = 0;
		
		row_starts[j] = k;
		
		for(int i=0; i < imageWidth; i+=RUNSTEP)
		{
			int t = classify(&data[i/2]);
			
			if (t==last) continue;
			
#if RUNSTEP > 2
			int end = (classify(&data[i/2-RUNSTEP/4])==last) ? i-RUNSTEP/2 : i-RUNSTEP;
#else
			int end = i-RUNSTEP;
#endif
			
			if (last && (end-start > MIN_RUN_LENGTH))
			{
				rle[k++].set(last, start, end, j);
				//printf("%d: (%d,%d) - (%d,%d)\n", last, start, j, end, j);
			}
			
			start = end+2;
			last = t;
		}
		
		if (last && (imageWidth-start > MIN_RUN_LENGTH))
		{
			rle[k++].set(last, start, imageWidth, j);
			//printf("%d: (%d,%d) - (%d,%d)\n", last, start, j, imageWidth, j);
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

// camera pseudo-focal length
#define HORZ_F  1.05f  // = tan(H_FOV)
#define VERT_F  0.695f // = tan(V_FOV)
#define H_SCALE 0.4f
#define V_SCALE 0.6f
// note: HORZ_F*H_SCALE = VERT_F*V_SCALE = 0.42

static Vector3D rotMul(const HMatrix* mat, const Vector3D& vec)
{
	Vector3D res(0.0f,0.0f,0.0f);
	for(int i=0; i < 3; i++)
		for(int j=0; j < 3; j++)
			res[i] += mat->get(i,j)*vec[j];
	return res;
}

Vector2D Vision::cameraToWorld(const HMatrix* cameraBodyTransform, const Vector2D& imageCoords)
{
	Vector3D cameraCoords = Vector3D(1.0f, -(imageCoords[0]/imageWidth-0.5f)*HORZ_F*H_SCALE,
	                                       -(imageCoords[1]/imageHeight-0.5f)*VERT_F*V_SCALE);
	//LOG_INFO("Camera: %f,%f\n", cameraCoords[1], cameraCoords[2]/*+0.035f*/);cameraBodyTransform->get(0,3)
	Vector3D ray = rotMul(cameraBodyTransform, cameraCoords);
	//ray.normalize(); printf("Object Heading: (%f %f %f)\n", ray[0], ray[1], ray[2]);
	
	Vector3D T( cameraBodyTransform->get(0,3),
	            cameraBodyTransform->get(1,3),
	            cameraBodyTransform->get(2,3) );
	
	T -= T[2]/ray[2]*ray;

	return Vector2D(T[0], T[1]);
}

static const char * object_name(VisionObject::Type typ) {
	switch(typ)
	{
		case VisionObject::Ball: return "Ball";
		case VisionObject::YellowGoalPost: return "B_GOAL";
		case VisionObject::BlueGoalPost: return "Y_GOAL";
		default: return "Unknown";
	}
}

void Vision::findObjects(const HMatrix* transform, VisionFeatures & outputVisionFeatures)
{
	for(int k = 0; k < row_starts[processHeight]; k++)
	{
		if (rle[k].rank >= 0)
		{
			float cen_x = rle[k].wcen_x/rle[k].area;
			float cen_y = rle[k].wcen_y/rle[k].area;
			Vector2D position = cameraToWorld(transform, Vector2D(cen_x, rle[k].y2));
			
			if (classes[rle[k].ob_class].vobj != VisionObject::Line)
			{
				LOG_INFO("#%d %s: (%d,%d)-(%d,%d) @i(%f,%f) @w(%f,%f) a%d",
					k, object_name(classes[rle[k].ob_class].vobj),
					rle[k].x1, rle[k].y1, rle[k].x2, rle[k].y2,
					cen_x, cen_y, position[0], position[1], rle[k].area);
				LOG_SHAPE(Log::OriginalImageScreen, Rectangle(Vector2D(rle[k].x1, rle[k].y1), Vector2D(rle[k].x2, rle[k].y2), 0x00FFFF, 1));
			}
			
			VisionObject *obj = new VisionObject(log, classes[rle[k].ob_class].vobj);
			obj->setPosition(position);
			obj->setConfidence(rle[k].area);
			obj->setBoundingBox(rle[k].x1, rle[k].y1, rle[k].x2, rle[k].y2);
			
			outputVisionFeatures.addVisionObject(*obj);
		}
	}
}

const SegmentedImage & Vision::getSegmentedImage() {
	memset(labeled_image, 0, imageWidth*imageHeight+1);
	for(int k=0; k < row_starts[processHeight]; k++)
	{
		int nextStart = rle[k].start+rle[k].y1*imageWidth;
		int nextEnd = rle[k].end+rle[k].y1*imageWidth;
		int nextColor = rle[k].ob_class;
		memset(labeled_image+nextStart, nextColor, nextEnd-nextStart);
	}
	
	return seg_img;
}

void Vision::disableBallDetectionNextFrame() {
}

} // end namespace
