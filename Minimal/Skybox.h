/////////////////////
// Skybox.h
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

class Skybox{
public:
	glm::vec3 position;
	glm::vec4 rotation;
	glm::vec3 scale;

	Skybox();
	void Render(glm::mat4 view, glm::mat4 proj);
	void Draw();
	glm::vec3 color;

private:
	/* Data */
	vector<GLfloat> vertices;
	vector<GLuint> indices;

	GLchar* vertexShaderPath = "./Models/skybox/skybox.vs";
	GLchar* fragShaderPath = "./Models/skybox/skybox.frag";
	Shader skyboxShader;
	GLuint VAO, VBO, EBO;

	GLchar* texturePathNX = "./Models/texcube/pattern.ppm";

	GLint cubeTextureNX;
	GLint cubeTextureNY;
	GLint cubeTextureNZ;
	GLint cubeTexturePX;
	GLint cubeTexturePY;
	GLint cubeTexturePZ;

	/* Functions */
	unsigned char* loadPPM(const char* filename, int& width, int& height);
	GLint TextureFromFile(const char* filename);
};