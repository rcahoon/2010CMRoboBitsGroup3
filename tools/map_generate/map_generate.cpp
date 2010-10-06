#include <stdio.h>
#include "Vision/VisionObject/VisionObject.h"
#include "shared/ConfigFile/ConfigFile.h"
#include "Vision/rcahoon/Vision.h"

struct ColorClass {
	// color range for classification
	int yl;
	int yu;
	int ul;
	int uu;
	int vl;
	int vu;
	
	ColorClass() {}
	
	ColorClass(int _yl, int _yu, int _ul, int _uu, int _vl, int _vu)
		: yl(_yl>>(8-Y_BITS)), yu(_yu>>(8-Y_BITS)),
		  ul(_ul>>(8-U_BITS)), uu(_uu>>(8-U_BITS)),
		  vl(_vl>>(8-V_BITS)), vu(_vu>>(8-V_BITS))
	{}
	
	bool match(int y, int u, int v)
	{
		return (y >= yl && y < yu) && (u >= ul && u < uu) && (v >= vl && v < vu);
	}
};

int main()
{
	ConfigFile configFile("./", "vclasses.txt");
	unsigned char Color_Map[Y_SIZE*U_SIZE*V_SIZE];
	int num_classes = configFile.getInt("vision/numClasses");
	ColorClass classes[8];

	for(int i=0; i < num_classes; i++)
	{
		std::string path = "vision/classes/"; path += (char)(i+'0');
		classes[i] = ColorClass(
			configFile.getInt(path + "/yl"),
			configFile.getInt(path + "/yu"),
			configFile.getInt(path + "/ul"),
			configFile.getInt(path + "/uu"),
			configFile.getInt(path + "/vl"),
			configFile.getInt(path + "/vu")
		);
		printf("%s  (%d,%d,%d)-(%d,%d,%d)\n", path.c_str(), classes[i].yl, classes[i].ul, classes[i].vl, classes[i].yu,
																	classes[i].uu, classes[i].vu);
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
	fclose(fid);
}
