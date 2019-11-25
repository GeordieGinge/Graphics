#include "Renderer.h"
#include "Pyramid.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent)
{
	Pyramid::CreateCube();

	waterRotate = 0.0f;
	camera = new Camera();
	heightMap = new HeightMap(TEXTUREDIR "River.raw");
	quad = Mesh::GenerateQuad();

	camera->SetPosition(Vector3(RAW_WIDTH * HEIGHTMAP_X / 2.0f,
		500.0f, RAW_WIDTH * HEIGHTMAP_X));


	light = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X / 1.0f), 1000, 
		(RAW_HEIGHT * HEIGHTMAP_Z)), Vector4(1, 1, 1, 1), (RAW_WIDTH * HEIGHTMAP_X) * 5);


	currentShader = new Shader(SHADERDIR "PerPixelLightVertex.glsl", SHADERDIR "PerPixelLightFragment.glsl");
	reflectShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"reflectFragment.glsl");
	skyboxShader = new Shader(SHADERDIR"skyboxVertex.glsl", SHADERDIR"skyboxFragment.glsl");
	lightShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"PerPixelFragment.glsl");
	bumpShader = new Shader(SHADERDIR "PerPixelVertex.glsl", SHADERDIR "PerPixelFragment.glsl");
	shadowShader = new Shader(SHADERDIR "shadowVert.glsl", SHADERDIR "shadowFrag.glsl");
	sceneShader = new Shader(SHADERDIR "shadowscenevert.glsl", SHADERDIR "shadowscenefrag.glsl");

	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Sand.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	heightMap->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR "NormalMap.PNG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	modelMatrix = Matrix4::Translation(Vector3(100, 0, 100));

	if (!currentShader->LinkProgram() ||!bumpShader->LinkProgram() || !reflectShader->LinkProgram()||
		!heightMap->GetTexture() || !heightMap->GetBumpMap() || !lightShader->LinkProgram() ||
		!skyboxShader->LinkProgram() || !sceneShader->LinkProgram()|| !shadowShader->LinkProgram())
	{
		return;
	}

	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
		GL_COMPARE_R_TO_TEXTURE);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadowFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	quad->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Blood.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR "sahara_ft.tga", TEXTUREDIR "sahara_bk.tga",
		TEXTUREDIR "sahara_up.tga", TEXTUREDIR "sahara_dn.tga",
		TEXTUREDIR "sahara_rt.tga", TEXTUREDIR "sahara_lf.tga",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0);

	watcherData = new MD5FileData(MESHDIR"hellknight.md5mesh");
	watcherNode = new MD5Node(*watcherData);

	watcherData->AddAnim(MESHDIR"walk7.md5anim");
	watcherNode->PlayAnim(MESHDIR"walk7.md5anim");

	SetTextureRepeating(quad->GetTexture(), true);
	SetTextureRepeating(heightMap->GetTexture(), true);
	SetTextureRepeating(heightMap->GetBumpMap(), true);

	projMatrix = Matrix4::Perspective(1.0f, 30000.0f,
		(float)width / (float)height, 45.0f);

	root = new SceneNode();
	root->AddChild(new Pyramid());
	root->AddChild(new Pyramid(Vector3(6480, 400, 5000), Vector3(1000, 1000, 1000)));
	root->AddChild(new Pyramid(Vector3(2000,200,2000),Vector3(100,100,100)));
	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	init = true;
}

Renderer ::~Renderer(void)
{
	glDeleteTextures(1, &shadowTex);
	glDeleteFramebuffers(1, &shadowFBO);
	delete root;
	delete camera;
	delete heightMap;
	delete quad;
	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete light;
	delete watcherData;
	delete watcherNode;
	delete sceneShader;
	delete shadowShader;
	Pyramid::DeleteCube();
	currentShader = 0;
}

void Renderer::UpdateScene(float msec)
{
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	waterRotate += msec / 1000.0f;
	frameFrustum.FromMatrix(projMatrix * viewMatrix);
	root->Update(msec);
	watcherNode->Update(msec);
}

void Renderer::BuildNodeLists(SceneNode* from)
{
	if (frameFrustum.InsideFrustum(*from))
	{
		Vector3 dir = from->GetWorldTransform().GetPositionVector() - camera->GetPosition();
		from->SetCameraDistance(Vector3::Dot(dir, dir));

		if (from->GetColour().w < 1.0f)
		{
			transparentNodeList.push_back(from);
		}
		else
		{
			nodeList.push_back(from);
		}
	}

	for (vector < SceneNode* >::const_iterator i = from->GetChildIteratorStart();
		i != from->GetChildIteratorEnd(); ++i)
	{
		BuildNodeLists((*i));
	}
}

