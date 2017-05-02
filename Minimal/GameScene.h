/////////////////////
// GameScene.h
/////////////////////
#include <iostream>
#include <memory>
#include <exception>
#include <algorithm>
#include <Windows.h>

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <OVR_CAPI.h>
#include <OVR_CAPI_GL.h>

#define MAX_MOLECULES 200

#define FAIL(X) throw std::runtime_error(X)

// Import the most commonly used types into the default namespace
using glm::ivec3;
using glm::ivec2;
using glm::uvec2;
using glm::mat3;
using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::quat;

#include "TexCube.h"
#include "Controller.h"

using namespace glm;


// a class for encapsulating building and rendering an RGB cube
class GameScene {

private:

	TexCube texCube;

	Controller leftController;
	Controller rightController;


	int gameState = 0;

public:

	struct hmdData {
		glm::vec3 hmdPos;
		glm::vec3 leftControllerPos;
		glm::vec3 rightControllerPos;
		glm::vec4 leftControllerOrientation;
		glm::vec4 rightControllerOrientation;
		ovrInputState inputState;
	} hmdData;

	GameScene() {

	}

	void resetGame() {

	}

	void render(const mat4 & projection, const mat4 & modelview) {

		texCube.Render(modelview, projection);

		// Controlls for the left controller
		leftController.inputState = hmdData.inputState;
		leftController.btn1 = ovrTouch_X;
		leftController.btn2 = ovrTouch_Y;
		leftController.hand = ovrHand_Left;

		leftController.position = hmdData.leftControllerPos;
		leftController.rotation = hmdData.leftControllerOrientation;
		leftController.Render(modelview, projection);

		// Controlls for the right controller
		rightController.inputState = hmdData.inputState;
		rightController.btn1 = ovrTouch_A;
		rightController.btn2 = ovrTouch_B;
		rightController.hand = ovrHand_Right;

		rightController.position = hmdData.rightControllerPos;
		rightController.rotation = hmdData.rightControllerOrientation;
		rightController.Render(modelview, projection);

		// Reset the game
		if (gameState != 0 && hmdData.inputState.Buttons & ovrTouch_A) {
			resetGame();
		}

	}
};