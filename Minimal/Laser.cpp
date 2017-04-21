/////////////////////
// Laser.cpp
/////////////////////

#include "Laser.h"

/////////////////////////////////////////////////////

Laser::Laser() {

	// Create the shader to use for the controller
	Shader laserS(vertexShaderPath, fragShaderPath);
	laserShader = laserS;

	// Sets the position / rotation / scale
	position = glm::vec3(0, 0, 0);

	// Set up the buffer arrays
	vertices = {
		0.0f,  0.0f, 0.0f,  // Top Right
		0.0f, 65.0f, 0.0f  // Bottom Right
	};

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &this->vertices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glBindVertexArray(0);
}

void Laser::SetGreen() {
	glm::vec3 green(0.0f, 1.0f, 0.0f);
	color = green;
}

void Laser::SetRed() {
	glm::vec3 red(1.0f, 0.0f, 0.0f);
	color = red;
}

void Laser::Draw() {
	glBindVertexArray(this->VAO);
	glLineWidth(3.0f);
	glDrawArrays(GL_LINES,0, 2);
	glBindVertexArray(0);
}

void Laser::Render(glm::mat4 view, glm::mat4 proj) {

	OutputDebugString("Rendering laser...\n");

	laserShader.Use();

	GLint objectColorLoc = glGetUniformLocation(laserShader.Program, "objectColor");
	glUniform3f(objectColorLoc, color.x, color.y, color.z);

	// Calculate the toWorld matrix for the model
	glm::mat4 model;
	model = glm::translate(model, position);
	glm::quat orientation = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);
	glm::mat4 rotationMatrix = glm::toMat4(orientation);
	model *= rotationMatrix;
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));

	glUniformMatrix4fv(glGetUniformLocation(laserShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(laserShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(laserShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

	Draw();
}

