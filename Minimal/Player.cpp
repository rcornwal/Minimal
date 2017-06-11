/////////////////////
// Player.cpp
/////////////////////

#include "Player.h"

/////////////////////////////////////////////////////

Player::Player() {
	mainPlayer = false;
}

void Player::Render(glm::mat4 view, glm::mat4 proj, glm::vec3 centerPos) {

	glm::vec3 offsetPos = (mainPlayer) ? position - centerPos : position;

	// Position the head avatar
	head.position = offsetPos + data.headPos;
	head.rotation = data.headOrientation;
	if (mainPlayer == false) {
		head.Render(view, proj, centerPos);
	}

	// Controlls for the right controller
	gun.position = offsetPos + data.rightHandPos;
	gun.rotation = data.rightHandOrientation;
	gun.triggerSqueezed = data.triggerSqueezed;
	gun.Render(view, proj, centerPos);
}

void Player::SetPerspectiveFromPlayer() {
	mainPlayer = true;
	GameObject::centerPos = position;
	Collider::centerPos = position;
}