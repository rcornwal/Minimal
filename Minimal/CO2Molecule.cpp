/////////////////////
// CO2Molecule.cpp
/////////////////////

#include "CO2Molecule.h"

#define BOUND 1.2f

/////////////////////////////////////////////////////

CO2Molecule::CO2Molecule() {
	this->setup();
}

CO2Molecule::CO2Molecule(Model co2M, Model o2M) {
	// Create the shader to use for the factory
	Shader facS(vertexShaderPath, fragShaderPath);
	co2Shader = facS;


	// Creates the model for the factory
	currentModel = co2M;
	co2Model = co2M;
	o2Model = o2M;

	random_vector = glm::normalize(glm::vec3(rand(), rand(), rand()));

	this->setup();
}

void CO2Molecule::setup() {
	// Sets the position / rotation / scale
	position = glm::vec3(0, 0, 0);
	rotation = 0.0f;
	scale = glm::vec3(1, 1, 1);
	color = glm::vec3(0.0f, 0.5f, 0.31f);
}

void CO2Molecule::ChangeToO2() {
	currentModel = o2Model;
	color = glm::vec3(1.0f, 1.0f, 1.0f);
}

void CO2Molecule::ChangeToCO2() {
	currentModel = co2Model;
	color = glm::vec3(0.0f, 0.5f, 0.31f);
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
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		position = spawn_point;
		init = false;
	}
	
	if (position.x >= BOUND) {
		x_move = -x_move;
	}
	if (position.y >= BOUND) {
		y_move = -y_move;
	}
	if (position.z >= BOUND) {
		z_move = -z_move;
	}
	if (position.x <= -BOUND) {
		x_move = -x_move;
	}
	if (position.y <= -BOUND) {
		y_move = -y_move;
	}
	if (position.z <= -BOUND) {
		z_move = -z_move;
	}

	glm::vec3 move = glm::normalize(glm::vec3(x_move, y_move, z_move)) * 0.003f;

	glm::mat4 translate = glm::translate	(glm::mat4(1.0f), position+move);
	glm::mat4 scale		= glm::scale		(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f));
	glm::mat4 rotate	= glm::rotate		(glm::mat4(1.0f), rotation+0.01f, random_vector);

	model = translate * scale * rotate;


	position = model[3];
	rotation += 0.01f;
	tick++;

	glUniformMatrix4fv(glGetUniformLocation(co2Shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(co2Shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(co2Shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));


	currentModel.Draw(co2Shader);

}
