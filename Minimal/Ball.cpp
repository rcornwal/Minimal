/////////////////////
// Ball.cpp
/////////////////////

#include "Ball.h"

/////////////////////////////////////////////////////

Ball::Ball() {

	// Create the shader to use for the sphere
	Shader sphereShader (vertexShaderPath, fragShaderPath);
	shader = sphereShader;

	// Creates the model for the controller
	Model sphereModel (pathToModel);
	model = sphereModel;

	// Sets the position / rotation / scale
	position = glm::vec3(0, -1.0f, 0);
	rotation = glm::vec4(1.0, 0, 0, 0);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Ball::Render(glm::mat4 view, glm::mat4 proj) {

	position = collider->pos;

	shader.Use();

	// Calculate the toWorld matrix for the model
	glm::mat4 modelMat;
	glm::mat4 T = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);
	modelMat = T * S;

	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

	model.Draw(shader);

}

glm::vec3 Ball::GetPos() {
	return position;
}

glm::vec3 Ball::GetScale() {
	return position;
}

glm::vec4 Ball::GetRot() {
	return rotation;
}

Model * Ball::GetModel() {
	return &model;
}