#include "Pyramid.h"

Mesh* Pyramid::pyramid = NULL;

Pyramid::Pyramid(void)
{
	SceneNode* body = new SceneNode(pyramid);
	body->SetModelScale(Vector3(500, 500, 500)); 
	body->SetTransform(Matrix4::Rotation(90, Vector3(0,0,0)));
	body->SetTransform(Matrix4::Translation(Vector3(2500, 200, 3000)));
	AddChild(body);

	body->SetBoundingRadius(500);
}

void Pyramid::Update(float msec)
{
	SceneNode::Update(msec);
}