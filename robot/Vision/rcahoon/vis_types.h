#ifndef RCAHOON_VIS_TYPES_H_
#define RCAHOON_VIS_TYPES_H_

#include "Vision/SegmentedImage/RGB.h"
#include "Vision/VisionObject/VisionObject.h"
#include "Vision.h"

typedef unsigned char uchar;

namespace RCahoon {

struct yuyv {
	uchar y,u,y2,v;
};

typedef yuyv pixel;

struct pixel_run {
	short start, end;    // x range of run
	short x1,x2,y1,y2;   // bounding box of region (x1,y1) - (x2,y2)
	VisionObject::Type type; // which object class this region represents
	short rank;          // union-tree rank, negative for child nodes
	pixel_run* region;   // parent of this run in the region
	
	//float a, b, c;
	//float wcen_x,wcen_y; // weighted centroid position (i.e. centroid position * area)
	int area;            // occupied area in pixels
	
	float span()
	{
		//return end - start;
		return area /(float) (y2-y1 +1) + 1;
	}
	
	void set(VisionObject::Type _type, short _start, short _end, short _row)
	{
		type = _type;
		x1 = start = _start;
		x2 = end = _end;
		area = x2 - x1;
		//wcen_x = area*(x1 + x2)/2.0f;
		y1 = y2 = _row;
		//wcen_y = area*_row;
		/*if (_type==VisionObject::Line)
		{
			a = (area+1)*(2*_start*_start+_start*(2*_end-1)+_end*(2*end+1))/6;
			b = (_start+_end)*(area+1)*_row/2;
			c = area*_row*_row;
		}*/
		rank = 0;
		region = this;
	}
	
	pixel_run& doUnion(pixel_run& r)
	{
		if (this==&r) // tried to union with self; not an error
			return *this;
		
		if ((r.rank < 0) || (rank < 0))
			return canon()->doUnion(*r.canon());
		if (r.rank > rank)
			return r.doUnion(*this);
		
		if (r.rank == rank)
			rank++;
		
		x1 = std::min(x1, r.x1);
		x2 = std::max(x2, r.x2);
		y1 = std::min(y1, r.y1);
		y2 = std::max(y2, r.y2);
		//wcen_x += r.wcen_x;
		//wcen_y += r.wcen_y;
		/*if (type==VisionObject::Line)
		{
			a += r.a;
			b += r.b;
			c += r.c;
		}*/
		area += r.area;
		
		r.region = region;
		r.rank = -1;
		
		return *this;
	}
	
	/*float getOrient()
	{
		return atan(b/(a-c))/2.0f;
	}*/

	pixel_run* canon()
	{
		if (rank >= 0) return this;
		return ( region = region->canon() );
	}
};

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

struct ColorClass {
	// label color
	RGB color;
	
	// minimum region size for this class
	int min_size;
	
	ColorClass() {}
	
	ColorClass(int _r, int _g, int _b, int _min_size)
		: color(_r, _g, _b), min_size(_min_size)
	{}
};

} // end namespace

#endif /* RCAHOON_VIS_TYPES_H_ */
