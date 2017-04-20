/////////////////////
// CO2Molecule.cpp
/////////////////////

#include "CO2Molecule.h"

/////////////////////////////////////////////////////

CO2Molecule::CO2Molecule() {

	// Create the shader to use for the factory
	Shader facS(vertexShaderPath, fragShaderPath);
	co2Shader = facS;


	// Creates the model for the factory
	Model facM(pathToFactory);
	co2Model = facM;

	// Sets the position / rotation / scale
	position = glm::vec3(0, 0, 0);

}

glm::mat4 CO2Molecule::GetModelMatrix() {
	glm::mat4 modelMat = glm::translate(position);
	return modelMat;
}

void CO2Molecule::Render(glm::mat4 view, glm::mat4 proj) {

	co2Shader.Use();

	GLint objectColorLoc = glGetUniformLocation(co2Shader.Program, "objectColor");
	GLint lightColorLoc = glGetUniformLocation(co2Shader.Program, "lightColor");
	GLint lightPosLoc = glGetUniformLocation(co2Shader.Program, "lightPos");
	GLint viewPosLoc = glGetUniformLocation(co2Shader.Program, "viewPos");

	glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLoc, 0.0f, 0.0f, 0.0f);
	glUniform3f(viewPosLoc, 0.0, 0.0, 0.0);
	
	Model rotate_model;

	// Calculate the toWorld matrix for the model
	// Apply the appropriate transformations for animations. 
	if (init) {
		model = glm::translate(model, spawn_point);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		init = false;
	}

	model = glm::translate(model, glm::vec3(0.0f, 0.01f, 0.0f));
	model = glm::rotate(model, 0.01f, glm::vec3(0, 1, 1));
	tick++;

	glUniformMatrix4fv(glGetUniformLocation(co2Shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(co2Shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(co2Shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));


	co2Model.Draw(co2Shader);

}
