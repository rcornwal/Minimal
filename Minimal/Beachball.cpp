/////////////////////
// Beachball.cpp
/////////////////////

#include "Beachball.h"

/////////////////////////////////////////////////////

Beachball::Beachball() {

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

void Beachball::Render(glm::mat4 view, glm::mat4 proj, glm::vec3 centerPos) {

	position = collider->pos;
	glm::vec3 offsetPos = position - centerPos;

	shader.Use();

	// Calculate the toWorld matrix for the model
	glm::mat4 modelMat;
	glm::mat4 T = glm::translate(glm::mat4(1.0f), offsetPos);
	glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);
	modelMat = T * S;

	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

	model.Draw(shader);

}

glm::vec3 Beachball::GetPos() {
	return position;
}

glm::vec3 Beachball::GetScale() {
	return position;
}

glm::vec4 Beachball::GetRot() {
	return rotation;
}

Model * Beachball::GetModel() {
	return &model;
}