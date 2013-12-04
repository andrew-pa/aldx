
#include "dx_app.h"
using namespace Microsoft::WRL;


dx_app::dx_app(
#ifdef MSAA
	 int MSAA_level,
#endif
#ifdef DRAW_FPS
	 bool draw_fps
#endif
	 )
	: windowSizeChanged(true), drawFPSCounter(draw_fps), vsync(true)
#ifdef MSAA
	, msaa_level(MSAA_level)
#endif
{
}

void dx_app::init(window_type win)
{
	window = win;
#ifdef D2D
	create_d2ddevice_indi_res();
#endif
	create_device_res();
	create_window_size_depres();
	load();
}


#ifdef D2D
void dx_app::create_d2ddevice_indi_res()
{
	D2D1_FACTORY_OPTIONS opt;
	ZeroMemory(&opt, sizeof(D2D1_FACTORY_OPTIONS));
#if defined(_DEBUG)
	opt.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif
	chr(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory1), &opt, &d2factory));
	chr(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &dwfactory));
}
#endif D2D

void  dx_app::create_device_res()
{
	UINT creflg = 0;
#ifdef D2D
	creflg |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#endif
#if defined(_DEBUG)
	creflg |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	ComPtr<ID3D11Device> xdev;
	ComPtr<ID3D11DeviceContext> xcntx;
	chr(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, creflg, 
		featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, 
		&xdev, &featureLevel, &xcntx));

	chr(xdev.As(&device));
	chr(xcntx.As(&context));

#ifdef D2D
	ComPtr<IDXGIDevice> dxgidevice;
	chr(device.As(&dxgidevice));
	chr(d2factory->CreateDevice(dxgidevice.Get(), &d2device));
	chr(d2device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2context));

#ifdef INCLUDE_FPS_DRAW
	dwfactory->CreateTextFormat(L"Segoe UI", 
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		14.f, L"en-US", ___fps_text_format.GetAddressOf());
	d2context->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), ___fps_text_brush.GetAddressOf());
#endif

#endif
}

void  dx_app::create_window_size_depres()
{
	windowSizeChanged = true;
	RECT cre;
	GetClientRect(window, &cre);
	windowBounds.width = (float)(cre.right - cre.left);
	windowBounds.height = (float)(cre.bottom - cre.top);

	float winwidth = conv_dips_pixels(windowBounds.width);
	float winheight = conv_dips_pixels(windowBounds.height);

	renderTargetView.Reset();
	depthStencilView.Reset();
#ifdef MSAA
	backBuffer.Reset();
#endif

	renderTargetSize.width = winwidth;
	renderTargetSize.height = winheight;

	DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
#ifdef D2D
		format = DXGI_FORMAT_B8G8R8A8_UNORM;
#endif

	if(swapChain != nullptr)
	{
		chr(swapChain->ResizeBuffers(2, 
			static_cast<UINT>(renderTargetSize.width), static_cast<UINT>(renderTargetSize.height), format, 0));
	}
	else
	{
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};
		swapChainDesc.Width = static_cast<UINT>(renderTargetSize.width);
		swapChainDesc.Height = static_cast<UINT>(renderTargetSize.height);
		swapChainDesc.Format = format;
		swapChainDesc.Stereo = false;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;
		swapChainDesc.Scaling = DXGI_SCALING_NONE;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		swapChainDesc.Flags = 0; 
		
		ComPtr<IDXGIDevice1> dxgid;
		chr(device.As(&dxgid));
		
		ComPtr<IDXGIAdapter> dxgiadp;
		chr(dxgid->GetAdapter(&dxgiadp));
		
		ComPtr<IDXGIFactory2> dxgif;
		chr(dxgiadp->GetParent(__uuidof(IDXGIFactory2), &dxgif));
		

		chr(dxgif->CreateSwapChainForHwnd(device.Get(), window, &swapChainDesc, nullptr, nullptr, &swapChain));

		//chr(dxgid->SetMaximumFrameLatency(2));
	}

#ifndef MSAA
	ComPtr<ID3D11Texture2D> backBuffer;
	chr(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer));

	chr(device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView));	
	
	CD3D11_TEXTURE2D_DESC depthStencilDesc(
		DXGI_FORMAT_D24_UNORM_S8_UINT, 
		static_cast<UINT>(renderTargetSize.width),
		static_cast<UINT>(renderTargetSize.height),
		1,
		1,
		D3D11_BIND_DEPTH_STENCIL
		);
	
	ComPtr<ID3D11Texture2D> depthStencil;
	chr(device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencil));

	CD3D11_DEPTH_STENCIL_VIEW_DESC dsvd(D3D11_DSV_DIMENSION_TEXTURE2D);
	chr(device->CreateDepthStencilView(depthStencil.Get(), &dsvd, &depthStencilView));
