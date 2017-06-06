/////////////////////
// Floor.cpp
/////////////////////

#include "Floor.h"

/////////////////////////////////////////////////////

Floor::Floor() {

	// Create the shader to use for the controller
	Shader conS(vertexShaderPath, fragShaderPath);
	floorShader = conS;

	// Creates the model for the controller
	Model conM (pathToFloor);
	model = conM;

	// Sets the position / rotation / scale
	position = glm::vec3(0, -1.0f, 0);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	rotation = glm::vec4(1.0, 0, 0, 0);
}

void Floor::Render(glm::mat4 view, glm::mat4 proj) {

	floorShader.Use();

	// Calculate the toWorld matrix for the model
	glm::mat4 modelMat;
	glm::mat4 T = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 R = glm::rotate(glm::mat4(1.0f), rotation.w, glm::vec3(rotation.x, rotation.y, rotation.z));
	glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);
	modelMat = T * R * S;

	glUniformMatrix4fv(glGetUniformLocation(floorShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
	glUniformMatrix4fv(glGetUniformLocation(floorShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(floorShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

	model.Draw(floorShader);
}

glm::vec3 Floor::GetPos() {
	return position;
}

glm::vec3 Floor::GetScale() {
	return scale;
}

glm::vec4 Floor::GetRot() {
	return rotation;
}

Model * Floor::GetModel() {
	return &model;
}