/////////////////////
// Head.h
/////////////////////

#pragma once
#include <Windows.h>
#include "Shader.h"
#include "Model.h"
#include "Laser.h"
#include "Ball.h"

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


class Head {
public:

	Head();

	// Positioning
	glm::vec3 position;
	glm::vec4 rotation;
	glm::vec3 scale;

	void Render(glm::mat4 view, glm::mat4 proj, glm::vec3 centerPos);

private:
	/* Data */
	Model headModel;
	GLchar* pathToGun = "./Models/helmet/helmet.obj";
	GLchar* vertexShaderPath = "./Models/helmet/helmet.vs";
	GLchar* fragShaderPath   = "./Models/helmet/helmet.frag";
	Shader headShader;

	bool triggerPress = false;
};