#else
	CD3D11_TEXTURE2D_DESC desc(DXGI_FORMAT_B8G8R8A8_UNORM, (UINT)renderTargetSize.width, (UINT)renderTargetSize.height, 1, 1);
		desc.BindFlags = D3D11_BIND_RENDER_TARGET;
		desc.SampleDesc.Count = msaa_level;
		
		chr(
			device->CreateTexture2D(
				&desc,
				nullptr,
				&offscreenBackBuffer
				)
			);

		chr(
			device->CreateRenderTargetView(
				offscreenBackBuffer.Get(),
				nullptr,
				&offscreenRenderTargetView
				)
			);

		chr(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer));
		chr(device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView));

			// Create a depth stencil view.
	CD3D11_TEXTURE2D_DESC depthStencilDesc(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		static_cast<UINT>(renderTargetSize.width),
		static_cast<UINT>(renderTargetSize.height),
		1,
		1,
		D3D11_BIND_DEPTH_STENCIL
		);

	depthStencilDesc.SampleDesc.Count = msaa_level;

	ComPtr<ID3D11Texture2D> depthStencil;
	chr(
		device->CreateTexture2D(
			&depthStencilDesc,
			nullptr,
			&depthStencil
			)
		);

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2DMS);
	chr(
		device->CreateDepthStencilView(
			depthStencil.Get(),
			&depthStencilViewDesc,
			&depthStencilView
			)
		);

#endif

	CD3D11_VIEWPORT viewport(
		0.0f,
		0.0f,
		renderTargetSize.width,
		renderTargetSize.height
		);

	render_target_stack = stack<render_target>();
	push_render_target(
#ifdef MSAA
		offscreenRenderTargetView, 
#else
		renderTargetView
#endif
		depthStencilView, viewport);

	context->RSSetViewports(1, &viewport);
#ifdef D2D
	D2D1_BITMAP_PROPERTIES1 bitprop =
		D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
		DP_LOGICALDPI,
		DP_LOGICALDPI);
	ComPtr<IDXGISurface> dxgibb;
#ifdef MSAA
	chr(offscreenBackBuffer.As(&dxgibb));
#else
	chr(backBuffer.As(&dxgibb));
#endif
	chr(d2context->CreateBitmapFromDxgiSurface(dxgibb.Get(), &bitprop, &d2targetBitmap));
	d2context->SetTarget(d2targetBitmap.Get());
	d2context->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
#endif
}

void  dx_app::update_window_size()
{
		RECT cre;
	GetClientRect(window, &cre);
	float nw = (float)(cre.right - cre.left);
	float nh = (float)(cre.bottom - cre.top);
	if(nw == windowBounds.width || nh == windowBounds.height) return;
		ID3D11RenderTargetView* nullviews[] = { nullptr };
		context->OMSetRenderTargets(ARRAYSIZE(nullviews), nullviews, nullptr);
		context->Flush();
#ifdef D2D
		d2context->SetTarget(nullptr);
		d2targetBitmap = nullptr;
		d2context->SetDpi(DP_LOGICALDPI, DP_LOGICALDPI);
#endif
		create_window_size_depres();
}

void  dx_app::ApplyOMSettings()
{
#ifdef MSAA
	context->OMSetRenderTargets(1, offscreenRenderTargetView.GetAddressOf(), depthStencilView.Get());
#else
	context->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());
#endif
		CD3D11_VIEWPORT viewport(
		0.0f,
		0.0f,
		renderTargetSize.width,
		renderTargetSize.height
		);
		context->RSSetViewports(1, &viewport); 
}

void dx_app::present()
{
#if defined(D2D) && defined(INCLUDE_FPS_DRAW)
	//Hack to get FPS on top of everything else
	if(drawFPSCounter)
	{
		d2context->BeginDraw();
		std::wstringstream wss;
		wss << "FPS: " << fps << " Frame Time: " << mspf << "\n";
		d2context->DrawTextW(wss.str().c_str(), wss.str().length(), ___fps_text_format.Get(), 
			D2D1::RectF(0, 0, 256, 32), ___fps_text_brush.Get());
		d2context->EndDraw();
	}
#endif

#ifdef MSAA
	context->ResolveSubresource(backBuffer.Get(), 0, offscreenBackBuffer.Get(), 0, DXGI_FORMAT_B8G8R8A8_UNORM);
#endif

	DXGI_PRESENT_PARAMETERS p = { 0 };
	
	auto hr = swapChain->Present1((vsync ? 1 : 0), 0, &p);
	context->DiscardView(renderTargetView.Get());
	context->DiscardView(depthStencilView.Get());

	if(hr == DXGI_ERROR_DEVICE_REMOVED)
	{
		windowBounds.width = 0;
		windowBounds.height = 0;
		swapChain = nullptr;
		
		create_device_res();
		create_d2ddevice_indi_res();
		update_window_size();
		load();
	}
	else
	{
		chr(hr);
	}
}

