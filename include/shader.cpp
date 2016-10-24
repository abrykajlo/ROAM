#include "shader.h"
#include <GL/glew.h>
#include <stdio.h>

void Shader::Cleanup() 
{
	glUseProgram(0);

	glDetachShader(program, fragmentShader);
	glDetachShader(program, vertexShader);

	glDeleteProgram(program);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
}

bool Shader::Init()
{
	program = glCreateProgram();

	if (!LoadFragmentShader(fragmentShaderSrc)) {
		return false;
	}

	if (!LoadVertexShader(vertexShaderSrc)) {
		return false;
	}

	if (!LinkProgram()) {
		return false;
	}

	return true;
}

void Shader::SetFragmentShader(string shader) 
{
	fragmentShaderSrc = shader;
}

void Shader::SetVertexShader(string shader) 
{
	vertexShaderSrc = shader;
}

void Shader::UseProgram() 
{
	glUseProgram(program);
}

void Shader::RegisterAttrib(string attrib)
{
	glBindAttribLocation(program, attribs.size(), attrib.c_str());
	attribs.push_back(attrib);
}

GLint Shader::GetAttribLocation(string attrib)
{
	for (int i = 0; i < attribs.size(); i++) {
		if (attrib == attribs[i]) {
			return i;
		}
	}

	return -1;
}

GLint Shader::GetUniformLocation(string uniform) 
{
	return glGetUniformLocation(program, uniform.c_str());
}

bool Shader::LoadFragmentShader(string frag) 
{
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	const char* src = frag.c_str();
	int length = frag.length();

	glShaderSource(fragmentShader, 1, &src, &length);
	glCompileShader(fragmentShader);

	int wasCompiled = 0;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &wasCompiled);

	if (!wasCompiled) {
		char infoLog[1000];
		int length;
		glGetShaderInfoLog(fragmentShader, 1000, &length, infoLog);
		printf("%s", infoLog);
		return false;
	}

	glAttachShader(program, fragmentShader);

	return true;
}

bool Shader::LoadVertexShader(string vert) 
{
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	
	const char* src = vert.c_str();
	int length = vert.length();

	glShaderSource(vertexShader, 1, &src, &length);
	glCompileShader(vertexShader);

	int wasCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &wasCompiled);

	if (!wasCompiled) {
		char infoLog[1000];
		int length;
		glGetShaderInfoLog(vertexShader, 1000, &length, infoLog);
		printf("%s", infoLog);
		return false;
	}

	glAttachShader(program, vertexShader);

	return true;
}

bool Shader::LinkProgram() 
{
	glLinkProgram(program);

	int isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);

	if (!isLinked) {
		char infoLog[1000];
		int length;
		glGetProgramInfoLog(program, 1000, &length, infoLog);
		printf("%s", infoLog);
		return false;
	}
	return true;
}

bool Shader::IsValid() {
	glValidateProgram(program);

	int isValid = 0;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &isValid);
	return isValid;
}