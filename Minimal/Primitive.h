/////////////////////
// Primitive.h
/////////////////////

#pragma once
#include <Windows.h>
#include "Model.h"
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
#include <OVR_CAPI.h>
#include <OVR_CAPI_GL.h>

class Primitive {
public:

	// Data
	glm::vec3 position;
	glm::vec4 rotation;
	glm::vec3 scale;
	glm::vec3 modelScale;

	// Functions
	virtual void Render(glm::mat4 view, glm::mat4 proj) {};
	virtual void SetRadius(float r) {};
	virtual void SetRed() {}
	virtual void SetBlue() {}
	virtual void SetLength(float l) {}
	virtual void SetHeight(float h) {}

protected:
	Model model;
	Shader shader;

	GLchar* vertexShaderPath;
	GLchar* fragShaderPath;
	GLchar* pathToModel;
};
