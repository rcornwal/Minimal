/////////////////////
// GameScene.h
/////////////////////
#pragma once

#include <string>
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
#include "rpc/client.h"

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
#include "Player.h"

using namespace glm;

// a class for encapsulating building and rendering an RGB cube
class GameScene {

private:

	Player player1;
	Player player2;

	Controller leftController;
	//Controller rightController;

	Gun gun;
	Environment env;
	int gameState = 0;
	GLboolean l_thumb_down = false;
	Skybox skybox;

public:

	struct inputData {
		glm::vec3 headPos;
		glm::vec4 headOrientation;
		glm::vec3 leftHandPos;
		glm::vec3 rightHandPos;
		glm::vec4 leftHandOrientation;
		glm::vec4 rightHandOrientation;
		bool triggerSqueezed;
	};
	inputData player1Data;
	inputData player2Data;


	GameScene() {

	}

	void resetGame() {

	}

	void render(const mat4 & projection, const mat4 & modelview, int player) {

		Physics::debug = false;
		Physics::Update(modelview, projection);

		// Render our players
		player1.data.headOrientation = player1Data.headOrientation;
		player1.data.headPos = player1Data.headPos;
		player1.data.leftHandOrientation = player1Data.leftHandOrientation;
		player1.data.leftHandPos = player1Data.leftHandPos;
		player1.data.rightHandOrientation = player1Data.rightHandOrientation;
		player1.data.rightHandPos = player1Data.rightHandPos;
		player1.data.triggerSqueezed = player1Data.triggerSqueezed;
		player1.playerNumber = player;
		if (player == 1) {
		}else if (player == 2) {
			player1.position = (glm::vec3(2.0f, 0, 0));
		}

		player2.data.headOrientation = player2Data.headOrientation;
		player2.data.headPos = player2Data.headPos;
		player2.data.leftHandOrientation = player2Data.leftHandOrientation;
		player2.data.leftHandPos = player2Data.leftHandPos;
		player2.data.rightHandOrientation = player2Data.rightHandOrientation;
		player2.data.rightHandPos = player2Data.rightHandPos;
		player2.data.triggerSqueezed = player2Data.triggerSqueezed;
		if (player == 1) {
			player2.position = glm::vec3(2.0f, 0, 0);
			player2.playerNumber = 2;
		}
		else {
			player2.playerNumber = 1;
			player2.position = glm::vec3(-2.0f, 0, 0);
		}

		player1.SetPerspectiveFromPlayer();
		skybox.Render(modelview, projection);

		// Render our environment (floor, walls, etc)
		GameObject::RenderAll(modelview, projection);

	}
};