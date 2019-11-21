/******************************************************************************
Class:Camera
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:FPS-Style camera. Uses the mouse and keyboard from the Window
class to get movement values!

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Window.h"
#include "Matrix4.h"
#include "Vector3.h"
#include <vector>

class Camera	{
public:
	Camera(void) {
		yaw = 0.0f;
		pitch = 0.0f;
		onRail = true;

		camIndex = 0;
		cameraPos.push_back(new Vector3(3655, 500, 6700));

		cameraPos.push_back(new Vector3(1500, 500, 6530));

		cameraPos.push_back(new Vector3(1555, 500, 5460));

		cameraPos.push_back(new Vector3(410, 500, 5190));

		cameraPos.push_back(new Vector3(575, 500, 320));

		cameraPos.push_back(new Vector3(6780, 500, 490));

		cameraPos.push_back(new Vector3(6370, 500, 4080));

		cameraPos.push_back(new Vector3(5770, 500, 4260));

		cameraPos.push_back(new Vector3(5800, 500, 5590));

		cameraPos.push_back(new Vector3(6730, 500, 6690));

		cameraPos.push_back(new Vector3(3655, 500, 6700));
	};

	Camera(float pitch, float yaw, Vector3 position){
		this->pitch		= pitch;
		this->yaw		= yaw;
		this->position	= position;

		camIndex = 0;
		
	}

	~Camera(void){
		cameraPos.clear();
	};

	void UpdateCamera(float msec = 10.0f);

	//Builds a view matrix for the current camera variables, suitable for sending straight
	//to a vertex shader (i.e it's already an 'inverse camera matrix').
	Matrix4 BuildViewMatrix();

	//Gets position in world space
	Vector3 GetPosition() const { return position;}
	//Sets position in world space
	void	SetPosition(Vector3 val) { position = val;}

	//Gets yaw, in degrees
	float	GetYaw()   const { return yaw;}
	//Sets yaw, in degrees
	void	SetYaw(float y) {yaw = y;}

	//Gets pitch, in degrees
	float	GetPitch() const { return pitch;}
	//Sets pitch, in degrees
	void	SetPitch(float p) {pitch = p;}

	void CheckCameraPosition();
	void SetCustomYaw(int index);
	void MoveToCameraPoint();

protected:
	float	yaw;
	float	pitch;
	Vector3 position;
	bool onRail;

	vector<Vector3*> cameraPos;
	int camIndex;
};