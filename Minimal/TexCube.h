/////////////////////
// TexCube.h
/////////////////////

#pragma once
#include <Windows.h>
#include "Shader.h"
#include "Model.h"

// Std includes
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

// GLM includes
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <GL/glew.h>

class TexCube {
public:
	glm::vec3 position;
	glm::vec4 rotation;
	glm::vec3 scale;
	glm::vec3 origScale;

	TexCube();
	void Render(glm::mat4 view, glm::mat4 proj);
	void Draw();
	glm::vec3 color;

	void ScaleUp();
	void ScaleDown();
	void ResetScale();

private:
	/* Data */
	vector<GLfloat> vertices;
	vector<GLuint> indices;

	GLchar* vertexShaderPath = "./Models/texcube/texcube.vs";
	GLchar* fragShaderPath   = "./Models/texcube/texcube.frag";
	GLchar* texturePath = "./Models/texcube/pattern.ppm";
	Shader texCubeShader;
	GLuint VAO, VBO, EBO;
	GLint cubeTexture;

	/* Functions */
	unsigned char* loadPPM(const char* filename, int& width, int& height);
	GLint TextureFromFile(const char* filename);
};