/////////////////////
// Controller.cpp
/////////////////////

#include "Controller.h"

/////////////////////////////////////////////////////

Controller::Controller() {

	// Create the shader to use for the controller
	Shader conS(vertexShaderPath, fragShaderPath);
	controllerShader = conS;

	// Creates the model for the controller
	Model conM (pathToController);
	controllerModel = conM;

	// Sets the position / rotation / scale
	position = glm::vec3(0, 0, 0);
}

glm::mat4 Controller::GetModelMatrix() {
	glm::mat4 modelMat = glm::translate(position);
	return modelMat;
}

void Controller::Render(glm::mat4 view, glm::mat4 proj) {

	controllerShader.Use();

	GLint objectColorLoc = glGetUniformLocation(controllerShader.Program, "objectColor");
	GLint lightColorLoc = glGetUniformLocation(controllerShader.Program, "lightColor");
	GLint lightPosLoc = glGetUniformLocation(controllerShader.Program, "lightPos");
	GLint viewPosLoc = glGetUniformLocation(controllerShader.Program, "viewPos");

	glUniform3f(objectColorLoc, 1.0f, 1.0f,1.0f);
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLoc, 0.0f ,0.0f, 0.0f);
	glUniform3f(viewPosLoc,0.0, 0.0, 0.0);

	// Calculate the toWorld matrix for the model
	glm::mat4 model;
	model = glm::translate(model, position);
	glm::quat orientation = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);
	glm::mat4 rotationMatrix = glm::toMat4(orientation);
	model *= rotationMatrix;

	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	glUniformMatrix4fv(glGetUniformLocation(controllerShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(controllerShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(controllerShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

	controllerModel.Draw(controllerShader);

	// Get the button presses
	if (inputState.Buttons & btn1) {
		// on button 1 press
	}else {
		
	}

	// Get the trigger presses
	if (inputState.IndexTrigger[hand] > .5) {
		laser.SetRed();
	}
	else {
		laser.SetGreen();
	}

	// Render the laser
	laser.position = position;
	laser.rotation = rotation;
	laser.Render(view, proj);
}

