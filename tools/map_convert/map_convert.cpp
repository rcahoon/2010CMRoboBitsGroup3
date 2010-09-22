#include <stdio.h>

#define Y_BITS_C  4
#define U_BITS_C  8
#define V_BITS_C  8

#define Y_SIZE_C  (1<<Y_BITS_C)
#define U_SIZE_C  (1<<U_BITS_C)
#define V_SIZE_C  (1<<V_BITS_C)

#define Y_BITS_R  4
#define U_BITS_R  6
#define V_BITS_R  6

#define Y_SIZE_R  (1<<Y_BITS_R)
#define U_SIZE_R  (1<<U_BITS_R)
#define V_SIZE_R  (1<<V_BITS_R)

int main()
{
	unsigned char Color_Map[Y_SIZE_R*U_SIZE_R*V_SIZE_R];
	unsigned char tempMap[Y_SIZE_C*U_SIZE_C*V_SIZE_C];
	
	FILE* fid = fopen("threshold.tmap", "rb");
	fread(tempMap, sizeof(unsigned char), Y_SIZE_C*U_SIZE_C*V_SIZE_C, fid);
	fclose(fid);

	for(int y=0; y < Y_SIZE_R; y++)
	for(int u=0; u < U_SIZE_R; u++)
	for(int v=0; v < V_SIZE_R; v++)
	{
		Color_Map[(y<<(U_BITS_R+V_BITS_R)) | (u<<V_BITS_R) | v] = tempMap[(y<<(Y_BITS_C-Y_BITS_R)<<(U_BITS_R+V_BITS_R)) | (u<<(U_BITS_C-U_BITS_R)<<V_BITS_R) | (v<<(V_BITS_C-V_BITS_R))];
	}

	fid = fopen("threshold", "wb+");
	fwrite(Color_Map, sizeof(unsigned char), Y_SIZE_R*U_SIZE_R*V_SIZE_R, fid);
	fclose(fid);
}
