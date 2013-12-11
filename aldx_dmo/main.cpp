#define LINK_DIRECTX
#include <helper.h>
#include <dx_app.h>
#include <render_shader.h>
#include <constant_buffer.h>
#include <states.h>
#include <texture2d.h>
#include <render_texture.h>
#include <mesh.h>
#include <camera.h>
#include <data_buffer.h>

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
	sampler_state samp;
	texture2d* tex;
public:
	simple_shader(){}
	simple_shader(ComPtr<ID3D11Device> device, datablob<byte>* vs_data, datablob<byte>* ps_data,
		const D3D11_INPUT_ELEMENT_DESC inputLayout[], int inputLayoutLength)
		: render_shader(device, vs_data, ps_data, inputLayout, inputLayoutLength),
		model_depd_cb(device, 0, model_depd()), camera_depd_cb(device, 1, camera_depd()),
		samp(device, 0)
	{ 
	}
	
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

	proprw(texture2d*, texture, { return tex; })

	inline void bind(ComPtr<ID3D11DeviceContext> context) override
	{
		render_shader::bind(context);
		model_depd_cb.bind(context, shader_stage::Vertex);
		camera_depd_cb.bind(context, shader_stage::Vertex);
		samp.bind(context, shader_stage::Pixel);
	}
	inline void unbind(ComPtr<ID3D11DeviceContext> context) override
	{
		render_shader::unbind(context);
		model_depd_cb.unbind(context, shader_stage::Vertex);
		camera_depd_cb.unbind(context, shader_stage::Vertex);
		samp.unbind(context, shader_stage::Pixel);
		tex->unbind(context, shader_stage::Pixel);
		
	}
	inline void update(ComPtr<ID3D11DeviceContext> context) override
	{
		model_depd_cb.update(context);
		camera_depd_cb.update(context);
		tex->bind(context, shader_stage::Pixel);
	}
};

class aldx_demo_app : public dx_app
{
	simple_shader ss;
	camera cam;
	mesh* m;
	mesh* g;
	render_texture* tex;
	texture2d* img;
public:
	aldx_demo_app() : dx_app(8, true), 
		cam(float3(0, 3.f, 6), float3(0,0.1f,0), 0.1f, 1000, to_radians(45.f)) {}
	void load() override
	{
		ss = simple_shader(device,
			read_data_from_package(L"simple_vs.cso"),
			read_data_from_package(L"simple_ps.cso"), posnormtex_layout, _countof(posnormtex_layout));

		tex = new render_texture(device, float2(512, 512));
		ss.texture() = tex;
		img = new texture2d(device, read_data_from_package(L"img.dds"));

		m = mesh::create_box(device, 1, 1, 1);
		g = mesh::create_grid(device, 16, 16, 4, 4, "ground");
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

	void render_scene(float t, float dt, const float4x4& view, const float4x4& proj)
	{
		ss.view(cam.view());
		ss.proj(cam.proj());
		ss.bind(context);

		ss.texture() = tex;
		ss.world(XMMatrixRotationRollPitchYaw(t*.3f, t*.7f, t*.4f) *
				 XMMatrixTranslation(0, 1, 0));
		ss.update(context);
		m->draw(context);

		ss.texture() = img;
		ss.world(float4x4::identity());
		ss.update(context);
		g->draw(context);

		tex->unbind(context, shader_stage::Pixel);
		ss.unbind(context);
	}

	void render(float t, float dt)
	{
		tex->push(this);
		render_scene(t, dt, cam.view(), cam.proj());
		pop_render_target();

		dx_app::render(t, dt);
		render_scene(t, dt, cam.view(), cam.proj());
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