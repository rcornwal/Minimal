/////////////////////
// Gun.h
/////////////////////

#pragma once
#include <Windows.h>
#include "Shader.h"
#include "Model.h"
#include "Laser.h"
#include "Ball.h"
#include "irrKlang.h"

// Std includes
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <deque>

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

#define MAX_BALLS 20

struct Ray {
	glm::vec3 origin;
	glm::vec3 dir;
	float dist;
};

class Gun {
public:

	Gun();

	deque<Ball*> balls;

	// Positioning
	glm::vec3 muzzlePos;
	glm::vec3 position;
	glm::vec4 rotation;
	glm::vec3 scale;

	// Controlls for the gun
	bool triggerSqueezed;

	void Render(glm::mat4 view, glm::mat4 proj, glm::vec3 centerPos);
	glm::vec3 GetMuzzlePos();
	void ShootBall();
	glm::vec3 GetColor();
	glm::vec3 GetPos();
	glm::vec4 GetRot();
	glm::vec3 GetScale();

	Ray ray;
	bool shootRedBalls = false;
private:
	/* Data */
	Laser laser;
	Model gunModel;
	Model redBallModel;
	Model blueBallModel;

	GLchar * pathToBulletModel_Red = "./Models/ball_red/ball_red.obj";
	GLchar * pathToBulletModel_Blue = "./Models/ball_blue/ball_blue.obj";

	GLchar* pathToGun = "./Models/raygun/raygun.obj";
	GLchar* vertexShaderPath = "./Models/raygun/raygun.vs";
	GLchar* fragShaderPath   = "./Models/raygun/raygun.frag";
	Shader gunShader;

	bool triggerPress = false;
};