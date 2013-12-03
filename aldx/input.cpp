
#include "Input.h"

static float2 _pos;
static float2 _lastPos;
static MouseButton mbstate;

float2 Mouse::position()
{
	return _pos;
}

float2 Mouse::deltaPosition()
{
	float2 d;
	d.x = _lastPos.x - _pos.x;
	d.y = _lastPos.y - _pos.y;
	return d;
}
bool Mouse::IsButtonDown(MouseButton button)
{
	return check_flag(mbstate, button);
}
bool Mouse::IsButtonUp(MouseButton button)
{
	return check_flag(mbstate, button);
}

#ifdef WIN32
void Mouse::___SetMouseState(float2 pos, MouseButton mb)
{
	_lastPos = _pos;
	_pos = pos;
	mbstate = mb;
}
#endif
