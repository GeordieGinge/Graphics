#include "Pyramid.h"

Mesh* Pyramid::pyramid = NULL;

Pyramid::Pyramid(void)
{
	SceneNode* body = new SceneNode(pyramid);
	body->SetModelScale(Vector3(500, 500, 500));
	body->SetTransform(Matrix4::Translation(Vector3(2000, 200, 2600)));
	AddChild(body);
}

void Pyramid::Update(float msec)
{
	SceneNode::Update(msec);
}