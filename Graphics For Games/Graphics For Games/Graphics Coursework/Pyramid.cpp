#include "Pyramid.h"

Mesh* Pyramid::pyramid = NULL;

Pyramid::Pyramid(Vector3 position, Vector3 scale)
{
	this->body = new SceneNode(pyramid);
	this->body->SetTransform(Matrix4::Translation(position) * Matrix4::Rotation(-90, Vector3(1, 0, 0)));
	this->body->SetModelScale(scale);
	this->body->SetBoundingRadius(1000);
	AddChild(body);
	Timer = 0;
}

Pyramid::Pyramid(void)
{
 body = new SceneNode(pyramid);
	body->SetModelScale(Vector3(1000, 1000, 1000));

	body->SetTransform(Matrix4::Translation(Vector3(500, 560, 6440)) *
		                                   (Matrix4::Rotation(-90, Vector3(1, 0, 0))));
	body->SetBoundingRadius(1000);
	AddChild(body);
	Timer = 0;
}

void Pyramid::Update(float msec)
{
	if (Timer < 700)
	{
		body->SetTransform(Matrix4::Translation(Vector3(body->GetTransform().GetPositionVector().x,
			body->GetTransform().GetPositionVector().y + 1, body->GetTransform().GetPositionVector().z)) *
		(Matrix4::Rotation(-90, Vector3(1, 0, 0))));
		Timer += 1;
	}


	SceneNode::Update(msec);
}