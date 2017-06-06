/////////////////////
// Laser.h
/////////////////////

#pragma once
#include <Windows.h>
#include "Shader.h"

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

class CaveDebugLine {
public:
	glm::vec3 position;
	glm::vec4 rotation;
	glm::vec3 scale;
	glm::vec3 color;

	CaveDebugLine();

	void Render(glm::mat4 view, glm::mat4 proj);
	void Draw();
	void SetStartPos(glm::vec3 pos);
	void SetEndPos(glm::vec3 pos);

private:
	/* Data */
	vector<GLfloat> vertices;
	vector<GLuint> indices;

	glm::vec3 prevPos;
	glm::vec3 startPos;
	glm::vec3 endPos;

	GLchar* vertexShaderPath = "./Models/laser/laser.vs";
	GLchar* fragShaderPath   = "./Models/laser/laser.frag";
	Shader lineShader;
	GLuint VAO, VBO, EBO;
};