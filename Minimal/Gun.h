/////////////////////
// Gun.h
/////////////////////

#pragma once
#include <Windows.h>
#include "Shader.h"
#include "Model.h"
#include "Laser.h"
#include "Ball.h"
#include "Network.h"

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

struct Ray {
	glm::vec3 origin;
	glm::vec3 dir;
	float dist;
};

class Gun {
public:
	glm::vec3 muzzlePos;

	glm::vec3 position;
	glm::vec4 rotation;
	glm::vec3 scale;

	// Controlls for this controller
	ovrInputState inputState;
	ovrTouch btn1, btn2;
	ovrHandType hand;

	Gun();
	void Render(glm::mat4 view, glm::mat4 proj);
	glm::vec3 GetMuzzlePos();
	void ShootBall();
	glm::vec3 GetColor();
	glm::vec3 GetPos();
	glm::vec4 GetRot();
	glm::vec3 GetScale();

	Ray ray;
private:
	/* Data */
	Laser laser;
	Model gunModel;
	GLchar* pathToGun = "./Models/raygun/raygun.obj";
	GLchar* vertexShaderPath = "./Models/raygun/raygun.vs";
	GLchar* fragShaderPath   = "./Models/raygun/raygun.frag";
	Shader gunShader;

	bool triggerPress = false;

};