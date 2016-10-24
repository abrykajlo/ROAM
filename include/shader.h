#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>
#include <GL/glew.h>

using namespace std;

class Shader {
public:
	bool Init();
	void Cleanup();
	void UseProgram();
	void SetVertexShader(string);
	void SetFragmentShader(string);
	void RegisterAttrib(string);
	bool IsValid();
	GLint GetUniformLocation(string);
	GLint GetAttribLocation(string); 
private:
	bool LoadFragmentShader(string);
	bool LoadVertexShader(string);
	bool LinkProgram();

	GLuint program;
	GLuint vertexShader;
	GLuint fragmentShader;

	string fragmentShaderSrc;
	string vertexShaderSrc;

	vector<string> attribs;
};

#endif