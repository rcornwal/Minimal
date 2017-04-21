/////////////////////
// CO2Molecule.cpp
/////////////////////

#include "CO2Molecule.h"

#define BOUND 20.0f

/////////////////////////////////////////////////////

CO2Molecule::CO2Molecule() {

	// Create the shader to use for the factory
	Shader facS(vertexShaderPath, fragShaderPath);
	co2Shader = facS;


	// Creates the model for the factory
	Model co2M(pathToFactory);
	co2Model = co2M;

	Model o2M(pathToO2);
	o2Model = o2M;

	// Sets the position / rotation / scale
	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	color = glm::vec3(0.0f, 0.5f, 0.31f);

}

void CO2Molecule::ChangeToO2() {
	co2Model = o2Model;
	color = glm::vec3(1.0f, 1.0f, 1.0f);
}

void CO2Molecule::Render(glm::mat4 view, glm::mat4 proj) {

	co2Shader.Use();

	GLint objectColorLoc = glGetUniformLocation(co2Shader.Program, "objectColor");
	GLint lightColorLoc = glGetUniformLocation(co2Shader.Program, "lightColor");
	GLint lightPosLoc = glGetUniformLocation(co2Shader.Program, "lightPos");
	GLint viewPosLoc = glGetUniformLocation(co2Shader.Program, "viewPos");

	glUniform3f(objectColorLoc, color.x, color.y, color.z);
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLoc, 0.0f, 0.0f, 0.0f);
	glUniform3f(viewPosLoc, 0.0, 0.0, 0.0);
	
	// Calculate the toWorld matrix for the model
	// Apply the appropriate transformations for animations. 
	if (init) {
		model = glm::translate(model, spawn_point);
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		init = false;
	}
	
	glm::vec3 random_vector = glm::normalize(glm::vec3(rand(), rand(), rand()));

	if (position.x >= BOUND) {
		x_move = -x_move;
	}
	if (position.y >= BOUND + 15.0f) {
		y_move = -y_move;
	}
	if (position.z >= BOUND - 1.5) {
		z_move = -z_move;
	}
	if (position.x <= -BOUND) {
		x_move = -x_move;
	}
	if (position.y <= -BOUND + 15.0f) {
		y_move = -y_move;
	}
	if (position.z <= -BOUND - 1.5) {
		z_move = -z_move;
	}

	model = glm::translate(model, glm::vec3(x_move, y_move, z_move));
	model = glm::rotate(model, 0.01f, random_vector);

	position = position + glm::vec3(x_move, y_move, z_move);
	rotation = rotation + random_vector;
	tick++;

	glUniformMatrix4fv(glGetUniformLocation(co2Shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(co2Shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(co2Shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));


	co2Model.Draw(co2Shader);

}
