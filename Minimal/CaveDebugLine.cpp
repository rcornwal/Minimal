/////////////////////
// CaveDebugLine.cpp
/////////////////////

#include "CaveDebugLine.h"

/////////////////////////////////////////////////////

CaveDebugLine::CaveDebugLine() {

	// Create the shader to use for the debug line
	Shader linerS(vertexShaderPath, fragShaderPath);
	lineShader = linerS;

	// Sets the position / rotation / scale
	position = glm::vec3(0, 0, 0);
	color = glm::vec3(1.0f, 0.0f, 0.0f);

	// Set up the buffer arrays
	startPos = glm::vec3(0, 0, 0);
	endPos = glm::vec3(0,0,0);
	prevPos = glm::vec3(0, 0, 0);
	vertices = {
		0,0,0,				
		1.0f, 1.0f, 1.0f		
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

void CaveDebugLine::Draw() {
	glBindVertexArray(this->VAO);
	glLineWidth(10.0f);
	glDrawArrays(GL_LINES,0, 2);
	glBindVertexArray(0);
}

void CaveDebugLine::Render(glm::mat4 view, glm::mat4 proj) {

	lineShader.Use();

	GLint objectColorLoc = glGetUniformLocation(lineShader.Program, "objectColor");
	glUniform3f(objectColorLoc, color.x, color.y, color.z);

	// Calculate the toWorld matrix for the model
	glm::vec3 difPos = endPos - startPos;

	glm::mat4 model;
	glm::mat4 transM = glm::translate(glm::mat4(1.0f), startPos - prevPos);
	glm::mat4 scaleM = glm::scale(glm::mat4(1.0f), difPos);
	model = transM * scaleM;

	glUniformMatrix4fv(glGetUniformLocation(lineShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(lineShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(lineShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

	Draw();
}

void CaveDebugLine::SetStartPos(glm::vec3 pos) {
	startPos = pos;
}

void CaveDebugLine::SetEndPos(glm::vec3 pos) {
	endPos = pos;
}
