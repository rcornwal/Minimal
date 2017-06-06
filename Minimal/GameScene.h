/////////////////////
// GameScene.h
/////////////////////
#pragma once

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

#include "Skybox.h"
#include "Gun.h"
#include "Environment.h"
#include "Controller.h"
#include "Physics.h"
#include "GameObject.h"
#include "Network.h"
#include "rpc\server.h"

using namespace glm;


// a class for encapsulating building and rendering an RGB cube
class GameScene {

private:

	Network network;

	Controller leftController;
	//Controller rightController;

	Gun gun;
	Environment env;
	int gameState = 0;
	GLboolean l_thumb_down = false;

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
		//Network::CreateServer();
		

	}

	void resetGame() {

	}

	void render(const mat4 & projection, const mat4 & modelview) {
		network.CreateServer();
		// Controlls for the left controller
		/*
		leftController.inputState = hmdData.inputState;
		leftController.btn1 = ovrTouch_X;
		leftController.btn2 = ovrTouch_Y;
		leftController.hand = ovrHand_Left;

		leftController.position = hmdData.leftControllerPos;
		leftController.rotation = hmdData.leftControllerOrientation;
		leftController.Render(modelview, projection);
		*/

		// Reset the game
		/*
		if (gameState != 0 && hmdData.inputState.Buttons & ovrTouch_A) {
		resetGame();
		}

		// Get the left stick
		if (hmdData.inputState.Thumbstick[ovrHand_Left].x > 0) {
		texCube.ScaleDown();
		}
		else if (hmdData.inputState.Thumbstick[ovrHand_Left].x < 0) {
		texCube.ScaleUp();
		}

		// Resets scale on left thumb press
		if (hmdData.inputState.Buttons & ovrButton_LThumb && !l_thumb_down) {
		texCube.ResetScale();
		l_thumb_down = true;
		}
		else if (!hmdData.inputState.Buttons && l_thumb_down) {
		l_thumb_down = false;
		}
		*/

		Physics::debug = true;
		Physics::Update(modelview, projection);

		// Controlls for the right controller
		gun.inputState = hmdData.inputState;
		gun.btn1 = ovrTouch_A;
		gun.btn2 = ovrTouch_B;
		gun.hand = ovrHand_Right;

		gun.position = hmdData.rightControllerPos;
		gun.rotation = hmdData.rightControllerOrientation;
		gun.Render(modelview, projection);

		// Render our environment (floor, walls, etc)
		env.Render(modelview, projection);
		GameObject::RenderAll(modelview, projection);
	}
};