#pragma once

 #include "../nclgl/OGLRenderer.h"
 #include "../8) Index Buffers/HeightMap.h"
 #include "../nclgl/Camera.h"

 #define POST_PASSES 10

 class Renderer : public OGLRenderer {
 public:
	 Renderer(Window & parent);
	 virtual ~Renderer(void);
	
		 virtual void RenderScene();
	 virtual void UpdateScene(float msec);
	
 protected:
	 void PresentScene();
	 void DrawPostProcess();
	 void DrawScene();
	
		22 Shader * sceneShader;
	23 Shader * processShader;
	24
		25 Camera * camera;
	26
		27 Mesh * quad;
	28 HeightMap * heightMap;
	29
		30 GLuint bufferFBO;
	31 GLuint processFBO;
	32 GLuint bufferColourTex[2];
	33 GLuint bufferDepthTex;
	34
};