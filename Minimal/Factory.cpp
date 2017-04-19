/////////////////////
// Factory.cpp
/////////////////////

#include "Factory.h"

/////////////////////////////////////////////////////

Factory::Factory() {

	// Create the shader to use for the factory
	Shader facS(vertexShaderPath, fragShaderPath);
	factoryShader = facS;

	
	// Creates the model for the factory
	Model facM(pathToFactory);
	factoryModel = facM;

	// Sets the position / rotation / scale
	position = glm::vec3(0, 0, 0);
	
}

glm::mat4 Factory::GetModelMatrix() {
	glm::mat4 modelMat = glm::translate(position);
	return modelMat;
}

void Factory::Render(glm::mat4 view, glm::mat4 proj) {
	factoryShader.Use();

	glUniformMatrix4fv(glGetUniformLocation(factoryShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(factoryShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

	// Draw the loaded model
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, -0.75f, -1.5f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	
	glUniformMatrix4fv(glGetUniformLocation(factoryShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	factoryModel.Draw(factoryShader);

}

