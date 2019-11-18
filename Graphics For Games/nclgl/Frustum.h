#pragma once
#include "..\nclgl\Plane.h"
#include "../../nclgl/Matrix4.h"
#include "..\nclgl\SceneNode.h"
class Matrix4; 

class Frustum
{
public:
	Frustum(void) {};
	~Frustum(void) {};
	void FromMatrix(const Matrix4& mvp);
	bool InsideFrustum(SceneNode& n);
protected:
	Plane planes[6];
};
