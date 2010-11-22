// (C) 2002-2006 James R. Bruce, Carnegie Mellon University
// Licenced under the GNU General Public License (GPL) version 2,
//   or alternately by a specific written agreement.

#include "Vision/SegmentedImage/RGB.h"
#include "shared/Shape/Rectangle.h"
#include "shared/Shape/Line.h"
#include "Vision/SegmentedImage/SegmentedImage.h"
//#include "VisionObject/VisionObject.h"

#define INDEX(x,y)	(IMAGEWIDTH*(y)+(x))
#define XP(index)	((index)%IMAGEWIDTH)
#define YP(index)	((int)((index)/IMAGEWIDTH)+1)

#define INTMAX 30000
#define INTMIN -30000
// Define the logging constants
#define COMPONENT VISION
//#define CLASS_LOG_LEVEL LOG_LEVEL_WARN
#include "Log/LogSettings.h"

#include "shared/crc32.h"

#include "vision.h"
#include <iostream>
#include <assert.h>

#define ODD(x) ((x) % 2)

#define CLASSIFY(pos) \
		if ( pos < IMAGESIZE ) \
		{ \
			p = buf[(pos) / 2]; \
			m = ( p.u << 8) + p.v; \
			if ( ODD((pos)) ) \
			{ \
				cmap[ (pos) ] = tmap[m + ((p.y1 >> 4) << 16)]; \
				/*cmap[ (pos) - 1 ] = tmap[m + ((p.y2 >> 4) << 16)];*/ \
		cmap[ (pos)-1 ] = cmap[ (pos) ]; \
			}\
			else \
			{ \
				cmap[ (pos) ] = tmap[m + ((p.y1 >> 4) << 16)]; \
				/*cmap[ (pos) + 1] = tmap[m + ((p.y2 >> 4) << 16)];*/ \
		cmap[(pos)+1] = cmap[(pos)]; \
			} \
		} \

