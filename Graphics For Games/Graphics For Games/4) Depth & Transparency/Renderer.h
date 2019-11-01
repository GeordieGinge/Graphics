#pragma once
#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);
	virtual void UpdateScene(float msec);
	virtual void RenderScene();

	void SwitchToPerspective();
	void SwitchToOrthographic();
	void ToggleObject();
	void ToggleDepth();
	void ToggleAlphaBlend();
	void ToggleBlendMode();
	void MoveObject(float by);


	//inline void SetScale(float s) { scale = s; }
	//inline void SetRotation(float r) { rotation = r; }
	//inline void SetPosition(Vector3 p) { position = p; }

protected:
	GLuint textures[2];
	Mesh * meshes[2];
	Mesh* triangle;
	Vector3 positions[2];
	
	Matrix4 textureMatrix;
	Matrix4 modelMatrix;
	Matrix4 projMatrix;
	Matrix4 viewMatrix;
	
	bool modifyObject;
	bool usingDepth;
	bool usingAlpha;
	int blendMode;

	//float scale;
	//float rotation;
	//Vector3 position;
	//Camera* camera;
};