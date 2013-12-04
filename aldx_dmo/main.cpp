#define LINK_DIRECTX
#include <helper.h>
#include <dx_app.h>

class aldx_demo_app : public dx_app
{
public:
	aldx_demo_app() : dx_app(4, true) {}
	void load() override
	{ }
	void update(float t, float dt) override
	{
	}
	void render(float t, float dt)
	{
		dx_app::render(t, dt);
	}
};

int CALLBACK WinMain(
	_In_  HINSTANCE inst,
	_In_  HINSTANCE pinst,
	_In_  LPSTR cmdLine,
	_In_  int cmds
	)
{
	aldx_demo_app app;
	return app.run(inst, cmds, L"ALDX demo app", 640, 480);
}