namespace RoboCup2010 {

int yL=0;
int xL=0;
int xH=IMAGEWIDTH;
int yH=IMAGEHEIGHT;

int STEP = 6;
int FOV = 20;

int scanLine = 0;


#define ROWSPACE 6
#define COLSPACE 20

#define HORMAX 2
#define HORMIN 1
#define VERT 0

int* horMinArray = new int [IMAGEHEIGHT/ROWSPACE];
int* horMaxArray = new int [IMAGEHEIGHT/ROWSPACE];

int* vertMinArray = new int [IMAGEWIDTH/COLSPACE];
int* vertMaxArray = new int [IMAGEWIDTH/COLSPACE];

int vertMax = 0;
int vertMin = IMAGEWIDTH;
int horMax = 0;
int horMin = IMAGEHEIGHT;

int* fuzzLeft = new int [IMAGEHEIGHT/ROWSPACE];
int* fuzzRight = new int [IMAGEHEIGHT/ROWSPACE];
int* fuzzVert = new int [IMAGEWIDTH/COLSPACE];

int bLeft = 0;
int bRight = 0;
int tLeft = 0;
int tRight = 0;

int barLeft = 0;
int barRight = 0;

pixel p;
int m;
cmap_t save;
int x,y,j;
Run r;


rgb YuvToRgb(yuv p) {
	rgb r;
	int y, u, v;

	y = p.y;
	u = p.v * 2 - 255;
	v = p.u * 2 - 255;

	r.red = bound(y + u, 0, 255);
	r.green = bound((int) (y - 0.51 * u - 0.19 * v), 0, 255);
	r.blue = bound(y + v, 0, 255);

	return (r);
}

LowVision::LowVision(ConfigFile & configFile, Log & _log) :
									log(_log), buf(NULL), cmap(NULL), tmap(NULL), rmap(NULL), reg(NULL),
									segmentedImage(configFile), segmentedColors(NULL) {
}

LowVision::~LowVision() {
	if (cmap != NULL) {
		delete[] cmap;
		cmap = NULL;
	}
	if (tmap != NULL) {
		delete[] tmap;
		tmap = NULL;
	}
	if (rmap != NULL) {
		delete[] rmap;
		rmap = NULL;
	}
	if (reg != NULL) {
		delete[] reg;
		reg = NULL;
	}
	for (int i = 0; i < NumColors; i++) {
		if (color[i].name != NULL) {
			free(color[i].name);
			color[i].name = NULL;
		}
	}
	if (segmentedColors != NULL) {
		delete[] segmentedColors;
		segmentedColors = NULL;
	}
}

bool LowVision::init(const char *colorfile, const char *tmapfile, int _width,
		int _height) {
	int num_y, num_u, num_v;
	int size;
	bool ok = true;

	max_width = width = _width;
	max_height = height = _height;

	// Load color information
	num_colors = CMVision::LoadColorInformation(color, NumColors, colorfile);
	if (num_colors > 0) {
		LOG_INFO	("Loaded %d colors.", num_colors)
	} else {
		LOG_ERROR("Could not load color information \"%s\".", colorfile)
									ok = false;
	}

	// Save color information to colorRgb
	memset(colorRgb, 0, NumColors * sizeof(rgb));
	for (int i = 0; i < num_colors; i++) {
		colorRgb[i] = color[i].color;
	}

	// Set up the colors for segmented image
	setupSegmentedImage();

	// Set up threshold
	size = 1 << (bits_y + bits_u + bits_v);
	num_y = 1 << bits_y;
	num_u = 1 << bits_u;
	num_v = 1 << bits_v;

	tmap = CMVision::NewThresholdMap<cmap_t>(num_y,num_u,num_v);
	// tmap = new cmap_t[size];
	// memset(tmap,0,size*sizeof(cmap_t));

	// CMVision::SaveThresholdFile(tmap,num_y,num_u,num_v,tmapfile);

	if(CMVision::LoadThresholdFile(tmap,num_y,num_u,num_v,tmapfile)) {
		LOG_INFO("Loaded thresholds.")
	} else {
		LOG_ERROR("Could not load thresholds \"%s\".", tmapfile)
									ok = false;
	}
	CMVision::CheckTMapColors(tmap,num_y,num_u,num_v,num_colors,0);

	// Allocate map structures
	max_width = width;
	max_height = height;
	size = width * height;
	max_runs = size / MIN_EXP_RUN_LENGTH;
	max_regions = size / MIN_EXP_REGION_SIZE;
	size = max_width * max_height;
	cmap = new cmap_t[size+1]; // extra needed for EncodeRuns terminator
	rmap = new Run[max_runs];
	reg = new Region[max_regions];

	mzero(cmap,size+1);
	mzero(rmap,max_runs);
	mzero(reg,max_regions);

	frame = -1;
	field = 0;

	return(ok) ;
}

bool LowVision::close()
{
	delete[](tmap);
	delete[](cmap);
	delete[](rmap);
	delete[](reg);

	tmap = NULL;
	cmap = NULL;
	rmap = NULL;
	reg = NULL;

	max_width = 0;
	max_height = 0;

	return(true);
}

unsigned checksum(char *buf,int len)
{
	unsigned u;
	int i;

	u = 0;
	for(i=0; i<len; i++) {
		u = ((u << 5) | (u >> (32-5))) ^ buf[i];
	}

	return(u);
}



bool LowVision::processFrame(vision_image &img)
{

	if ( img.buf == NULL )
	{
		return false;
	}

	buf = img.buf;
	width = img.width;
	height = img.height;
	field = img.field;
	frame++;


	r.next = 0;

	CLASSIFY(0)

	save = cmap[0];

	j = 0;

	/***********************************************************************************/
	/***********************************************************************************/

	int index = 0;

	xMin = 500;
	yMin = 500;
	xMax = 0;
	yMax = 0;
	ballPresent = 0;


	vertMax = 0;
	vertMin = IMAGEWIDTH;
	horMax = 0;
	horMin = IMAGEHEIGHT;

	//BZERO WORKS ON BYTES :: FIX ME
	bzero(cmap,IMAGEWIDTH*IMAGEHEIGHT);

	bzero(fuzzLeft, IMAGEHEIGHT/ROWSPACE * sizeof(int));
	bzero(fuzzRight, IMAGEHEIGHT/ROWSPACE * sizeof(int));

	barLeft = 0;
	barRight = 0;

	for( int col = 0; col < IMAGEWIDTH; col += COLSPACE)
	{
		vertMinArray[col/COLSPACE] = vertLineMin(col);
		LOG_SHAPE(Log::OriginalImageScreen, Rectangle(Vector2D(col, YP(vertMinArray[col/COLSPACE])),Vector2D( col + 2, YP(vertMinArray[col/COLSPACE]) + 2),0x0000FF, 2));
	}

	vertMin = YP(fuzzyModeVert(vertMinArray, IMAGEWIDTH/COLSPACE));


	bLeft = 0;
	bRight = 0;
	tLeft = 0;
	tRight = 0;

	vertMin = vertMin-10;
	if(vertMin > 240 || vertMin < 0)
		vertMin = 0;

	for(int row = vertMin; row < IMAGEHEIGHT; row += ROWSPACE)
	{
		horMinArray[row/ROWSPACE] = horLineMin(row);
		horMaxArray[row/ROWSPACE] = horLineMax(row);

		if(abs(horMinArray[row/ROWSPACE]) != INTMAX)
			LOG_SHAPE(Log::OriginalImageScreen, Rectangle(Vector2D(XP(horMinArray[row/ROWSPACE]), row),Vector2D(XP(horMinArray[row/ROWSPACE]) + 2, row + 2),0x0000FF, 2));
		if(abs(horMaxArray[row/ROWSPACE]) != INTMAX)
			LOG_SHAPE(Log::OriginalImageScreen, Rectangle(Vector2D(XP(horMaxArray[row/ROWSPACE]), row),Vector2D(XP(horMaxArray[row/ROWSPACE]) + 2, row + 2),0x0000FF, 2));

	}
	horMin = XP(fuzzyModeHorMin(horMinArray, IMAGEHEIGHT/ROWSPACE));
	horMax = XP(fuzzyModeHorMax(horMaxArray, IMAGEHEIGHT/ROWSPACE));

	removeOutliers(fuzzLeft, IMAGEHEIGHT/ROWSPACE);
	removeOutliers(fuzzRight, IMAGEHEIGHT/ROWSPACE);

	tLeft = YP(fuzzLeft[0]);
	tRight = YP(fuzzRight[0]);

	int i = 0;
	while( fuzzLeft[i] != 0 && i < IMAGEHEIGHT/ROWSPACE){
		i++;
		LOG_SHAPE(Log::OriginalImageScreen, Rectangle(Vector2D(XP(fuzzRight[i]), YP(fuzzRight[i])),Vector2D(XP(fuzzRight[i]) + 2, YP(fuzzRight[i])+ 2),0xFF0000, 2));
	}
	bLeft = YP(fuzzLeft[i-1]);

	i = 0;
	while( fuzzRight[i] != 0 && i < IMAGEHEIGHT/ROWSPACE){
		i++;
	}
	bRight = YP(fuzzRight[i-1]);

	/*barLeft = XP(fuzzVert[0]);

	int i = 0;
	while( fuzzVert[i] != 0 && i < IMAGEHEIGHT/ROWSPACE){
		i++;
		LOG_SHAPE(Log::OriginalImageScreen, Rectangle(Vector2D(XP(fuzzVert[i]), YP(fuzzVert[i])),Vector2D(XP(fuzzVert[i]) + 2, YP(fuzzVert[i])+ 2),0x00FF00, 2));

	}
	barRight = XP(fuzzVert[i-1]);*/

	//LOG_TRACE("horMin %d vertMin %d  horMax %d vertMax %d", horMin, vertMin, horMax, vertMax);
	if( !(horMax > 320 || bLeft > 240 || bRight > 240 || horMin < 0 || tLeft < 0 || tRight < 0)){
	//	LOG_SHAPE(Log::OriginalImageScreen, Line(Vector2D(barLeft, tLeft),Vector2D(barRight, tRight),0x00FFFF, 2));

		LOG_SHAPE(Log::OriginalImageScreen, Line(Vector2D(horMin, tLeft),Vector2D(horMax, tRight),0xFF00FF, 2));
		LOG_SHAPE(Log::OriginalImageScreen, Line(Vector2D(horMin, bLeft),Vector2D(horMax, bRight),0xFF0000, 2));

		LOG_SHAPE(Log::OriginalImageScreen, Line(Vector2D(horMin, tLeft),Vector2D(horMin, bLeft),0xFF0000, 2));
		LOG_SHAPE(Log::OriginalImageScreen, Line(Vector2D(horMax, tRight),Vector2D(horMax, bRight),0xFF0000, 2));
	}




	////////////////////////////////////////////////////////////////////////////////////////////////////
	LOG_SHAPE(Log::OriginalImageScreen, Rectangle(Vector2D(xL, yL),Vector2D(xH, yH),0xFF0099, 2));

for(y=yL; y<yH; y+=STEP)
{
	for(x=xL; x<xH; x+=STEP)
	{
		index = y * IMAGEWIDTH + x  ;

		p = buf[(index) / 2];
		m = ( p.u << 8) + p.v;
		if (index % 2)
		{
			cmap[ (index) ] = tmap[m + ((p.y1 >> 4) << 16)];
			cmap[ (index)-1 ] = cmap[ (index) ];
		}
		else
		{
			cmap[ (index) ] = tmap[m + ((p.y1 >> 4) << 16)];
			cmap[(index)+1] = cmap[(index)];
		}
		if(cmap[index]==3)
		{
			if(x > xMax)
				xMax = x;
			if(x < xMin)
				xMin = x;
			if(y > yMax)
				yMax = y;
			if(y < yMin)
				yMin = y;
			ballPresent = 1;
		}
	}

}
if(ballPresent){
	STEP=4;
	LOG_SHAPE(Log::OriginalImageScreen, Rectangle(Vector2D(xMin, yMin),Vector2D(xMax, yMax),0x00FFFF, 2));
	yH=yMax + FOV;
	yL=yMin - FOV;
	xH=xMax + FOV;
	xL=xMin - FOV;

	if(yL < 0)
		yL = 0;
	if(xL < 0)
		xL = 0;
	if(yH > IMAGEHEIGHT)
		yH = IMAGEHEIGHT;
	if(xH > IMAGEWIDTH)
		x = IMAGEWIDTH;
}
else{

	STEP=8;
	yL=0;
	yH=IMAGEHEIGHT;
	xL=0;
	xH=IMAGEWIDTH;
}

	/***********************************************************************************/
	/***********************************************************************************/


	return(true);
}
/*************************************************************************************
 * GoalLine finding methods
 *************************************************************************************/
# define ADJ 1

int LowVision::horLineMin(int pos)
{
	for(x=0; x<320; x++){

		int index = IMAGEWIDTH*pos + x;

		p = buf[(index) / 2];
		m = ( p.u << 8) + p.v;
		if (index % 2){
			cmap[ (index) ] = tmap[m + ((p.y1 >> 4) << 16)];
			cmap[ (index)-1 ] = cmap[ (index) ];
		}
		else{
			cmap[ (index) ] = tmap[m + ((p.y1 >> 4) << 16)];
			cmap[(index)+1] = cmap[(index)];
		}
		if(cmap[index]==GOAL && getAdj(x, pos) > ADJ){

			return index;
		}
	}
	return INTMIN;  //NOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOTEED.
}

int LowVision::horLineMax(int pos)
{
	for(x= 320; x>0; x--){

		int index = IMAGEWIDTH*pos + x;

		p = buf[(index) / 2];
		m = ( p.u << 8) + p.v;
		if (index % 2){
			cmap[ (index) ] = tmap[m + ((p.y1 >> 4) << 16)];
			cmap[ (index)-1 ] = cmap[ (index) ];
		}
		else{
			cmap[ (index) ] = tmap[m + ((p.y1 >> 4) << 16)];
			cmap[(index)+1] = cmap[(index)];
		}
		if(cmap[index]==GOAL && getAdj(x, pos) > ADJ){
			return index;
		}
	}
	return INTMAX;
}


int LowVision::vertLineMin(int pos)
{
	for(y=0; y<240; y ++){

		int index = IMAGEWIDTH*y + pos;

		p = buf[(index) / 2];
		m = ( p.u << 8) + p.v;
		if (index % 2){
			cmap[ (index) ] = tmap[m + ((p.y1 >> 4) << 16)];
			cmap[ (index)-1 ] = cmap[ (index) ];
		}
		else{
			cmap[ (index) ] = tmap[m + ((p.y1 >> 4) << 16)];
			cmap[(index)+1] = cmap[(index)];
		}
		if(cmap[index]==GOAL && getAdj(pos, y) > ADJ){
			return index;
		}
	}
	return INTMIN;
}

int LowVision::vertLineMax(int pos)
{
	for(y=240; y>0; y--){

		int index = IMAGEWIDTH*y + pos;

		p = buf[(index) / 2];
		m = ( p.u << 8) + p.v;
		if (index % 2){
			cmap[ (index) ] = tmap[m + ((p.y1 >> 4) << 16)];
			cmap[ (index)-1 ] = cmap[ (index) ];
		}
		else{
			cmap[ (index) ] = tmap[m + ((p.y1 >> 4) << 16)];
			cmap[(index)+1] = cmap[(index)];
		}
		if(cmap[index]==GOAL && getAdj(pos, y) > ADJ){
			return index;
		}
	}
	return INTMAX;

}

/**************************************************************************************
 * Helper functions
 *************************************************************************************/
#define FUZZ 10

int LowVision::fuzzyModeHorMin(int* array, int length)
{
	int max = 0;
	int counter = 0;
	int bestPos = -1;
	int* fLeft = new int [length];
	int index = 0;

	for(int q = 0; q < length; q++)
	{
		counter = 0;
		index = 0;
		bzero(fLeft, length*sizeof(int));

		for(int p =0; p<length; p++){

			if( abs(XP(array[q]) - XP(array[p])) < FUZZ && abs(array[p]) != INTMAX){

				counter ++;
				fLeft[index] = array[p];
				index++;
			}
		}
		if(counter > max){
			bestPos = q;
			max = counter;
			memcpy(fuzzLeft, fLeft, length*sizeof(int));
		}
	}
	return array[bestPos];
}

int LowVision::fuzzyModeHorMax(int* array, int length)
{
	int max = 0;
	int counter = 0;
	int bestPos = -1;
	int* fRight = new int [length];
	int index = 0;

	for(int q = 0; q < length; q++)
	{
		counter = 0;
		index = 0;
		bzero(fRight, length*sizeof(int));

		for(int p =0; p<length; p++){

			if( abs(XP(array[q]) - XP(array[p])) < FUZZ && abs(array[p]) != INTMAX){

				counter++;
				fRight[index] = array[p];
				index++;
			}
		}
		if(counter > max){
			bestPos = q;
			max = counter;
			memcpy(fuzzRight, fRight, length*sizeof(int));
		}
	}
	return array[bestPos];
}

int LowVision::fuzzyModeVert(int* array, int length)
{
	int max = -1;
	int counter = 0;
	int bestPos = -1;
	int* fVert = new int [length];
	int index = 0;

	for(int q = 0; q < length; q++)
	{
		counter = 0;

		for(int p =0; p<length; p++){
			if( YP(abs(array[q]) - YP(array[p])) < FUZZ && abs(array[p]) != INTMAX){
				counter++;
				fVert[index] = array[p];
				index++;
			}
		}
		if(counter > max){
			bestPos = q;
			max = counter;
			memcpy(fuzzVert, fVert, length*sizeof(int));
		}
	}
	return array[bestPos];
}

void LowVision::removeOutliers(int* array, int length)
{
	//int length = sizeof(array)/sizeof(array[0]);
	int* clean = new int[length];
	bzero(clean,sizeof(clean));
	int index = 0;

	if (abs(YP(array[1]) - YP(array[0])) < FUZZ){
		clean[index] = array[0];
		index++;
	}


	for(int i = 1; i< length-2; i++)
	{
		if (abs(YP(array[i-1]) - YP(array[i])) < FUZZ || abs(YP(array[i+1]) - YP(array[i])) < FUZZ){
			clean[index] = array[i];
			index++;
		}
	}

	if (abs(YP(array[length-2]) - YP(array[length-1])) < FUZZ){
		clean[index] = array[length-1];
		index++;
	}

	memcpy(array, clean, sizeof(clean));
}


int LowVision::getAdj(int x, int y)
{
	int count = 0;

	if(cmap[INDEX(x-1, y-1)] == GOAL)
		count++;
	if(cmap[INDEX(x-1, y)] == GOAL)
		count++;
	if(cmap[INDEX(x, y-1)] == GOAL)
		count++;
	if(cmap[INDEX(x+1, y+1)] == GOAL)
		count++;
	if(cmap[INDEX(x, y+1)] == GOAL)
		count++;
	if(cmap[INDEX(x+1, y)] == GOAL)
		count++;
	if(cmap[INDEX(x-1, y+1)] == GOAL)
		count++;
	if(cmap[INDEX(x+1, y-1)] == GOAL)
		count++;

	return count;
}

int LowVision::maxValue(int* array, int length)
{
	int max = array[0];       // start with max = first element

	for(int i = 1; i<length; i++)
	{
		if(array[i] > max)
			max = array[i];
	}
	return max;                // return highest value in array
}
int LowVision::minValue(int* array, int length)
{
	int min = array[0];       // start with max = first element

	for(int i = 1; i<length; i++)
	{
		if(array[i] < min)
			min = array[i];
	}
	return min;                // return highest value in array
}
/**************************************************************************************
 * Mutator methods for the bounding box variables
 **************************************************************************************/
int LowVision::getxMin()
{
	return xMin;
}
int LowVision::getyMin()
{
	return yMin;
}
int LowVision::getxMax()
{
	return xMax;
}
int LowVision::getyMax()
{
	return yMax;
}

int LowVision::getBallPresent()
{
	return ballPresent;
}

int LowVision::getHorMin(){
	return horMin;
}
int LowVision::getHorMax(){
	return horMax;
}
int LowVision::getbLeft(){
	return bLeft;
}
int LowVision::getbRight(){
	return bRight;
}
int LowVision::gettLeft(){
	return tLeft;
}
int LowVision::gettRight(){
	return tRight;
}

int LowVision::getScanLine()
{
	return scanLine;
}


/**************************************************************************************/
/**************************************************************************************/

int WritePPM(const rgb *img, int width, int height, const char *filename) {
	FILE *out;
	int wrote;

	out = fopen(filename, "wb");
	if (!out)
		return (0);

	fprintf(out, "P6\n%d %d\n%d\n", width, height, 255);
	wrote = fwrite(img, sizeof(rgb), width * height, out);
	fclose(out);

	return (wrote);
}

bool LowVision::saveThresholdImage(const char *filename) {
	rgb *buf;
	int wrote;

	buf = new rgb[width * height];
	if (!buf)
		return (false);

	IndexToRgb(buf, cmap, width, height, color, num_colors);
	wrote = WritePPM(buf, width, height, filename);
	delete[] buf;

	return (wrote > 0);
}

bool LowVision::saveColorizedImage(const char *filename, rgb *reg_color) {
	rgb *buf;
	int wrote;

	buf = new rgb[width * height];
	if (!buf)
		return (false);
	mzero(buf, width * height);

	RegionToRgbImage(buf, width, height, rmap, num_runs, reg_color);
	wrote = WritePPM(buf, width, height, filename);
	delete[] (buf);

	return (wrote > 0);
}

yuv LowVision::getImagePixel(int x, int y) {
#ifdef YUVPIXEL
	pixel p2 = buf[(y*width + x)];
	yuv p;
	p.y = p2.y;
#else
	pixel p2 = buf[(y * width + x) / 2];
	yuv p;
	p.y = (x & 1) ? p2.y2 : p2.y1;
#endif
	p.u = p2.u;
	p.v = p2.v;
	return (p);
}

Region *LowVision::findRegion(int x, int y) {
	int r = FindRun(rmap, 0, num_runs - 1, x, y);
	return ((r >= 0) ? &reg[rmap[r].parent] : NULL);
}

cmap_t LowVision::getTMap(int yi, int ui, int vi) {
	if (yi >= 0 && yi < tmap_y_size && ui >= 0 && ui < tmap_u_size && vi >= 0
			&& vi < tmap_v_size) {
		return (tmap[(yi << (bits_u + bits_v)) + (ui << bits_v) + vi]);
	} else {
		return (0);
	}
}

bool LowVision::setTMap(int yi, int ui, int vi, cmap_t val) {
	int l;
	if (yi >= 0 && yi < tmap_y_size && ui >= 0 && ui < tmap_u_size && vi >= 0
			&& vi < tmap_v_size) {
		l = (yi << (bits_u + bits_v)) + (ui << bits_v) + vi;
		if ((tmap[l] | 128) == (val | 128))
			return (false);
		tmap[l] = val;
		return (true);
	} else {
		return (false);
	}
}

int LowVision::setTMap(int yi, int ui, int vi, cmap_t val, int uv_radius) {
	int n = 0;
	for (int u = ui - uv_radius; u < ui + uv_radius + 1; u++) {
		for (int v = vi - uv_radius; v < vi + uv_radius + 1; v++) {
			n += setTMap(yi, u, v, val);
		}
	}
	return (n);
}

bool LowVision::clearDirtyTMap(int yi, int ui, int vi) {
	int l;
	if (yi >= 0 && yi < tmap_y_size && ui >= 0 && ui < tmap_u_size && vi >= 0
			&& vi < tmap_v_size) {
		l = (yi << (bits_u + bits_v)) + (ui << bits_v) + vi;
		if ((tmap[l] & 128) == 0)
			return (false);
		tmap[l] &= ~128;
		return (true);
	} else {
		return (false);
	}
}

bool LowVision::saveTMap(const char *tmapfile) const {
	return (CMVision::SaveThresholdFile(tmap, tmap_y_size, tmap_u_size,
			tmap_v_size, tmapfile));
}

int LowVision::countCMapPixels(int x, int y, int w, int h, cmap_t c,
		int &total_num) {
	// fix bounding area to be on image
	if (x < 0) {
		w += x;
		x = 0;
	}

	if (y < 0) {
		h += y;
		y = 0;
	}

	if (x + w >= width)
		w = width - x;
	if (y + h >= height)
		h = height - y;

	// count up matching color pixels
	int num = 0;

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			num += (getClassPixel(x + j, y + i) == c);
		}
	}

	total_num = w * h;
	return (num);
}

