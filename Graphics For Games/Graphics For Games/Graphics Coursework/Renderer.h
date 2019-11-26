#pragma once

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/HeightMap.h"
#include "../../nclgl/SceneNode.h"
#include "../../nclgl/Frustum.h"
#include "../../nclgl/MD5Node.h"
#include <algorithm>

#define SHADOWSIZE 16384

class Renderer : public OGLRenderer
{
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

protected:
	void Renderer::DrawNode(SceneNode* n);
	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawHeightmap();
	void DrawWater();
	void DrawSkybox();
	void DrawShadowScene();
	void DrawCombinedScene();
    void DrawMesh();

	SceneNode* root;

	Shader* sceneShader;
	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	Shader* bumpShader;
	Shader* shadowShader;

	HeightMap* heightMap;
	Mesh* quad;

	MD5FileData* watcherData;
	MD5Node* watcherNode;

	Light* light;
	Camera* camera;

	GLuint cubeMap;
	GLuint shadowTex;
	GLuint shadowFBO;

	Frustum frameFrustum;

	vector <SceneNode*> transparentNodeList;
	vector <SceneNode*> nodeList;

	float waterRotate;
};
