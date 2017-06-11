#pragma once

#include "LeapApp.h"

LeapApp::LeapApp() {
	camera_position = glm::vec3(0, 0, 1.5);
	camera_front = glm::vec3(0, 0, -1);
	controller = Leap::Controller();
};

int LeapApp::run()
{
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Fast and Backwards Reaching Inverse Kinematic (FABRIK) Solver - Robb & Jesse - CSE 163", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	// Leap motion stuff
	bool leap_trigger = false;

	//Create scene to render 
	std::shared_ptr<GameScene> caveScene;
	caveScene = std::shared_ptr<GameScene>(new GameScene());

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();

		// Clear the colorbuffer
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		LeapData leap_data;
		ProcessFrame(&leap_data);
		
		glm::quat q = glm::angleAxis(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		
		caveScene->player1Data.headPos = glm::vec3(0, 0, 0);
		caveScene->player1Data.headOrientation = glm::vec4(q.x, q.y, q.z, q.w);
		caveScene->player1Data.leftHandPos = leap_data.leftHandPos / 10.0f;
		caveScene->player1Data.rightHandPos = leap_data.rightHandPos;
		caveScene->player1Data.leftHandOrientation = leap_data.leftHandOrientation;
		caveScene->player1Data.rightHandOrientation = leap_data.rightHandOrientation;
		caveScene->player1Data.triggerSqueezed = leap_data.triggerSqueezed;


		// Transformation matrices
		glm::mat4 projection = glm::perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(this->camera_position, this->camera_position + this->camera_front, glm::vec3(0.0f, 1.0f, 0.0f));
		caveScene->render(projection, view, 1);

		// Swap the buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void LeapApp::ProcessFrame(LeapData * leap_data)
{
	// Get the most recent frame and report some basic information
	const Leap::Frame frame = controller.frame();
	glm::vec3 rpos, lpos;
	glm::vec4 rort, lort;
	bool trigger = false;

	Leap::HandList hands = frame.hands();
	for (Leap::HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
		// Get the first hand
		const Leap::Hand hand = *hl;

		Leap::Vector pos = hand.palmPosition();
		pos /= 100.0f;
		pos = pos - Leap::Vector(0, 1.0f, 0);

		if (hand.isRight()) rpos = glm::vec3(pos.x, pos.y, pos.z);
		else if (hand.isLeft()) lpos = glm::vec3(pos.x, pos.y, pos.z);

		float yaw = hand.direction().yaw();
		float pitch = hand.direction().pitch();
		float roll = hand.palmNormal().roll();
			
		glm::vec4 new_rotation = toQuaternion(-yaw, pitch, roll);
		if (hand.isRight()) {
			rort = new_rotation;
		}
		else if (hand.isLeft()) lort = new_rotation;

		if (hand.isRight()) {
			Leap::Vector thumb_position;
			Leap::Vector index_position;
			const Leap::FingerList fingers = hand.fingers();
			for (Leap::FingerList::const_iterator it = fingers.begin(); it != fingers.end(); ++it) {
				const Leap::Finger finger = *it;
				if (finger.type() == Leap::Finger::TYPE_THUMB) {
					thumb_position = finger.tipPosition();
				}
				else if (finger.type() == Leap::Finger::TYPE_INDEX) {
					index_position = finger.tipPosition();
				}
			}

			float distance = (thumb_position - index_position).magnitude();
			trigger = (distance < 75.0f);
		}
		

	}

	leap_data->rightHandPos= glm::vec3(rpos.x, rpos.y, rpos.z);
	leap_data->rightHandOrientation = rort;
	leap_data->leftHandPos = glm::vec3(lpos.x, lpos.y, lpos.z);
	leap_data->leftHandOrientation = lort;
	leap_data->triggerSqueezed = trigger;
}

glm::vec4 LeapApp::toQuaternion(float yaw, float pitch, float roll)
{
	glm::quat pitchAngle = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
	glm::quat yawAngle = glm::angleAxis(yaw, glm::vec3(0, 1, 0));
	glm::quat rollAngle = glm::angleAxis(roll, glm::vec3(0, 0, 1));
	glm::quat combined = (rollAngle * yawAngle * pitchAngle);

	return glm::vec4(combined.x, combined.y, combined.z, combined.w);
}
