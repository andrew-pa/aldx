
#include "mesh.h"


mesh::mesh(ComPtr<ID3D11Device> device, void* vertices, void* indices,
		   uint32 _indexCount, uint32 _vertexCount, size_t _vertexStride, std::string _mesh_name, D3D11_PRIMITIVE_TOPOLOGY _topo)
		   : indexCount(_indexCount), vertexCount(_vertexCount), vertexStride(_vertexStride), topology(_topo), _name(_mesh_name)
{
	D3D11_SUBRESOURCE_DATA vertex_srd = { 0 };
	vertex_srd.pSysMem = vertices;
	CD3D11_BUFFER_DESC vertex_bfds(_vertexStride * _vertexCount, D3D11_BIND_VERTEX_BUFFER);
	chr(device->CreateBuffer(&vertex_bfds, &vertex_srd, &vertexBuffer));
	D3D11_SUBRESOURCE_DATA index_srd = { 0 };
	index_srd.pSysMem = indices;
	CD3D11_BUFFER_DESC index_bfds(sizeof(UINT) * _indexCount, D3D11_BIND_INDEX_BUFFER);
	chr(device->CreateBuffer(&index_bfds, &index_srd, &indexBuffer));
}

void mesh::draw(ComPtr<ID3D11DeviceContext> context, UINT startidx, UINT basevtx)
{
	UINT stride = vertexStride;
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(topology);
	context->DrawIndexed(indexCount, startidx, basevtx);
}

void mesh_create_box(float w, float h, float d, 
			   void*& vertices, void*& indices, uint32& indexCount, uint32& vertexCount)
{
	dvertex* v = new dvertex[24];

	float w2 = 0.5f*w;
	float h2 = 0.5f*h;
	float d2 = 0.5f*d;
	
	// Fill in the front face vertex data.
	v[0] = dvertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[1] = dvertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[2] = dvertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[3] = dvertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	v[4] = dvertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[5] = dvertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[6] = dvertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[7] = dvertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the top face vertex data.
	v[8]  = dvertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[9]  = dvertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[10] = dvertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[11] = dvertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	v[12] = dvertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[13] = dvertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[14] = dvertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[15] = dvertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the left face vertex data.
	v[16] = dvertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[17] = dvertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[18] = dvertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[19] = dvertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the right face vertex data.
	v[20] = dvertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[21] = dvertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[22] = dvertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[23] = dvertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	UINT* i = new UINT[36];

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7]  = 5; i[8]  = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] =  9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	vertices = v;
	indices = i;
	indexCount = 36;
	vertexCount = 24;
}

void mesh_create_grid(float w, float d, UINT  m, UINT n, 
				void*& vertices, void*& indices, uint32& indexCount, uint32& vertexCount)
{
	UINT vtxc = m*n;
	UINT fc = (m-1)*(n-1)*2;
	float hw = 0.5f*w;
	float hd = 0.5f*d;
	float dx = w /  (n-1);
	float dz = d / (m-1);

	float du = 1.0f / (n-1);
	float dv = 1.0f / (m-1);

	dvertex* v = new dvertex[vtxc];

	for(UINT i = 0; i < m; ++i)
	{
		float z = hd - i*dz;
		for(UINT j = 0; j < n; ++j)
		{
			float x = -hw + j*dx;
			v[i*n+j].pos = float3(x, 0, z);
			v[i*n+j].norm = float3(0, 1, 0);
			v[i*n+j].texc = float2(j*du, i*dv);
		}
	}

	UINT* ix = new UINT[fc*3];

	UINT k = 0;
	for(UINT i = 0; i < m-1; ++i)
	{
		for(UINT j = 0; j < n-1; ++j)
		{
			ix[k] = i*n+j;
			ix[k+1] =i*n+j+1;
			ix[k+2] = (i+1)*n+j;
			ix[k+3] = (i+1)*n+j;
			ix[k+4] = i*n+j+1;
			ix[k+5] = (i+1)*n+j+1;
			k+=6;
		}
	}

	vertices = v;
	indices = ix;
	indexCount = fc*3;
	vertexCount = vtxc;
}

void mesh_create_sphere(float radius, UINT sliceCount, UINT stackCount, 
				  std::vector<dvertex>* pvertices, std::vector<uint>* pindices, uint32& indexCount, uint32& vertexCount)
{
	std::vector<dvertex>* vertices = pvertices;
	std::vector<UINT>* indices = pindices;

	dvertex topv(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	dvertex botv(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	vertices->push_back(topv);

	float dphi = XM_PI/stackCount;
	float dtheta = 2.0f*XM_PI/sliceCount;

	for(UINT i = 1; i <= stackCount-1; ++i)
	{
		float phi = i*dphi;
		for(UINT j = 0; j <= sliceCount; ++j)
		{
			float theta = j*dtheta;
			dvertex v;
			v.pos.x = radius * sinf(phi) * cosf(theta);
			v.pos.y = radius * cosf(phi);
			v.pos.z = radius * sinf(phi) * sinf(theta);
			auto P = XMLoadFloat3(&v.pos);
			XMStoreFloat3(&v.norm, XMVector3Normalize(P));
			v.texc = float2(theta / XM_2PI, phi / XM_PI);
			vertices->push_back(v);
		}
	}

	vertices->push_back(botv);

	for(UINT i = 1; i <= sliceCount; ++i)
	{
		indices->push_back(0);
		indices->push_back(i+1);
		indices->push_back(i);
	}

	UINT bi = 1;
	UINT rvc = sliceCount+1;
	for(UINT i = 0; i < stackCount-2; ++i)
	{
		for(uint j = 0; j < sliceCount; ++j)
		{
			indices->push_back(bi + i*rvc+j);
			indices->push_back(bi + i*rvc+j+1);
			indices->push_back(bi + (i+1)*rvc+j);
			indices->push_back(bi + (i+1)*rvc+j);
			indices->push_back(bi + (i*rvc + j+1));
			indices->push_back(bi + (i+1)*rvc+j+1);
		}
	}

	UINT spi = (UINT)vertices->size()-1;
	bi = spi - rvc;
	for(uint i = 0; i < sliceCount; ++i)
	{
		indices->push_back(spi);
		indices->push_back(bi+i);
		indices->push_back(bi+i+1);
	}

	indexCount = indices->size();
	vertexCount = vertices->size();
}
