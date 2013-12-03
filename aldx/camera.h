#pragma once
#include "helper.h"

#undef near
#undef far

//camera
// A perspective camera with some movement methods
class camera
{
public:
	//Create a camera at pos looking in the direction look.
	camera(float3 pos, float3 look, float n, float f, float fov);
	
	camera() { }

	//Update the Projection matrix, if the aspect ratio/near/far/fov has been modified
	void update_proj(float aspectRatio);
	//Update the View matrix, if the position/right/up/look has changed. Usually called every frame
	void update_view();

	float4x4 proj() const { return _proj; };
	
	float4x4 view() const { return _view; };

	inline float& near() { return _nearp; }
	inline float& far() { return _farp; }
	inline float& fovAngle() { return _fovAngle; }

	inline float3& position() { return _position; }
	inline float3& right() { return _right; }
	inline float3& up() { return _up; }
	inline float3& look() { return _look; }

	//Point the camera at a particular point in space
	void target(float3 targ);
	//Move the camera to look at target from pos, with the up vector provided (mvector version)
	void look_at(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
	//Move the camera to look at target from pos, with the up vector provided (float3 version)
	void look_at(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up);

	//Move the camera forward by d
	void forward(float d);
	//Move the camera sideways by d
	void strafe(float d);

	//Rotate the camera on the X axis by angle
	void yaw(float angle);
	//Rotate the camera on the Y axis by angle
	void pitch(float angle);
	//Rotate the camera on the Z axis by angle
	void roll(float angle);

	//Rotate the camera around the world Y axis by angle. Useful for a FPS camera
	void rotate_worldY(float angle);
protected:
	float4x4 _proj, _view;
	float _nearp, _farp, _fovAngle;
	
	float3 _position;

	float3 _right;
	float3 _up;
	float3 _look;
};

class tracking_camera : public camera
{
	float _minDist;
	float _maxDist;
	float _targetHeight;
public:
		//Create a camera at pos looking in the direction look.
	tracking_camera(float3 pos, float3 look, float n, float f, float fov, float minD, float maxD, float camTargHeight);
	
	tracking_camera() { }

	void update(float3 trackPos);

	inline float& min_dist() { return _minDist; }
	inline float& max_dist() { return _maxDist; }
	inline float& target_height() { return _targetHeight; }
};