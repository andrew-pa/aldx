
#include "camera.h"


camera::camera(float3 pos, float3 look, float n, float f, float fov)
	: _position(pos), _right(-1,0,0), _up(0,1,0), _look(look), _nearp(n), _farp(f), _fovAngle(fov)
{
	load3as(pos, P);
	load3as(look, L);
	mvector up = XMVectorSet(0,1,0, 0);
	LookAt(P, L, up);
}

void camera::UpdateProj(float aspectRatio)
{
	XMStoreFloat4x4(&_proj, 
		XMMatrixPerspectiveFovLH(_fovAngle, aspectRatio, _nearp, _farp));
}

void camera::UpdateView()
{
	mvector r = XMLoadFloat3(&_right);
	mvector u = XMLoadFloat3(&_up);
	mvector l = XMLoadFloat3(&_look);
	mvector p = XMLoadFloat3(&_position);

	//Reorthonormalize the Look/Up/Right vectors
	l = XMVector3Normalize(l);
	u = XMVector3Normalize(XMVector3Cross(l, r));
	r = XMVector3Cross(u, l);

	float x = -XMVectorGetX(XMVector3Dot(p, r));
	float y = -XMVectorGetX(XMVector3Dot(p, u));
	float z = -XMVectorGetX(XMVector3Dot(p, l));

	XMStoreFloat3(&_right, r);
	XMStoreFloat3(&_up, u);
	XMStoreFloat3(&_look, l);

	//Create view matrix
	_view(0, 0) = _right.x;	_view(0, 1) = _up.x;	_view(0, 2) = _look.x;	_view(0, 3) = 0.0f;
	_view(1, 0) = _right.y;	_view(1, 1) = _up.y;	_view(1, 2) = _look.y;	_view(1, 3) = 0.0f;
	_view(2, 0) = _right.z;	_view(2, 1) = _up.z;	_view(2, 2) = _look.z;	_view(2, 3) = 0.0f;
	_view(3, 0) = x;			_view(3, 1) = y;		_view(3, 2) = z;			_view(3, 3) = 1.0f;
}

void camera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{
	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, L));
	XMVECTOR U = XMVector3Cross(L, R);

	XMStoreFloat3(&_position, pos);
	XMStoreFloat3(&_look, L);
	XMStoreFloat3(&_right, R);
	XMStoreFloat3(&_up, U);
}

void camera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	XMVECTOR P = XMLoadFloat3(&pos);
	XMVECTOR T = XMLoadFloat3(&target);
	XMVECTOR U = XMLoadFloat3(&up);

	LookAt(P, T, U);
}

void camera::Target(float3 targ)
{
	LookAt(_position, targ, _up);
}

void camera::Forward(float d)
{
	mvector s = XMVectorReplicate(d);
	mvector l = XMLoadFloat3(&_look);
	mvector p = XMLoadFloat3(&_position);
	XMStoreFloat3(&_position, XMVectorMultiplyAdd(s, l, p));
}

void camera::Strafe(float d)
{
	mvector s = XMVectorReplicate(d);
	mvector r = XMLoadFloat3(&_right);
	mvector p = XMLoadFloat3(&_position);
	XMStoreFloat3(&_position, XMVectorMultiplyAdd(s, r, p));
}

void camera::Yaw(float angle)
{
	matrix r = XMMatrixRotationAxis(XMLoadFloat3(&_up), angle);
	XMStoreFloat3(&_right, XMVector3TransformNormal(XMLoadFloat3(&_right), r));
	XMStoreFloat3(&_up, XMVector3TransformNormal(XMLoadFloat3(&_up), r));
	XMStoreFloat3(&_look, XMVector3TransformNormal(XMLoadFloat3(&_look), r));
}
void camera::Pitch(float angle)
{
	matrix r = XMMatrixRotationAxis(XMLoadFloat3(&_right), angle);
	XMStoreFloat3(&_right, XMVector3TransformNormal(XMLoadFloat3(&_right), r));
	XMStoreFloat3(&_up, XMVector3TransformNormal(XMLoadFloat3(&_up), r));
	XMStoreFloat3(&_look, XMVector3TransformNormal(XMLoadFloat3(&_look), r));
}
void camera::Roll(float angle)
{
	matrix r = XMMatrixRotationAxis(XMLoadFloat3(&_look), angle);
	XMStoreFloat3(&_right, XMVector3TransformNormal(XMLoadFloat3(&_right), r));
	XMStoreFloat3(&_up, XMVector3TransformNormal(XMLoadFloat3(&_up), r));
	XMStoreFloat3(&_look, XMVector3TransformNormal(XMLoadFloat3(&_look), r));
}

void camera::RotateWorldY(float angle)
{
	matrix r = XMMatrixRotationY(angle);
	XMStoreFloat3(&_right, XMVector3TransformNormal(XMLoadFloat3(&_right), r));
	XMStoreFloat3(&_up, XMVector3TransformNormal(XMLoadFloat3(&_up), r));
	XMStoreFloat3(&_look, XMVector3TransformNormal(XMLoadFloat3(&_look), r));
}

//

Trackingcamera::Trackingcamera(float3 pos, float3 look, float n, float f, float fov,
							   float minD, float maxD, float camTargHeight)
							   : camera(pos, look, n, f, fov), _minDist(minD), _maxDist(maxD), _targetHeight(camTargHeight)
{

}

void Trackingcamera::Update(float3 trackPos)
{
	_position.y = _targetHeight;
	load3as(trackPos, TP);
	load3as(_position, P);
	auto camToObj = TP - P;
	float camdist = XMVectorGetX(XMVector3Length(camToObj));
	float corrf = 0;
	if(camdist < _minDist)
		corrf = .15f * (_minDist-camdist)/camdist;
	if(camdist > _maxDist)
		corrf = .15f * (_maxDist-camdist)/camdist;
	auto cpos = P - corrf*camToObj;
	LookAt(cpos, TP, XMVectorSet(0, 1, 0, 0));
	UpdateView();
	/*_position.y = _targetHeight;
	float3 camToObj = (trackPos) - _position;
	float camDist = length(camToObj);
	float corrf = 0;
	if(camDist > _minDist)
	{
		corrf = .15f * (_minDist-camDist)/camDist;
	}
	if(camDist < _maxDist)
	{
		corrf = .15f * (_maxDist-camDist)/camDist;
	}
	float3 campos = (_position) - (corrf*camToObj);
	LookAt(campos, trackPos, float3(0, 1, 0));
	UpdateView();*/
}