#pragma once
#include "helper.h"

namespace aldx
{
	struct dvertex
	{
		float3 pos;
		float3 norm;
		float2 texc;
		dvertex()
		{}
		dvertex(float px, float py, float pz, float nx, float ny, float nz, float3 col)
			: pos(px, py, pz), norm(nx, ny, nz), texc(0, 0)
		{
		}
		dvertex(float px, float py, float pz, float nx, float ny, float nz, float tx = 0, float ty = 0, float tz = 0, float u = 0, float v = 0)
			:pos(px, py, pz), norm(nx, ny, nz), texc(u, v)
		{
		}
	};

	//mesh
	// A mesh object, basically a index and vertex buffer. Try to use model instead
	//* !! Does not store CPU copies of Vertices or Indices !!
	class mesh
	{
		std::string _name;

		ComPtr<ID3D11Buffer> vertexBuffer;
		ComPtr<ID3D11Buffer> indexBuffer;
		uint32 indexCount;
		uint32 vertexCount;
		size_t vertexStride;
		D3D11_PRIMITIVE_TOPOLOGY topology;
	public:
		mesh() { }
		mesh(ComPtr<ID3D11Device> device, void* vertices, void* indices,
			uint32 indexCount, uint32 vertexCount, size_t vertexStride,
			std::string _mesh_name = "", D3D11_PRIMITIVE_TOPOLOGY _topo = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		void draw(ComPtr<ID3D11DeviceContext> context, UINT startidx = 0, UINT basevtx = 0);

		proprw(ComPtr<ID3D11Buffer>, vertex_buffer, { return vertexBuffer; })
			proprw(ComPtr<ID3D11Buffer>, index_buffer, { return indexBuffer; })
			proprw(uint32, index_count, { return indexCount; })
			proprw(uint32, vertex_count, { return vertexCount; })
			proprw(size_t, vertex_stride, { return vertexStride; })
			proprw(D3D11_PRIMITIVE_TOPOLOGY, prim_topology, { return topology; })
			proprw(std::string, name, { return _name; });

		static mesh* create_box(ComPtr<ID3D11Device> device, float w, float h, float d, const string& mesh_name = "box");
		static mesh* create_grid(ComPtr<ID3D11Device> device, float w, float d, uint m, uint n, const string& mesh_name = "grid");
		static mesh* create_sphere(ComPtr<ID3D11Device> device, float radius, uint sliceCount, uint stackCount, const string& mesh_name = "uv_sphere");
	};
}