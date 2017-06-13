/////////////////////
// Tree.h
/////////////////////

#pragma once
#include <Windows.h>
#include "Shader.h"
#include "Model.h"
#include "Laser.h"
#include "GameObject.h"

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

class Tree : public GameObject {
public:
	glm::vec3 position;
	glm::vec4 rotation;
	glm::vec3 scale;
	Model model;

	Tree();
	void Render(glm::mat4 view, glm::mat4 proj, glm::vec3 centerPos);

private:
	/* Data */
	GLchar* pathToTree = "./Models/tree/tree.obj";
	GLchar* vertexShaderPath = "./Models/tree/tree.vs";
	GLchar* fragShaderPath   = "./Models/tree/tree.frag";
	Shader treeShader;

	/* Functions */
	glm::vec3 GetPos();
	glm::vec3 GetScale();
	glm::vec4 GetRot();
	Model * GetModel();
};