int LowVision::countTMapVoxels(int yi0, int yi1, int ui0, int ui1, int vi0,
		int vi1, cmap_t c, int &total_num) {
	// fix bounding area to be on valid tmap
	if (yi0 < 0)
		yi0 = 0;
	if (ui0 < 0)
		ui0 = 0;
	if (vi0 < 0)
		vi0 = 0;

	if (yi1 >= tmap_y_size)
		yi1 = tmap_y_size - 1;
	if (ui1 >= tmap_u_size)
		ui1 = tmap_u_size - 1;
	if (vi1 >= tmap_v_size)
		vi1 = tmap_v_size - 1;

	// count up matching entries
	int num = 0;

	for (int y = yi0; y <= yi1; y++) {
		for (int u = ui0; u <= ui1; u++) {
			for (int v = vi0; v <= vi1; v++) {
				num += (getTMapFast(y, u, v) == c);
			}
		}
	}

	total_num = max(yi1 - yi0 + 1, 0) * max(ui1 - ui0 + 1, 0) * max(vi1 - vi0
			+ 1, 0);

	return (num);
}

void LowVision::expandTMap(int x, int y, cmap_t c) {
	static const int N = 1;
	static const int M = 2;

	bool match = (getClassPixel(x, y) == c);

	if (!match) {
		int cmap_total = 0;
		int cmap_num = countCMapPixels(x - N, y - N, 2 * N + 1, 2 * N + 1, c,
				cmap_total);

		yuv p = getImagePixel(x, y);
		int yi = p.y >> (8 - bits_y);
		int ui = p.u >> (8 - bits_u);
		int vi = p.v >> (8 - bits_v);

		// drop out early if already set (since last update)
		if (getTMap(yi, ui, vi) == c)
			return;

		int tmap_total = 0;
		int tmap_num = countTMapVoxels(yi, yi, ui - M, ui + M, vi - M, vi + M,
				c, tmap_total);

		float cf = (float) (cmap_num - match) / (cmap_total - 1);
		float tf = (float) (tmap_num - match) / (tmap_total - 1);

		if (cf > 0.6 && tf > 0.25) {
			//bool b = setTMap(yi,ui,vi,c);
			LOG_DEBUG		("(%d %d) [%d %d %d] -> %d (%d)",
					x,y,yi,ui,vi,
					c,b)
		}

		if(false) {
			LOG_DEBUG("(%d %d) %d/%d %d/%d",
					x,y, cmap_num,cmap_total, tmap_num,tmap_total)
					LOG_DEBUG("cf=%f tf=%f",
							cf,tf)
		}
	}
}

