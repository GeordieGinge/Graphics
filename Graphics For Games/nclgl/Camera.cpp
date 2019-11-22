#include "Camera.h"

/*
Polls the camera for keyboard / mouse movement.
Should be done once per frame! Pass it the msec since
last frame (default value is for simplicities sake...)
*/
void Camera::UpdateCamera(float msec)	{

	//Update the mouse by how much
	pitch -= (Window::GetMouse()->GetRelativePosition().y);
	yaw -= (Window::GetMouse()->GetRelativePosition().x);

	//Bounds check the pitch, to be between straight up and straight down ;)
	pitch = min(pitch, 90.0f);
	pitch = max(pitch, -90.0f);


	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_TAB)) {
		if (onRail) { onRail = false; }
		else if (!onRail) { onRail = true; }
	}



	if(onRail)
	{ 
		CheckCameraPosition();
	}

	else if(!onRail)
	{
		if (yaw < 0) {
			yaw += 360.0f;
		}
		if (yaw > 360.0f) {
			yaw -= 360.0f;
		}

		msec *= 5.0f;

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_W)) {
			position += Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * msec;
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {
			position -= Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * msec;
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {
			position += Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * msec;
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {
			position -= Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * msec;
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) {
			position.y += msec;
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT)) {
			position.y -= msec;
		}
	}

	

	


}

/*
Generates a view matrix for the camera's viewpoint. This matrix can be sent
straight to the shader...it's already an 'inverse camera' matrix.
*/
Matrix4 Camera::BuildViewMatrix()	{
	//Why do a complicated matrix inversion, when we can just generate the matrix
	//using the negative values ;). The matrix multiplication order is important!
	return	Matrix4::Rotation(-pitch, Vector3(1,0,0)) * 
			Matrix4::Rotation(-yaw, Vector3(0,1,0)) * 
			Matrix4::Translation(-position);
};

void Camera::CheckCameraPosition()
{
	if (camIndex < cameraPos.size())
	{
		if (position != *cameraPos[camIndex])
		{
			MoveToCameraPoint();
		}
		else if (position == *cameraPos[camIndex])
		{
			camIndex++;
		}
	}
}
void Camera::MoveToCameraPoint()
{
	if (cameraPos[camIndex] != NULL)
	{

		SetCustomYaw(camIndex);
		Vector3 currentPoint = *cameraPos[camIndex];
		if (position.x < cameraPos[camIndex]->x)
		{
			position.x += 5;

		}
		else if (position.x > cameraPos[camIndex]->x)
		{
			position.x -= 5;

		}

		if (position.y > cameraPos[camIndex]->y)
		{
			position.y -= 5;

		}

		else if (position.y < cameraPos[camIndex]->y)
		{
			position.y += 5;

		}

		if (position.z > cameraPos[camIndex]->z)
		{
			position.z -= 5;

		}
		else if (position.z < cameraPos[camIndex]->z)
		{
			position.z += 5;

		}
	}

}

void Camera::SetCustomYaw(int index)
{
	switch (index)
	{
	case 0:
		break;

	case 1:
		break;

	case 2:

		break;
	}
}