/////////////////////
// Player.cpp
/////////////////////

#include "Player.h"

/////////////////////////////////////////////////////
extern irrklang::ISoundEngine * SoundEngine;
rpc::client p("localhost", 8080);

Player::Player() {
	mainPlayer = false;

	//shield.AddCollider(Collider::SphereType);
	//shield.SetRadius(.1f);
	//shield.IsStatic(true);

	head.ManualRender(true);
	head.AddCollider(Collider::SphereType);
	head.SetRadius(.1f);
	head.IsStatic(true);

	playerNumber = -1;
	hasWon = false;
	wasHit = false;

	// hit effect
	Model hitEffectM(pathToHitEffect);
	hitEffectModel = hitEffectM;
	hitEffect = new Ball(hitEffectModel, true);
	hitEffect->ManualRender(true);
}

void Player::Render(glm::mat4 view, glm::mat4 proj, glm::vec3 centerPos) {

	glm::vec3 offsetPos = (mainPlayer) ? position - centerPos : position;

	// Position the head avatar
	head.position = offsetPos + data.headPos;
	head.rotation = data.headOrientation;
	head.collider->pos = head.position + centerPos;
	head.collider->rot = head.rotation;
	head.drawHelmet = (mainPlayer == false);
	head.Render(view, proj, centerPos);
	
	if (head.collider->destroyedBall == true) {
		if (wasHit == false) {
			wasHit = true;
			if (playerNumber == 1)
				p.call("HurtPlayer1");
			else
				p.call("HurtPlayer2");
			if (p.call("CheckWinState").as<int>() == playerNumber) {
				OutputDebugString("Game is over!");
				hasWon = true;
			}
		}

		hitEffect->position = offsetPos + data.headPos;
		hitEffect->scale = glm::vec3(.05f, .05f, .05f);
		hitEffect->Render(view, proj, centerPos);
		SoundEngine->play2D("./audio/hit.wav", GL_FALSE);
	}
	else {
		wasHit = false;
	}

	// Controlls for the right controller
	gun.position = offsetPos + data.rightHandPos;
	gun.rotation = data.rightHandOrientation;
	gun.triggerSqueezed = data.triggerSqueezed;
	gun.shootRedBalls = (playerNumber == 1);
	gun.Render(view, proj, centerPos);

	// Position the shiled
	//shield.position = offsetPos + data.leftHandPos;
	//shield.rotation = data.leftHandOrientation;
	//shield.collider->pos = shield.position + centerPos;
	//shield.collider->rot = shield.rotation;
}

void Player::SetPerspectiveFromPlayer() {
	mainPlayer = true;
	GameObject::centerPos = position;
	Collider::centerPos = position;
}