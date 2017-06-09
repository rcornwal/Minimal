#include "LeapApp.h"
class LeapApp {
	this->camera_position = glm::vec3(0, 0, 3);
	this->camera_front = glm::vec3(0, 0, -1);
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

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

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
	Leap::Controller controller;
	bool leap_trigger = false;

	//Create scene to render 
	Gun gun;

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

		ProcessFrame(controller, &gun, &leap_trigger);

		// Transformation matrices
		glm::mat4 projection = glm::perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(this->camera_position, this->camera_position + this->camera_front, glm::vec3(0.0f, 1.0f, 0.0f));
		gun.Render(view, projection);

		// Swap the buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void LeapApp::ProcessFrame(Leap::Controller controller, Gun * gun, bool * leap_trigger)
{
	// Get the most recent frame and report some basic information
	const Leap::Frame frame = controller.frame();
	Leap::Vector pos = Leap::Vector(gun->position.x, gun->position.y, gun->position.z);
	glm::vec4 new_rotation = gun->rotation;

	Leap::HandList hands = frame.hands();
	for (Leap::HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
		// Get the first hand
		const Leap::Hand hand = *hl;
		if (hand.isRight()) {
			pos = hand.palmPosition();
			pos /= 40.0f;
			pos = pos - Leap::Vector(0, 5.0f, 0);

			float yaw = -hand.direction().yaw() + 135;
			float pitch = hand.direction().pitch();
			float roll = -hand.palmNormal().roll();
			
			new_rotation = toQuaternion(yaw, pitch, roll);
		}


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

		if (distance < 75.0f) *leap_trigger = true;
		else *leap_trigger = false;

		cout << *leap_trigger << endl;

	}

	gun->position = glm::vec3(pos.x, pos.y, pos.z);
	gun->rotation = new_rotation;
}

glm::vec4 LeapApp::toQuaternion(float yaw, float pitch, float roll)
{
	glm::vec4 q;
	float t0 = std::cos(yaw * 0.5f);
	float t1 = std::sin(yaw * 0.5f);
	float t2 = std::cos(roll * 0.5f);
	float t3 = std::sin(roll * 0.5f);
	float t4 = std::cos(pitch * 0.5f);
	float t5 = std::sin(pitch * 0.5f);

	q.x = t0 * t2 * t4 + t1 * t3 * t5;
	q.y = t0 * t3 * t4 - t1 * t2 * t5;
	q.z = t0 * t2 * t5 + t1 * t3 * t4;
	q.w = t1 * t2 * t4 - t0 * t3 * t5;
	return q;
}