void  dx_app::render(float t, float dt)
{
	static uint64_t frameCnt = 0;
	static float timElp = 0.0f;
	frameCnt++;
	if((t - timElp) >= 1.0f)
	{
		mspf = 1000.0f / (float)frameCnt;
		fps = (float)frameCnt;
		frameCnt = 0;
		timElp += 1.0f;
	}
	const float clearColor[] = { 0.8f, 0.5f, 0.0f, 1.0f };
	context->ClearRenderTargetView(current_render_target().render_targetv.Get(), clearColor);
	context->ClearDepthStencilView(current_render_target().depth_stencil.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	context->OMSetRenderTargets(1, current_render_target().render_targetv.GetAddressOf(), current_render_target().depth_stencil.Get());
	context->RSSetViewports(1, &render_target_stack.top().viewport);
}

//WIN32 stuff
//Includes a bunch of bad weird hacks including globals and a static app var
#ifdef WIN32
#include "timer.h"
#include <windowsx.h>
#include "input.h"
static bool paused = false;
static bool resizeing = false;
static  dx_app* app =nullptr;
static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_ACTIVATE:
		if(LOWORD(wParam) == WA_INACTIVE)
		{
			paused = true;
		}
		else
		{
			paused = false;
		}
		return 0;

	case WM_SIZE:
		if(app)
		{
			if(wParam == SIZE_MINIMIZED)
			{
				paused = true;
			}
			else if(wParam == SIZE_MAXIMIZED)
			{
				paused = false;
				if(app != nullptr)
					app->update_window_size();
			}
			else if(wParam == SIZE_RESTORED)
			{
				paused = false;
				if(resizeing) 
				{
				}
				else
				{
					if(app != nullptr)
						app->update_window_size();
				}
			}
		}
		return 0;
	case WM_ENTERSIZEMOVE:
		paused = true;
		resizeing = true;
		return 0;
	case WM_EXITSIZEMOVE:
		paused = false;
		resizeing = false;
		if(app != nullptr)
			app->update_window_size();
		return 0;
	
#ifdef USE_RAWINPUT
	case WM_INPUT:
		{
			static float2 m;
			uint size;
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));
			byte* data = new byte[size];
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, data, &size, sizeof(RAWINPUTHEADER));
			RAWINPUT* rin = (RAWINPUT*)data;

			if(rin->header.dwType == RIM_TYPEMOUSE)
			{
				float mx = (float)rin->data.mouse.lLastX;
				float my = (float)rin->data.mouse.lLastY;
				if(check_flag(rin->data.mouse.usFlags, (USHORT)MOUSE_MOVE_RELATIVE))
				{
					m.x += mx;
					m.y += my;
				}
				else
				{
					m.x = mx;
					m.y = my;
				}
				mouse_button mb = mouse_button::none;
				if(check_flag(rin->data.mouse.usButtonFlags, 
					(USHORT)RI_MOUSE_LEFT_BUTTON_DOWN))
				{
					mb = mb | mouse_button::left;
				}
				if(check_flag(rin->data.mouse.usButtonFlags, 
					(USHORT)RI_MOUSE_RIGHT_BUTTON_DOWN))
				{
					mb = mb | mouse_button::right;
				}
				if(check_flag(rin->data.mouse.usButtonFlags, 
					(USHORT)RI_MOUSE_MIDDLE_BUTTON_DOWN))
				{
					mb = mb | mouse_button::middle;
				}
				if(check_flag(rin->data.mouse.usButtonFlags, 
					(USHORT)RI_MOUSE_BUTTON_4_DOWN))
				{
					mb = mb | mouse_button::x1;
				}
				if(check_flag(rin->data.mouse.usButtonFlags, 
					(USHORT)RI_MOUSE_BUTTON_5_DOWN))
				{
					mb = mb | mouse_button::x2;
				}
				
				mouse::___set_mouse_state(float2(mx, my), mb);
			}
		}
		return 0;
#endif

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int dx_app::run(HINSTANCE hInst, int showCmd, const wchar_t* windowTitle, int cliW, int cliH)
{
	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = MainWndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInst;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = L"DXAppWndClass";

	if(!RegisterClass(&wc))
	{
		throw exception("Failed to Register window class");
	}

	RECT r = { 0, 0, cliW, cliH };
	AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, false);
	int w = r.right - r.left;
	int h = r.bottom - r.top;

	auto ti = new timer();

	HWND wnd = CreateWindow(L"DXAppWndClass", windowTitle, WS_OVERLAPPEDWINDOW, 100,
		100, w, h, 0, 0, hInst, 0);
	if(!wnd)
	{
		throw exception("Failed to create window");
	}

	ShowWindow(wnd, showCmd);
	UpdateWindow(wnd);

	app = this;

#ifdef USE_RAWINPUT
	RAWINPUTDEVICE rid[1];
	rid[0].usUsagePage = 0x01; //Mouse pointer
	rid[0].usUsage = 0x02;
	rid[0].dwFlags = 0;
	rid[0].hwndTarget = wnd;
	if(!RegisterRawInputDevices(rid, 1, sizeof(rid[0])))
		throw exception("Failed to register Raw Input");
#endif
	
	this->init(wnd);

	MSG msg = {0};
	ti->reset();
	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 
		{
			if(!paused)
			{
				ti->update();
				this->update(ti->total_time(), ti->delta_time());
				this->render(ti->total_time(), ti->delta_time());
				this->present();
			}
			else
			{
				Sleep(100);
			}
		}
	}

	CloseWindow(wnd);
	return (int)msg.wParam;
}
#endif