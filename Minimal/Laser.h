/////////////////////
// Laser.h
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

class Laser {
public:
	float laserDist = 75.0f;;
	glm::vec3 position;
	glm::vec4 rotation;
	glm::vec3 scale;

	Laser();
	void Render(glm::mat4 view, glm::mat4 proj);
	void Draw();
	void SetGreen();
	void SetRed();
	glm::vec3 color;

private:
	/* Data */
	vector<GLfloat> vertices;
	vector<GLuint> indices;

	GLchar* vertexShaderPath = "./Models/laser/laser.vs";
	GLchar* fragShaderPath   = "./Models/laser/laser.frag";
	Shader laserShader;
	GLuint VAO, VBO, EBO;
};