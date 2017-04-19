/////////////////////
// Shader.h
/////////////////////

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <Windows.h>
using namespace std;

#include <GL/glew.h>

class Shader {
public:
	GLuint Program;
	// Generate our shaders
	Shader() {}

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
		// Get the vertex/fragment source code from filepaths
		string vertexCode;
		string fragmentCode;
		ifstream vShaderFile;
		ifstream fShaderFile;
		// Allows the ifstream objects to throw exceptions
		vShaderFile.exceptions(ifstream::badbit);
		fShaderFile.exceptions(ifstream::badbit);
		try {
			// Open the files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			stringstream vShaderStream, fShaderStream;
			// Fead files into string streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close the files
			vShaderFile.close();
			fShaderFile.close();
			// Convert the streams into strings
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (ifstream::failure e){
			OutputDebugString("\nERROR, SHADER FILE NOT SUCCESFFULY READ\n");
		}
		const GLchar* VShaderCode = vertexCode.c_str();
		const GLchar* FShaderCode = fragmentCode.c_str();
		// Compile the shaders
		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];
		// Vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &VShaderCode, NULL);
		glCompileShader(vertex);
		// Print any errors that occured compiling vertex shader
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			OutputDebugString("\nERROR - VERTEX SHADER COMPILATION FAILED\n");
		}
		// Fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &FShaderCode, NULL);
		glCompileShader(fragment);
		// Print any errors that occured compiling vertex shader
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			OutputDebugString("\nERROR - FRAG SHADER COMPILATION FAILED\n");
		}
		// The Shader Program
		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertex);
		glAttachShader(this->Program, fragment);
		glLinkProgram(this->Program);
		// Print any linking errors
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
			OutputDebugString("\nERROR - SHADER PROGRAM LINKING FAILED\n");
			OutputDebugString(infoLog);
		}
		// The shaders are linked to the program and are no longer needed
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	// Use this shader
	void Use() {
		glUseProgram(this->Program);
	}
private:

};

#endif