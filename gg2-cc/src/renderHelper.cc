#include "renderHelper.h"

RenderPackage::RenderPackage():bitmap(NULL), center_x(0), center_y(0)
{
}


RenderUnit::RenderUnit(int type, int pic, int r, int g, int b, int t, float x,
	float y, float dx, float dy, float xscale, float yscale, float angle,
	bool relativeToMap):
	type(type), pic(pic), r(r), g(g), b(b), t(t), x(x), y(x), dx(dx), dy(dy),
	xscale(xscale), yscale(yscale), angle(angle),
	relativeToMap(relativeToMap)
{
} // RenderUnit()


