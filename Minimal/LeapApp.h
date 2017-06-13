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
#include "AnaglyphCam.h"
#include "rpc/client.h"

struct LeapData {
	glm::vec3 leftHandPos;
	glm::vec3 rightHandPos;
	glm::vec4 leftHandOrientation;
	glm::vec4 rightHandOrientation;
	bool triggerSqueezed;
};

class LeapApp {
public:
	LeapApp(int player);
	int run();
	void ProcessFrame(LeapData * leap_data);
	static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);
	void SaveServerInformation();
	void GetServerInformation();

	Leap::Controller controller;
	glm::vec4 toQuaternion(float yaw, float pitch, float roll);
	glm::vec3 camera_position;
	glm::vec3 camera_front;
	GLfloat deltaTime;
	GLfloat lastFrame;

	bool soundplayed = false;

	int player;
	//Create scene to render 
	std::shared_ptr<GameScene> caveScene;

	int screenWidth;
	int screenHeight;
};