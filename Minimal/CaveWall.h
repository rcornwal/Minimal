/////////////////////
// CaveWall.h
/////////////////////

#pragma once
#include <Windows.h>
#include "CaveDebugLine.h"
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

class CaveWall {
public:
	CaveWall();

	// The debug lines
	CaveDebugLine debug_topRight;
	CaveDebugLine debug_topLeft;
	CaveDebugLine debug_bottomRight;
	CaveDebugLine debug_bottomLeft;

	glm::vec2 screensize;
	glm::vec3 position;
	glm::mat4 rotation;
	glm::vec3 scale;
	glm::vec3 color;

	GLuint framebuffer;

	void Render(glm::mat4 view, glm::mat4 proj, int eyeNum);
	void RenderDebug(glm::mat4 view, glm::mat4 proj);
	void Draw();
	void GenerateFramebuffer(GLuint fb);
	glm::mat4 GenerateProjection(glm::mat4 view);

private:
	/* Data */
	glm::vec4 topRightPos;
	glm::vec4 topLeftPos;
	glm::vec4 bottomRightPos;
	glm::vec4 bottomLeftPos;

	vector<GLfloat> vertices;
	vector<GLuint> indices;
	GLuint textureColorbuffer;
	GLchar* vertexShaderPath = "./Models/cavewall/cavewall.vs";
	GLchar* fragShaderPath   = "./Models/cavewall/cavewall.frag";
	Shader CaveWallShader;
	GLuint VAO, VBO, EBO;

	/* Functions */
	GLuint GenerateAttachmentTexture();
};