#pragma once
#include <DirectXMath.h>
#include <iostream>

using namespace DirectX;

namespace aldx
{

	typedef XMVECTOR mvector;
	typedef XMMATRIX matrix;
	//typedef XMFLOAT3 float3;
	struct float2 : public XMFLOAT2
	{
		float2(float x, float y) : XMFLOAT2(x, y) { }
		explicit float2(float xy) : XMFLOAT2(xy, xy) {}
		float2() : XMFLOAT2(0, 0) { }
		float2(const mvector& m)
		{
			XMStoreFloat2(this, m);
		}

		inline operator mvector()
		{
			return XMLoadFloat2(this);
		}
	};
	inline std::ostream& operator <<(std::ostream& os, const float2& v)
	{
		os << "(" << v.x << ", " << v.y << ")";
		return os;
	}

	struct float3 : public XMFLOAT3
	{
		float3(float x, float y, float z) : XMFLOAT3(x, y, z) { }
		explicit float3(float xyz) : XMFLOAT3(xyz, xyz, xyz) {}
		float3() : XMFLOAT3(0, 0, 0) { }
		float3(float2 xy, float z) : XMFLOAT3(xy.x, xy.y, z) {}
		float3(const mvector& m)
		{
			XMStoreFloat3(this, m);
		}

		inline operator mvector()
		{
			return XMLoadFloat3(this);
		}
		inline operator mvector() const
		{
			return XMLoadFloat3(this);
		}
	};
	inline std::ostream& operator <<(std::ostream& os, const float3& v)
	{
		os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
		return os;
	}

	struct float4 : public XMFLOAT4
	{
		float4(float x, float y, float z, float w) : XMFLOAT4(x, y, z, w) { }
		float4() : XMFLOAT4(0, 0, 0, 0) { }
		float4(float2 xy, float z, float w) : XMFLOAT4(xy.x, xy.y, z, w) {}
		float4(float2 xy, float2 zw) : XMFLOAT4(xy.x, xy.y, zw.x, zw.y) {}
		float4(float3 xyz, float w) : XMFLOAT4(xyz.x, xyz.y, xyz.z, w) {}
		explicit float4(float xyzw) : XMFLOAT4(xyzw, xyzw, xyzw, xyzw){}
		float4(const mvector& m)
		{
			XMStoreFloat4(this, m);
		}

		inline operator mvector()
		{
			return XMLoadFloat4(this);
		}
	};
	inline std::ostream& operator <<(std::ostream& os, const float4& v)
	{
		os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
		return os;
	}

	struct float4x4 : public XMFLOAT4X4
	{
		float4x4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33) :
		XMFLOAT4X4(m00, m01, m02, m03,
		m10, m11, m12, m13,
		m20, m21, m22, m23,
		m30, m31, m32, m33){}
		float4x4() : XMFLOAT4X4() { }
		float4x4(const matrix& m)
		{
			XMStoreFloat4x4(this, m);
		}

		inline operator matrix()
		{
			return XMLoadFloat4x4(this);
		}

		static inline float4x4 identity()
		{
			return float4x4(1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1);
		}
	};


#pragma region DXMath hacks

#ifndef loadX
#define loadX
#define load2(x) XMVECTOR v##x = XMLoadFloat2(&x)
#define load3(x) XMVECTOR v##x = XMLoadFloat3(&x)
#define load3as(x, n) XMVECTOR n = XMLoadFloat3(&(x));
#define load4(x) XMVECTOR v##x = XMLoadFloat4(&x);
#define load4x4(x) XMMATRIX m##x = XMLoadFloat4x4(&x);
#endif

#ifndef storeX
#define storeX
	//#define store2(x) XMStoreFloat2(&(x), v##x);
	//#define store3(x) XMStoreFloat3(&(x), v##x);
	//#define store4(x) XMStoreFloat4(&(x), v##x);
#define store2(x, r) XMStoreFloat2(&(x), (r));
#define store3(x, r) XMStoreFloat3(&(x), (r));
#define store4(x, r) XMStoreFloat4(&(x), (r));
#define store4x4(x, r) XMStoreFloat4x4(&(x), (r));
#endif

