#pragma once

#include <Windows.h>
#include "Shader.h"
#include "Primitive.h"

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

class Sphere : public Primitive {
public:

	Sphere();
	void Render(glm::mat4 view, glm::mat4 proj);
	void SetRed();
	void SetBlue();
	void SetRadius(float r);

protected:

	// Data
	GLchar* vertexShaderPath = "./Models/Primitives/sphere/sphere.vs";
	GLchar* fragShaderPath = "./Models/Primitives/sphere/sphere.frag";
	GLchar* pathToModel = "./Models/Primitives/sphere/sphere.obj";
	glm::vec3 color;
};

