#pragma once

#include "../../nclgl/SceneNode.h"
#include "../../nclgl/OBJMesh.h"
#include "../../nclgl/OGLRenderer.h"

class Pyramid : public SceneNode
{
public:
	Pyramid(Vector3 position, Vector3 scale);
	Pyramid(void);
	~Pyramid(void) {};
	virtual void Update(float msec);

	static void CreateCube()
	{
		OBJMesh* m = new OBJMesh();
		m->LoadOBJMesh(MESHDIR"pyramid.OBJ");
		m->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Sand.JPG",
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		m->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"NormalMap.png",
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		pyramid = m;
		pyramid->CreateNormals();
		pyramid->CreateTangents();
		pyramid->CreateBufferData();
	}
	static void DeleteCube() { delete pyramid; }

protected:
	static Mesh* pyramid;
	SceneNode* body;
	int Timer;
};