void Renderer::SortNodeLists()
{
	std::sort(transparentNodeList.begin(), transparentNodeList.end(), SceneNode::CompareByCameraDistance);
	std::sort(nodeList.begin(), nodeList.end(), SceneNode::CompareByCameraDistance);
}

void Renderer::DrawNodes()
{
	for (vector < SceneNode* >::const_iterator i = nodeList.begin();
		i != nodeList.end(); ++i)
	{
		DrawNode((*i));
	}

	for (vector < SceneNode* >::const_reverse_iterator i =
		transparentNodeList.rbegin();
		i != transparentNodeList.rend(); ++i)
	{
		DrawNode((*i));
	}
}

void Renderer::DrawMesh()
{
	modelMatrix = Matrix4::Translation(Vector3(3000, 200, 2700));
	Matrix4 tempMatrix = textureMatrix * modelMatrix;
	//	glUseProgram(currentShader->GetProgram());  
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "textureMatrix"), 1, false, *&tempMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*) & (watcherNode->GetWorldTransform() * Matrix4::Scale(watcherNode->GetModelScale())));


	glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)&watcherNode->GetColour());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useTexture"), (int)watcherNode->GetMesh()->GetTexture());
	UpdateShaderMatrices();
	watcherNode->Draw(*this);
}

void Renderer::DrawNode(SceneNode* n)
{

	if (n->GetMesh())
	{
		modelMatrix.ToIdentity();
		Matrix4 tempMatrix = textureMatrix * modelMatrix;
		//	glUseProgram(currentShader->GetProgram());  
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "textureMatrix"), 1, false, *&tempMatrix.values);
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*) & (n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale())));


		glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)&n->GetColour());

		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useTexture"), (int)n->GetMesh()->GetTexture());
		n->Draw(*this);
	}
}

void Renderer::RenderScene()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawSkybox();
	DrawWater();
	DrawShadowScene();
	DrawCombinedScene();
	/*
	for (int y = 1; y < 2; ++y)
	{
		for (int x = 1; x < 2; ++x)
		{
			modelMatrix.ToIdentity();
			SetCurrentShader(lightShader);
			SetShaderLight(*light);
			UpdateShaderMatrices();
			modelMatrix = Matrix4::Translation(Vector3(x * 2000, 200, y * 2000));
			UpdateShaderMatrices();
			watcherNode->Draw(*this);
		}
	}
	*/
	SwapBuffers();
}

void Renderer::DrawShadowScene()
{
	SetCurrentShader(shadowShader);
	BuildNodeLists(root);
	SortNodeLists();
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

	glClear(GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);


	viewMatrix = Matrix4::BuildViewMatrix(
		light->GetPosition(), Vector3(0, 0, 0));
	textureMatrix = biasMatrix * (projMatrix * viewMatrix);

	UpdateShaderMatrices();
	DrawHeightmap();
	DrawNodes();
	DrawMesh();

	glUseProgram(0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	ClearNodeLists();
}

void Renderer::DrawCombinedScene()
{
	SetCurrentShader(sceneShader);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"shadowTex"), 2);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	SetShaderLight(*light);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	viewMatrix = camera->BuildViewMatrix();
	UpdateShaderMatrices();
	DrawHeightmap();
	DrawNodes();
	DrawMesh();
	
	glUseProgram(0);
	ClearNodeLists();
}

void Renderer::ClearNodeLists()
{
	transparentNodeList.clear();
	nodeList.clear();
}

void Renderer::DrawSkybox()
{
	glDepthMask(GL_FALSE);
	SetCurrentShader(skyboxShader);

	UpdateShaderMatrices();
	quad->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
}

void Renderer::DrawHeightmap()
{
	modelMatrix = Matrix4::Translation(Vector3(0, 0, 0));
	Matrix4 tempMatrix = textureMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "textureMatrix"), 1, false, *&tempMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, *&modelMatrix.values);


	UpdateShaderMatrices();
	heightMap->Draw();
}

void Renderer::DrawWater()
{
	SetCurrentShader(reflectShader);
	SetShaderLight(*light);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"cubeTex"), 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	float heightX = (RAW_WIDTH * HEIGHTMAP_X / 2.0f);

	float heightY = 435 * HEIGHTMAP_Y / 3.9f;

	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f);

	modelMatrix =
		Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
		Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) *
		Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));

	UpdateShaderMatrices();

	quad->Draw();

	glUseProgram(0);
}