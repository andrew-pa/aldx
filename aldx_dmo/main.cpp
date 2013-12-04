#define LINK_DIRECTX
#include <helper.h>
#include <dx_app.h>
#include <render_shader.h>
#include <constant_buffer.h>
#include <mesh.h>
#include <camera.h>

static const D3D11_INPUT_ELEMENT_DESC posnormtex_layout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
class simple_shader : public render_shader
{
	struct model_depd
	{
		float4x4 world;
		float4x4 inv_world;
		model_depd() { }
	};

	struct camera_depd
	{
		float4x4 view, proj;
		camera_depd() { }
	};

	constant_buffer<model_depd> model_depd_cb;
	constant_buffer<camera_depd> camera_depd_cb;
public:
	simple_shader(){}
	simple_shader(ComPtr<ID3D11Device> device, datablob<byte>* vs_data, datablob<byte>* ps_data,
		const D3D11_INPUT_ELEMENT_DESC inputLayout[], int inputLayoutLength)
		: render_shader(device, vs_data, ps_data, inputLayout, inputLayoutLength),
		model_depd_cb(device, 0, model_depd()), camera_depd_cb(device, 1, camera_depd())
	{ }
	
	inline void world(const float4x4& m) override
	{
		model_depd_cb.data().world = m;
		model_depd_cb.data().inv_world = inverse(m);
	}

	inline void view(const float4x4& m) override
	{
		camera_depd_cb.data().view = m;
	}

	inline void proj(const float4x4& m) override
	{
		camera_depd_cb.data().proj = m;
	}

	inline void bind(ComPtr<ID3D11DeviceContext> context) override
	{
		render_shader::bind(context);
		model_depd_cb.bind(context, ShaderStage::Vertex);
		camera_depd_cb.bind(context, ShaderStage::Vertex);
	}
	inline void unbind(ComPtr<ID3D11DeviceContext> context) override
	{
		render_shader::unbind(context);
		model_depd_cb.unbind(context, ShaderStage::Vertex);
		camera_depd_cb.unbind(context, ShaderStage::Vertex);
	}
	inline void update(ComPtr<ID3D11DeviceContext> context) override
	{
		model_depd_cb.update(context);
		camera_depd_cb.update(context);
	}
};

class aldx_demo_app : public dx_app
{
	simple_shader ss;
	camera cam;
	mesh* m;
public:
	aldx_demo_app() : dx_app(8, true), 
		cam(float3(0, 2.5f, -5), float3(0,0.1f,0), 0.1f, 1000, XMConvertToRadians(45.f)) {}
	void load() override
	{
		ss = simple_shader(device,
			read_data_from_package(L"simple_vs.cso"),
			read_data_from_package(L"simple_ps.cso"), posnormtex_layout, _countof(posnormtex_layout));

		m = mesh::create_box(device, 1, 1, 1);
	}
	void update(float t, float dt) override
	{
		if (windowSizeChanged)
		{
			cam.update_proj(windowBounds.width / windowBounds.height);
			windowSizeChanged = false;
		}
		cam.update_view();
	}
	void render(float t, float dt)
	{
		dx_app::render(t, dt);
		ss.view(cam.view());
		ss.proj(cam.proj());
		ss.bind(context);	
		
		float4x4 world;
		XMStoreFloat4x4(&world, XMMatrixRotationRollPitchYaw(t*.2f, t*.5f, t*.3f));
		ss.world(world);
		ss.update(context);
		m->draw(context);
		
		ss.unbind(context);
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