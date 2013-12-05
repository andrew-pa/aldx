#pragma once

#include "helper.h"
#include "render_target_stack.h"

using Microsoft::WRL::ComPtr;

#define D2D
#define MSAA

#define DRAW_FPS 

#ifdef WIN32
#define USE_RAWINPUT
#endif

#ifdef WIN32
//rect
// Simple rectangle struct
struct rect
{
public:
	float width, height;
	rect() : width(0), height(0) {} 
	rect(float w, float h) : width(w), height(h) { }
};

typedef HWND window_type;
//for lack of anything better
#define DP_LOGICALDPI 96.0f
#endif

//dx_app
// Handles all DirectX (Direct3D/Direct2D) setup. Subclass to create new applications, then override the pure functions. Also includes FPS measuring and display if D2D is enabled
class dx_app : public render_target_stack
{
public:
	dx_app(
#ifdef MSAA
		int MSAA_level,
#endif
#ifdef DRAW_FPS
		bool draw_fps
#endif
		);
	virtual void create_device_res();
	virtual void create_window_size_depres();
	virtual void update_window_size();
	virtual void load() = 0;
	virtual void update(float t, float dt) = 0;
	virtual void render(float t, float dt);
	virtual void present();

#ifdef WIN32
	int run(HINSTANCE hInst, int showCmd, const wstring& windowTitle, int cliW, int cliH);
#endif

#ifdef D2D
	virtual void create_d2ddevice_indi_res();
#endif

	propr(render_target, current_render_target, override { return rtexsk.top(); })
	inline void pop_render_target() override
	{
		if (rtexsk.size() == 1) return; //maintain back buffer
		rtexsk.pop();
		set_render_target();
	}
	inline void push_render_target(ComPtr<ID3D11RenderTargetView> rtv, ComPtr<ID3D11DepthStencilView> dsv, 
		D3D11_VIEWPORT vp) override
	{
		rtexsk.push(render_target(rtv, dsv, vp));
		set_render_target();
	}
	inline void set_render_target() override
	{
		const float clearColor[] = { 0.8f, 0.5f, (float)rtexsk.size() / 5.f, 1.0f };
		context->ClearRenderTargetView(current_render_target().render_targetv.Get(), clearColor);
		context->ClearDepthStencilView(current_render_target().depth_stencil.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		context->OMSetRenderTargets(1, current_render_target().render_targetv.GetAddressOf(), current_render_target().depth_stencil.Get());
		context->RSSetViewports(1, &rtexsk.top().viewport);
	}
protected:
	ComPtr<ID3D11Device1> device;
	ComPtr<ID3D11DeviceContext1> context;
	ComPtr<IDXGISwapChain1> swapChain;
	ComPtr<ID3D11RenderTargetView> renderTargetView;
	ComPtr<ID3D11DepthStencilView> depthStencilView;

#ifdef MSAA
	ComPtr<ID3D11Texture2D> backBuffer;
	ComPtr<ID3D11Texture2D> offscreenBackBuffer;
	ComPtr<ID3D11RenderTargetView> offscreenRenderTargetView;
	uint msaa_level;
#endif

#ifdef D2D
	ComPtr<ID2D1Factory1> d2factory;
	ComPtr<IDWriteFactory> dwfactory;
	ComPtr<ID2D1Device> d2device;
	ComPtr<ID2D1DeviceContext> d2context;
	ComPtr<ID2D1Bitmap1> d2targetBitmap;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	rect renderTargetSize;
	rect windowBounds;
	HWND window;
//	XMFLOAT4X4 orientationTransform;

	bool windowSizeChanged;

#if defined(D2D) && defined(DRAW_FPS)
	bool drawFPSCounter;	
	ComPtr<IDWriteTextFormat> _debug_text_format;
	ComPtr<ID2D1SolidColorBrush> _debug_text_brush;
#endif
	float fps, mspf;

	bool vsync;

private:
	stack<render_target> rtexsk;

	void init(window_type win);
	inline float conv_dips_pixels(float dips)
	{
		static const float c = 96.0f;
		return floor(dips * DP_LOGICALDPI / c + 0.5f);
	}


};