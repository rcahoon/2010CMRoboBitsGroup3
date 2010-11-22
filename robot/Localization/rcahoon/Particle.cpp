#include "Localization.h"
#include "shared/random.h"
#include <stdio.h>

namespace RCahoon {

/*static inline float pdf(float x, float var)
{
	return var/(var+x*x);
}*/

/*//TODO: do sensor fusion correctly
void Particle::update(Localization& loc, std::vector<WorldObject const *> world_objs, Particle delta)
{
//TODO: change to worldobjects
	pos_x += t_x;
	pos_y += t_y;
	angle += rot;
	
	if (belief() < 1.0f/NUM_PARTICLES) init(loc.field);
	
	if (vis_objs.size() > 0)
	{
		float dpos_x = 2.0f;
		float dpos_y = 2.0f;
		float dang = 0.2f;
		
		while(dpos_x > 1.0f || dpos_y > 1.0f || dang > 0.05f)
		{
			Vector2D grad(0,0);
			float moment = 0.0f;
		
			for(std::vector<VisionObject const *>::iterator iter = vis_objs.begin();
				iter != vis_objs.end(); iter++)
			//for(unsigned j=0; j < vis_objs.size(); j++)
			{
				const VisionObject* obj = *iter;
				//const VisionObject* obj = vis_objs[j];
				Vector2D position = this->convertRelativeToGlobal(obj->getPosition());
				loc.fieldBound(position);
				Vector2D vec;
				switch(obj->getType())
				{
					case VisionObject::Line:
						/*for(int i=0; i < FIELD_NUM_LINES; i++)
						{
							Vector2D p2, p1;
							field.getLineSegment(i, &p1, &p2);
							Vector2D vec1 = (p2 - p1);
							Vector2D vec2 = (p1 - position);
							float dist = vec1.cross(vec2) / vec1.length();
							float pr = pdf(dist, LINE_VAR);
							Vector2D subvec = vec1.perp();
							subvec *= subvec.dot(vec2);
							if (subvec.length() > 0.01)
								vec += subvec.norm(pr);
						}* /
						vec = loc.LINE_MAP(position);
					break;
					case VisionObject::BlueGoalPost:
						vec = (loc.field.getBlueGoalPostLeft() - position) + (loc.field.getBlueGoalPostRight() - position);
					break;
					case VisionObject::YellowGoalPost:
						vec = (loc.field.getYellowGoalPostLeft() - position) + (loc.field.getYellowGoalPostRight() - position);
					break;
					default:
						continue;
					break;
				}
				moment += obj->getConfidence() * obj->getPosition().cross(vec);
				grad += obj->getConfidence() * vec;
			}
			//TODO: hill climbing/loop
			// delta update
			if (dpos_x*grad.x < 0) dpos_x *= -0.5f;
			else dpos_x *= 1.2f;
			if (dpos_y*grad.y < 0) dpos_y *= -0.5f;
			else dpos_y *= 1.2f;
			if (dang*moment < 0) dang *= -0.5f;
			else dang *= 1.2f;
		
			pos_x += Noisy<float>(dpos_x, 0);
			pos_y += Noisy<float>(dpos_y, 0);
			angle += Noisy<float>(dang, 0);
			
			printf("%.2f %.2f %.2f\t| %.2f %.2f %.2f\t| %.2f %.2f %.2f\n",
			       pos_x.val(), pos_y.val(), angle.val(), grad.x, grad.y, moment, dpos_x, dpos_y, dang);
		}
	}
}*/

}