void LowVision::expandTMap(cmap_t c) {
	for (int y = 1; y < height - 1; y++) {
		for (int x = 1; x < width - 1; x++) {
			expandTMap(x, y, c);
		}
	}
}

uint32_t LowVision::getStateHash() {
	uint32_t hv = 0;
	int sz = width * height;
	hv = CalcCRC32(hv, buf, sz / 2 * sizeof(pixel));
	hv = CalcCRC32(hv, cmap, sz * sizeof(cmap_t));
	hv = CalcCRC32(hv, rmap, num_runs * sizeof(Run));
	// hv = CalcCRC32(hv,reg,num_regions*sizeof(Region));
	return (hv);
}

void LowVision::dump() {
	LOG_INFO("image: %dx%d field=%d",width,height,field)
								LOG_INFO("runs: %d/%d (%1.2f%%)",
										num_runs,max_runs,100.0*num_runs/max_runs)
										LOG_INFO("regions: %d/%d (%1.2f%%)",
												num_regions,max_regions,100.0*num_regions/max_regions)
}

void LowVision::setupSegmentedImage() {
	segmentedColors = new RGB[num_colors];
	for (int i = 0; i < num_colors; i++) {
		segmentedColors[i].set(colorRgb[i].red, colorRgb[i].green,
				colorRgb[i].blue);
	}

	segmentedImage.setRGB(segmentedColors, num_colors);
}

