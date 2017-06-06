/////////////////////
// Controller.h
/////////////////////

#pragma once
#include <Windows.h>
#include "Shader.h"
#include "Model.h"
#include "Laser.h"

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

struct ControllerRay {
	glm::vec3 origin;
	glm::vec3 dir;
	float dist;
};

class Controller {
public:
	glm::vec3 position;
	glm::vec4 rotation;
	glm::vec3 scale;

	// Controlls for this controller
	ovrInputState inputState;
	ovrTouch btn1, btn2;
	ovrHandType hand;

	Controller();
	void Render(glm::mat4 view, glm::mat4 proj);

	glm::vec3 GetColor();

	ControllerRay ray;
private:
	/* Data */
	Laser laser;
	Model controllerModel;
	GLchar* pathToController = "./Models/wand/wand.obj";
	GLchar* vertexShaderPath = "./Models/wand/wand.vs";
	GLchar* fragShaderPath   = "./Models/wand/wand.frag";
	Shader controllerShader;


	/* Functions */
	glm::mat4 GetModelMatrix();
};