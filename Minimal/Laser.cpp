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
}

void renderCylinder(float x1, float y1, float z1, float x2, float y2, float z2, float radius, int subdivisions, GLUquadricObj *quadric)
{
	float vx = x2 - x1;
	float vy = y2 - y1;
	float vz = z2 - z1;

	//handle the degenerate case of z1 == z2 with an approximation
	if (vz == 0)
		vz = .00000001;

	float v = sqrt(vx*vx + vy*vy + vz*vz);
	float ax = 57.2957795*acos(vz / v);
	if (vz < 0.0)
		ax = -ax;
	float rx = -vy*vz;
	float ry = vx*vz;
	glPushMatrix();

	//draw the cylinder body
	glTranslatef(x1, y1, z1);
	glRotatef(ax, rx, ry, 0.0);
	gluQuadricOrientation(quadric, GLU_OUTSIDE);
	gluCylinder(quadric, radius, radius, v, subdivisions, 1);

	//draw the first cap
	gluQuadricOrientation(quadric, GLU_INSIDE);
	gluDisk(quadric, 0.0, radius, subdivisions, 1);
	glTranslatef(0, 0, v);

	//draw the second cap
	gluQuadricOrientation(quadric, GLU_OUTSIDE);
	gluDisk(quadric, 0.0, radius, subdivisions, 1);
	glPopMatrix();
}
void renderCylinder_convenient(float x1, float y1, float z1, float x2, float y2, float z2, float radius, int subdivisions)
{
	//the same quadric can be re-used for drawing many cylinders
	GLUquadricObj *quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	renderCylinder(x1, y1, z1, x2, y2, z2, radius, subdivisions, quadric);
	gluDeleteQuadric(quadric);
}

void Laser::Draw() {
	GLfloat vertices[] = {
		0.5f,  0.5f, 0.0f,  // Top Right
		0.5f, -0.5f, 0.0f,  // Bottom Right
		-0.5f, -0.5f, 0.0f,  // Bottom Left
		-0.5f,  0.5f, 0.0f   // Top Left 
	};
	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3,  // First Triangle
		1, 2, 3   // Second Triangle
	};
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Laser::Render(glm::mat4 view, glm::mat4 proj) {

	OutputDebugString("Rendering laser...\n");

	laserShader.Use();

	GLint objectColorLoc = glGetUniformLocation(laserShader.Program, "objectColor");
	GLint lightColorLoc = glGetUniformLocation(laserShader.Program, "lightColor");
	GLint lightPosLoc = glGetUniformLocation(laserShader.Program, "lightPos");
	GLint viewPosLoc = glGetUniformLocation(laserShader.Program, "viewPos");

	glUniform3f(objectColorLoc, 1.0f, 1.0f,1.0f);
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLoc, 0.0f,0.0f, 0.0f);
	glUniform3f(viewPosLoc,0.0, 0.0, 0.0);

	// Calculate the toWorld matrix for the model
	glm::mat4 model;
	model = glm::translate(model, position);
	//glm::quat orientation = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);
	//glm::mat4 rotationMatrix = glm::toMat4(orientation);
	//model *= rotationMatrix;
	//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));

	glUniformMatrix4fv(glGetUniformLocation(laserShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(laserShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(laserShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

	Draw();
}