const SegmentedImage & LowVision::getSegmentedImage() {
	segmentedImage.setIndices((char const *) cmap);
	return segmentedImage;
}

//====================================================================//
//    Region Filter Class Implementation
//====================================================================//

void RegionFilter::init(const Region *reg_list) {
	reg = reg_list;
	// skip too-large regions in sorted region list
	while (reg != NULL && reg->area > area.max)
		reg = reg->next;
}

const Region *RegionFilter::getNext() {
	// terminate when no regions, or no suitably large ones
	if (!reg)
		return (NULL);

	// find the next region matching our ranges
	while (reg) {
		w = reg->x2 - reg->x1 + 1;
		h = reg->y2 - reg->y1 + 1;

		if (reg->area < area.min)
			return (NULL);
		if (width.check(w) && height.check(h)) {
			const Region *match = reg;
			reg = reg->next;
			return (match);
		}

		reg = reg->next;
	}

	return (NULL);
}

bool RegionFilter::operator()(const Region &reg) const {
	int w = reg.x2 - reg.x1 + 1;
	int h = reg.y2 - reg.y1 + 1;

	return (area.check(reg.area) && width.check(w) && height.check(h));
}

//====================================================================//
//    Confidence Mask Class Implementation
//====================================================================//

void ConfMask::clear(float _def)
// set all queries to the following value
{
	reset();
	mask = &def;
	def = (int) (255.0 * _def);
	w = h = 1;
	vw = vh = 1;

	// make sure all query coordinates get zeroed
	xs = ys = 30;
}

