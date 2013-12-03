
#include "Input.h"

static float2 _pos;
static float2 _lastPos;
static mouse_button mbstate;

float2 mouse::position()
{
	return _pos;
}

float2 mouse::deltaPosition()
{
	float2 d;
	d.x = _lastPos.x - _pos.x;
	d.y = _lastPos.y - _pos.y;
	return d;
}
bool mouse::button_down(mouse_button button)
{
	return check_flag(mbstate, button);
}
bool mouse::button_up(mouse_button button)
{
	return check_flag(mbstate, button);
}

#ifdef WIN32
void mouse::___set_mouse_state(float2 pos, mouse_button mb)
{
	_lastPos = _pos;
	_pos = pos;
	mbstate = mb;
}
#endif
