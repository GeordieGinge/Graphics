#include "Shader.h"
#include <iostream>
Shader::Shader(string vFile, string fFile, string gFile) {
	program = glCreateProgram();
	objects[SHADER_VERTEX] = GenerateShader(vFile, GL_VERTEX_SHADER);
	objects[SHADER_FRAGMENT] = GenerateShader(fFile, GL_FRAGMENT_SHADER);
	objects[SHADER_GEOMETRY] = 0;
		
	if (!gFile.empty()) 
	{
		objects[SHADER_GEOMETRY] = GenerateShader(gFile,
		GL_GEOMETRY_SHADER);
		glAttachShader(program, objects[SHADER_GEOMETRY]);	
	}
	glAttachShader(program, objects[SHADER_VERTEX]);
	glAttachShader(program, objects[SHADER_FRAGMENT]);
	SetDefaultAttributes();
}
 Shader ::~Shader(void) {
	 for (int i = 0; i < 3; ++i) {
		 glDetachShader(program, objects[i]);
		 glDeleteShader(objects[i]);
	}
	 glDeleteProgram(program);
}
 GLuint Shader::GenerateShader(string from, GLenum type) {
	  cout << " Compiling Shader ... " << endl;	 
		 string load;
	  if (!LoadShaderFile(from, load)) {
		  cout << " Compiling failed !" << endl;
		  loadFailed = true;
		  return 0;
		 
	 }
	 
		  GLuint shader = glCreateShader(type);
	 
		  const char* chars = load.c_str();
	  glShaderSource(shader, 1, &chars, NULL);
	  glCompileShader(shader);
	 
		  GLint status;
	  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	 
		  if (status == GL_FALSE) {
		  cout << " Compiling failed !" << endl;
		  char error[512];
		  glGetInfoLogARB(shader, sizeof(error), NULL, error);
		  cout << error;
		  loadFailed = true;
		  return 0;
		 
	 }
	  cout << " Compiling success !" << endl << endl;
	  loadFailed = false;
	  return shader;
	 
 }
