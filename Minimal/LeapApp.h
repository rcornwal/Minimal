#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Leap.h>

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "GameScene.h"


// Properties
const GLuint screenWidth = 800, screenHeight = 600;

struct LeapData {
	glm::vec3 leftHandPos;
	glm::vec3 rightHandPos;
	glm::vec4 leftHandOrientation;
	glm::vec4 rightHandOrientation;
	bool triggerSqueezed;
};

class StereoCam {

public:

	StereoCam();



	float EyeSeparation;
	float AspectRatio;
	float FOV;
	float Convergence;
	float NearClip;
	float FarClip;
};

class LeapApp {
public:
	LeapApp();
	int run();
	void ProcessFrame(LeapData * leap_data);

	Leap::Controller controller;
	glm::vec4 toQuaternion(float yaw, float pitch, float roll);
	glm::vec3 camera_position;
	glm::vec3 camera_front;
	GLfloat deltaTime;
	GLfloat lastFrame;
};