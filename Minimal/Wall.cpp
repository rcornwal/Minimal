/////////////////////
// Wall.cpp
/////////////////////

#include "Wall.h"

/////////////////////////////////////////////////////

Wall::Wall() {

	// Create the shader to use for the controller
	Shader conS(vertexShaderPath, fragShaderPath);
	wallShader = conS;

	// Creates the model for the controller
	Model conM (pathToFloor);
	model = conM;

	// Sets the position / rotation / scale
	position = glm::vec3(0, -1.0f, 0);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	rotation = glm::vec4(1.0, 0, 0, 0);
}

void Wall::Render(glm::mat4 view, glm::mat4 proj) {

	wallShader.Use();

	// Calculate the toWorld matrix for the model
	glm::mat4 modelMat;
	glm::mat4 T = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 R = glm::rotate(glm::mat4(1.0f), rotation.w, glm::vec3(rotation.x, rotation.y, rotation.z));
	glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);
	modelMat = T * R * S;

	glUniformMatrix4fv(glGetUniformLocation(wallShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
	glUniformMatrix4fv(glGetUniformLocation(wallShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(wallShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

	model.Draw(wallShader);
}

glm::vec3 Wall::GetPos() {
	return position;
}

glm::vec3 Wall::GetScale() {
	return scale;
}

glm::vec4 Wall::GetRot() {
	return rotation;
}

Model * Wall::GetModel() {
	return &model;
}