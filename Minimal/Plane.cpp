/////////////////////
// Plane.cpp
/////////////////////

#include "Plane.h"

/////////////////////////////////////////////////////

Plane::Plane() {

	// Create the shader to use for the sphere
	Shader sphereShader (vertexShaderPath, fragShaderPath);
	shader = sphereShader;

	// Creates the model for the controller
	Model sphereModel (pathToModel);
	model = sphereModel;

	// Sets the position / rotation / scale
	position = glm::vec3(0.0f, -0.0f, -.5f);
	rotation = glm::vec4(1.0, 0, 0, 0);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	modelScale = glm::vec3(1.0f, 1.0f, 1.0f);
	color = glm::vec3(0.0f, 0.0f, 1.0f);
}

void Plane::SetRed() {
	color = glm::vec3(1.0f, 0.0f, 0.0f);
}

void Plane::SetBlue() {
	color = glm::vec3(0.0f, 0.0f, 1.0f);
}

void Plane::SetLength(float length) {

	// get the bounding vertices
	glm::vec3 max = model.GetMax();
	glm::vec3 min = model.GetMin();

	// ratio of radiuses == ratio of scales
	float curLength = (max.x - min.x);
	float lengthRatio = curLength / length;
	float curScale = scale.x;
	float newScale = curScale / lengthRatio;
	scale = glm::vec3(newScale, scale.y, scale.z);
}

void Plane::SetHeight(float height) {

	// get the bounding vertices
	glm::vec3 max = model.GetMax();
	glm::vec3 min = model.GetMin();

	// ratio of heights == ratio of scales
	float curHeight = (max.z - min.z);
	float heightRatio = curHeight / height;
	float curScale = scale.z;
	float newScale = curScale / heightRatio;
	scale = glm::vec3(scale.x, scale.y, newScale);
}

void Plane::Render(glm::mat4 view, glm::mat4 proj) {

	shader.Use();

	// Calculate the toWorld matrix for the model
	glm::mat4 modelMat;
	glm::mat4 T = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 R = glm::rotate(glm::mat4(1.0f), rotation.w, glm::vec3(rotation.x, rotation.y, rotation.z));
	glm::mat4 S = glm::scale(glm::mat4(1.0f), scale * modelScale);
	modelMat = T * R * S;

	glUniform3f(glGetUniformLocation(shader.Program, "objectColor"), color.x, color.y, color.z);
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

	model.Draw(shader);
}