void ConfMask::reset() {
	if (mask != &def)
		delete[] (mask);
	mask = NULL;
}

bool ConfMask::load(const char *filename, int _vw, int _vh)
// load an image mask
{
	// int fmt;
	reset();

	// load image and allocate
	vw = _vw;
	vh = _vh;
	mask = NULL; // ReadGrayscale(w,h,filename);
	if (!mask)
		goto error;

	// set up shift values
	xs = 0;
	while ((vw >> xs) > w)
		xs++;
	ys = 0;
	while ((vh >> ys) > h)
		ys++;

	if (vw % (1 << xs) != 0 || vh % (1 << ys) != 0) {
		//    LOG_ERROR("vw/h and vh/h must be powers of 2")
		//    LOG_ERROR("vw=%d vh=%d w=%d h=%d (xs=%d ys=%d)", vw, vh, w, h, xs, ys)
	}

	// delete(img);
	return (true);

	error: delete (mask);
	mask = NULL;
	return (false);
}

float ConfMask::get(float x, float y) {
	int ix, iy;
	uchar val;

	ix = ((int) x) >> xs;
	iy = ((int) y) >> ys;
	val = mask[iy * w + ix];

	//  LOG_DEBUG("%d %d %d", ix, iy, val)

	return (val / 255.0);
}

} // end namespace
