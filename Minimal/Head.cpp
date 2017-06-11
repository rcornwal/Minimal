/////////////////////
// Head.cpp
/////////////////////

#include "Head.h"

/////////////////////////////////////////////////////

Head::Head() {

	// Create the shader to use for the head
	Shader conS(vertexShaderPath, fragShaderPath);
	headShader = conS;

	// Creates the model for the head
	Model conM (pathToGun);
	headModel = conM;

	// Sets the position / rotation / scale
	position = glm::vec3(0, 0, 0);


}


void Head::Render(glm::mat4 view, glm::mat4 proj, glm::vec3 centerPos) {

	headShader.Use();

	glm::vec3 offsetPos = position - centerPos;

	// Calculate the toWorld matrix for the model
	glm::mat4 model;
	model = glm::translate(model, position);
	glm::quat orientation = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);
	glm::mat4 rotationMatrix = glm::toMat4(orientation);
	model *= rotationMatrix;
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));

	glUniformMatrix4fv(glGetUniformLocation(headShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(headShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(headShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

	headModel.Draw(headShader);

}
