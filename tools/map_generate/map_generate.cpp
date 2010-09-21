#include <stdio.h>
#include "Vision/VisionObject/VisionObject.h"
#include "shared/ConfigFile/ConfigFile.h"
#include "Vision/rcahoon/Vision.h"

/*#define Y_BITS  4
#define U_BITS  6
#define V_BITS  6
#define Y_SIZE  (1<<Y_BITS)
#define U_SIZE  (1<<U_BITS)
#define V_SIZE  (1<<V_BITS)*/

/*struct ColorClass {
	// label color
	RGB color;
	
	// color range for classification
	int yl;
	int yu;
	int ul;
	int uu;
	int vl;
	int vu;
	
	// minimum region size for this class
	int min_size;
	
	// VisionObject Type
	VisionObject::Type vobj;
	
	ColorClass() {}
	
	ColorClass(int _yl, int _yu, int _ul, int _uu, int _vl, int _vu, int _r, int _g, int _b, int _min_size, VisionObject::Type _vobj)
		: color(_r, _g, _b),
		  yl(_yl>>(8-Y_BITS)), yu(_yu>>(8-Y_BITS)),
		  ul(_ul>>(8-U_BITS)), uu(_uu>>(8-U_BITS)),
		  vl(_vl>>(8-V_BITS)), vu(_vu>>(8-V_BITS)),
		  min_size(_min_size),
		  vobj(_vobj)
	{}
	
	bool match(int y, int u, int v)
	{
		return (y >= yl && y < yu) && (u >= ul && u < uu) && (v >= vl && v < vu);
	}
};*/

int main()
{
	ConfigFile configFile("../../robot/config/", "vision/vclasses.txt");
	unsigned char Color_Map[Y_SIZE*U_SIZE*V_SIZE];
	int num_classes = configFile.getInt("vision/numClasses");
	ColorClass classes[5];

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
	}

	for(int y=0; y < Y_SIZE; y++)
	for(int u=0; u < U_SIZE; u++)
	for(int v=0; v < V_SIZE; v++)
	{
		for(int c=0; c < num_classes; c++)
		{
			// last class defined will take precedence on overlapping ranges
			if (classes[c].match(y,u,v))
			{
				Color_Map[(y<<(U_BITS+V_BITS)) | (u<<V_BITS) | v] = c;
			}
		}
	}

	FILE* fid = fopen("threshold", "wb+");
	fwrite(Color_Map, sizeof(unsigned char), Y_SIZE*U_SIZE*V_SIZE, fid);
}
