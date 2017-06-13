/////////////////////
// Shield.cpp
/////////////////////

#include "Shield.h"

/////////////////////////////////////////////////////

Shield::Shield() {

	// Create the shader to use for the controller
	Shader conS(vertexShaderPath, fragShaderPath);
	shieldShader = conS;

	// Creates the model for the controller
	Model conM (pathToShield);
	shieldModel = conM;

	// Sets the position / rotation / scale
	position = glm::vec3(0, 0, 0);
	growing = true;
	minScale = .1f;
	maxScale = .13f;
	scaleStep = .0001f;
	curScale = .1f;
}


void Shield::Render(glm::mat4 view, glm::mat4 proj, glm::vec3 centerPos) {

	shieldShader.Use();

	glm::vec3 offsetPos = position - centerPos;

	GLint objectColorLoc = glGetUniformLocation(shieldShader.Program, "objectColor");
	glm::vec3 blue = glm::vec3(.49f, .975f, 1.0f);
	glm::vec3 green = glm::vec3(0.0f, 1.0f, 0);
	glm::vec3 color = (collider->destroyedBall == true) ? green : blue;
	glUniform3f(objectColorLoc, color.x, color.y, color.z);

	// Calculate the toWorld matrix for the model
	glm::mat4 model;
	model = glm::translate(model, position);
	glm::quat orientation = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);
	glm::mat4 rotationMatrix = glm::toMat4(orientation);
	model *= rotationMatrix;
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	curScale = (growing == true) ? curScale + scaleStep : curScale - scaleStep;
	if (curScale >= maxScale)
		growing = false;
	if (curScale <= minScale)
		growing = true;

	model = glm::scale(model, glm::vec3(curScale, curScale, curScale));

	glUniformMatrix4fv(glGetUniformLocation(shieldShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shieldShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shieldShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

	//shieldModel.Draw(shieldShader);
}

glm::vec3 Shield::GetPos() {
	return position;
}

glm::vec4 Shield::GetRot() {
	return rotation;
}

glm::vec3 Shield::GetScale() {
	return scale;
}