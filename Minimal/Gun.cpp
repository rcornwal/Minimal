/////////////////////
// Gun.cpp
/////////////////////

#include "Gun.h"

/////////////////////////////////////////////////////

Gun::Gun() {

	// Create the shader to use for the controller
	Shader conS(vertexShaderPath, fragShaderPath);
	gunShader = conS;

	// Creates the model for the controller
	Model conM (pathToGun);
	gunModel = conM;

	// Sets the position / rotation / scale
	position = glm::vec3(0, 0, 0);
	muzzlePos = glm::vec3(0, 1.3f, 3.3f);

	// Bind the getters to the server
	//rpc::server * srv = Network::GetServer();
	//srv->bind("GetGunPos", &Gun::GetPos);
	glm::vec3 p = GetPos();
}

glm::vec3 Gun::GetColor() {
	return laser.color;
}

glm::vec3 Gun::GetMuzzlePos() {
	// Figure out the muzzle position
	glm::mat4 model;
	model = glm::translate(model, position);
	glm::quat orientation = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);
	glm::mat4 rotationMatrix = glm::toMat4(orientation);
	model *= rotationMatrix;
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	glm::vec4 p(muzzlePos, 1.0f);
	p = model * p;
	glm::vec3 global_muzzlePos(p.x, p.y, p.z);
	return global_muzzlePos;
}

void Gun::ShootBall() {
	
	// Create our ball and shoot it
	float shootForce = 0.04f;
	Ball* b = new Ball();
	b->position = GetMuzzlePos();
	b->scale = glm::vec3(.03f, .03f, .03f);
	b->AddCollider(Collider::SphereType);
	b->SetRadius(.04f);
	b->ApplyForce(glm::vec3(ray.dir.x * shootForce, ray.dir.y * shootForce, ray.dir.z * shootForce));
}

void Gun::Render(glm::mat4 view, glm::mat4 proj) {

	gunShader.Use();

	// Calculate the toWorld matrix for the model
	glm::mat4 model;
	model = glm::translate(model, position);

	glm::quat orientation = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);
	glm::mat4 rotationMatrix = glm::toMat4(orientation);
	model *= rotationMatrix;
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	glUniformMatrix4fv(glGetUniformLocation(gunShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(gunShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(gunShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

	gunModel.Draw(gunShader);

	// Get the button presses
	if (inputState.Buttons & btn1) {
		// on button 1 press


	}else {
		
	}

	// Get the trigger presses
	if (inputState.IndexTrigger[hand] > .5) {
		laser.SetRed();
		if (!triggerPress) {
			triggerPress = true;
			ShootBall();
		}
	}
	else {
		triggerPress = false;
		laser.SetGreen();
	}

	// Render the laser
	laser.position = GetMuzzlePos();
	laser.rotation = rotation;
	laser.Render(view, proj);

	// Calculates the ray equation
	ray.origin = position;
	ray.dir = glm::normalize(orientation * glm::vec3(0, 0, -1));
	ray.dist = 75.0f; //magic number - same as laserDist in Laser.h

}

glm::vec3 Gun::GetPos() {
	return position;
}

glm::vec4 Gun::GetRot() {
	return rotation;
}

glm::vec3 Gun::GetScale() {
	return scale;
}