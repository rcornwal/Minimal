#pragma once

#include <Windows.h>
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

class Ball : public GameObject{
public:

	glm::vec3 position;
	glm::vec4 rotation;
	glm::vec3 scale;
	Model model;
	Shader shader;

	Ball();
	Ball(Model ballModel, bool);
	void Render(glm::mat4 view, glm::mat4 proj, glm::vec3 centerPos);

protected:

	// Data
	GLchar* vertexShaderPath_Red = "./Models/ball_red/ball_red.vs";
	GLchar* fragShaderPath_Red = "./Models/ball_red/ball_red.frag";

	GLchar* vertexShaderPath_Blue = "./Models/ball_blue/ball_blue.vs";
	GLchar* fragShaderPath_Blue = "./Models/ball_blue/ball_blue.frag";

	glm::vec3 GetPos();
	glm::vec3 GetScale();
	glm::vec4 GetRot();
	Model * GetModel();

};

