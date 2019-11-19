#pragma once

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/HeightMap.h"
#include "../../nclgl/SceneNode.h"
#include "../../nclgl/Frustum.h"
#include "../../nclgl/MD5Node.h"
#include <algorithm>

class Renderer : public OGLRenderer
{
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

	void Renderer::DrawNode(SceneNode* n);
	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawHeightmap();
	void DrawWater();
	void DrawSkybox();

	SceneNode* root;

	Shader* sceneShader;
	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;


	HeightMap* heightMap;
	Mesh* quad;

	MD5FileData* hellData;
	MD5Node* hellNode;

	Light* light;
	Camera* camera;

	GLuint cubeMap;

	Frustum frameFrustum;

	vector <SceneNode*> transparentNodeList;
	vector <SceneNode*> nodeList;

	float waterRotate;
};
