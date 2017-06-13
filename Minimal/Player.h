/////////////////////
// Player.h
/////////////////////

#pragma once
#include <Windows.h>
#include "Gun.h"
#include "Head.h"
#include "Shield.h"
#include "irrklang.h"

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

class Player : GameObject {
public:

	struct playerData {
		glm::vec3 headPos;
		glm::vec4 headOrientation;
		glm::vec3 leftHandPos;
		glm::vec3 rightHandPos;
		glm::vec4 leftHandOrientation;
		glm::vec4 rightHandOrientation;
		bool triggerSqueezed;
	} data;

	Player();
	int playerNumber;
	void Render(glm::mat4 view, glm::mat4 proj, glm::vec3 centerPos);
	void SetPerspectiveFromPlayer();
	glm::vec3 position;


private:
	/* Data */
	Gun gun;
	Head head;
	Shield shield;
	bool mainPlayer;

	Ball * hitEffect;
	Model hitEffectModel;
	GLchar * pathToHitEffect = "./Models/ball_red/ball_red.obj";
};