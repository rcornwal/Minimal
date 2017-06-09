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

#include "Gun.h"


// Properties
const GLuint screenWidth = 800, screenHeight = 600;

class LeapApp {

public:
	int run();
	void ProcessFrame(Leap::Controller controller, Gun * gun, bool * leap_trigger);

private:
	glm::vec4 toQuaternion(float yaw, float pitch, float roll);
	glm::vec3 camera_position;
	glm::vec3 camera_front;
	GLfloat deltaTime;
	GLfloat lastFrame;
};