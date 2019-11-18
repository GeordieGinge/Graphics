#pragma once
#include "../../nclgl/SceneNode.h"
#include "../../nclgl/OBJMesh.h"

class Pyramid : public SceneNode
{
public:
	Pyramid(void);
	~Pyramid(void) {};
	virtual void Update(float msec);


	static void CreateCube()
	{
		OBJMesh* m = new OBJMesh();
		m->LoadOBJMesh(MESHDIR"pyramid.obj");
		m->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Sand2.jpg",
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		pyramid = m;
	}
	static void DeleteCube() { delete pyramid; }

protected:
	static Mesh* pyramid;
	SceneNode* body;
};