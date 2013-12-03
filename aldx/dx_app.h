#pragma once

#include "helper.h"
#include <dwrite_1.h>
#include "input.h"

using Microsoft::WRL::ComPtr;

#define D2D
#define MSAA

#define INCLUDE_FPS_DRAW

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
#endif

//dx_app
// Handles all DirectX (Direct3D/Direct2D) setup. Subclass to create new applications, then override the pure functions. Also includes FPS measuring and display if D2D is enabled
class dx_app
{
public:
	dx_app(window_type window);
	virtual ~dx_app();

	virtual void create_device_res();
	virtual void create_window_size_depres();
	virtual void update_window_size();
	virtual void load() = 0;
	virtual void update(float t, float dt) = 0;
	virtual void render(float t, float dt);
	virtual void present();

#ifdef WIN32
	int run(HINSTANCE hInst, int showCmd, const wchar_t* windowTitle, int cliW, int cliH);
#endif

#ifdef D2D
	virtual void create_d2ddevice_indi_res();
#endif

protected:
	inline float conv_dips_pixels(float dips);
	ComPtr<ID3D11Device1> device;
	ComPtr<ID3D11DeviceContext1> context;
	ComPtr<IDXGISwapChain1> swapChain;
	ComPtr<ID3D11RenderTargetView> renderTargetView;
	ComPtr<ID3D11DepthStencilView> depthStencilView;

	void ApplyOMSettings();

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
	XMFLOAT4X4 orientationTransform;

	bool windowSizeChanged;

#if defined(D2D) && defined(INCLUDE_FPS_DRAW)
	bool drawFPSCounter;	
	ComPtr<IDWriteTextFormat> _debug_text_format;
	ComPtr<ID2D1SolidColorBrush> _debug_text_brush;
#endif
	float fps, mspf;

	bool vsync;
};