#ifndef returnX
#define returnX
#define return2(r) { float2 ret; store2(ret, r); return ret; }
#define return3(r) { float3 ret; store3(ret, r); return ret; }
#define return4(r) { float4 ret; store4(ret, r); return ret; }
#define return4x4(r) { float4x4 ret; store4x4(ret, r); return ret; }
#endif

	inline float to_radians(float degrees)
	{
		return degrees * (XM_PI / 180.f);
	}

	//clamp : clamp a floating point value
	inline float clamp(float x, float max = 1.0f, float min = -1.0f)
	{
		if (x > max) return max;
		else if (x < min) return min;
		return x;
	}

	inline float3 f4wf3(const float4& b)
	{
		return float3(b.x, b.y, b.z);
	}
	inline float4 f3wf4(const float3& b, float w = 0.f)
	{
		return float4(b.x, b.y, b.z, w);
	}

	//inline float3 operator+(float3 a, float3 b)
	//{
	//	load3(a); load3(b);
	//	float3 ret; store3(ret, XMVectorAdd(va, vb)); return ret;
	//}
	//inline float3 operator*(float3 a, float3 b)
	//{
	//	load3(a); load3(b);
	//	float3 ret; store3(ret, va*vb); return ret;
	//}
	//inline float3 operator-(float3 a, float3 b)
	//{
	//	load3(a); load3(b);
	//	float3 ret; store3(ret, XMVectorAdd(va, vb)); return ret;
	//}
	//inline float4 operator+(float4 a, float4 b)
	//{
	//	load4(a); load4(b);
	//	float4 ret; store4(ret, XMVectorSubtract(va, vb)); return ret;
	//}
	//inline float4 operator-(float4 a, float4 b)
	//{
	//	load4(a); load4(b);
	//	float4 ret; store4(ret, XMVectorSubtract(va, vb)); return ret;
	//}
	//inline float3 operator*(float3 a, float b)
	//{
	//	load3(a);
	//	return3(XMVectorScale(va, b));
	//}
	//inline float3 operator*(float a, float3 b)
	//{
	//	load3(b);
	//	return3(XMVectorScale(vb, a));
	//}
	//inline float4 operator*(float4 a, float b)
	//{
	//	load4(a);
	//	return4(XMVectorScale(va, b));
	//}
	//inline float4 operator*(float a, float4 b)
	//{
	//	load4(b);
	//	return4(XMVectorScale(vb, a));
	//}
	//inline float3 operator-(float3 a)
	//{
	//	load3(a);
	//	return3(XMVectorNegate(va));
	//}
	//inline float4 operator-(float4 a)
	//{
	//	load4(a);
	//	return4(XMVectorNegate(va));
	//}

	/*inline XMVECTOR operator*(FXMVECTOR a, float b)
	{
		return XMVectorScale(a, b);
	}
	inline XMVECTOR operator*(float b, FXMVECTOR a)
	{
		return XMVectorScale(a, b);
	}*/


	inline float dot(float3 a, float3 b)
	{
		load3(a);
		load3(b);
		return XMVectorGetX(XMVector3Dot(va, vb));
	}
	inline float dot(float4 a, float4 b)
	{
		load4(a);
		load4(b);
		return XMVectorGetX(XMVector2Dot(va, vb));
	}

	inline float3 cross(float3 a, float3 b)
	{
		load3(a); load3(b);
		return3(XMVector3Cross(va, vb));
	}
	inline float4 cross(float4 a, float4 b)
	{
		load4(a); load4(b);
		return4(XMVector3Cross(va, vb));
	}

	inline float3 normalize(float3 a)
	{
		load3(a);
		return3(XMVector3Normalize(va));
	}
	inline float4 normalize(float4 a)
	{
		load4(a);
		return4(XMVector4Normalize(va));
	}

	inline float4x4 operator*(float4x4& a, float4x4& b)
	{
		load4x4(a); load4x4(b);
		return4x4(XMMatrixMultiply(ma, mb));
	}

	inline float4x4 transpose(const float4x4& a)
	{
		load4x4(a);
		return4x4(XMMatrixTranspose(ma));
	}
	inline float4x4 inverse(const float4x4& a)
	{
		load4x4(a);
		return4x4(XMMatrixInverse(nullptr, ma));
	}

	inline float length(float3 a)
	{
		load3(a);
		return XMVectorGetX(XMVector3Length(va));
	}

	//inline mvector vf3(float3& a)
	//{
	//	load3(a);
	//	return va;
	//}
	//
	//inline mvector vf4(float4& a)
	//{
	//	load4(a);
	//	return va;
	//}

#pragma endregion
}