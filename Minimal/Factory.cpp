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
}

//glm::mat4 GetModelMatrix() {

//}

void Factory::Render(glm::mat4 view, glm::mat4 proj) {
	//factoryShader.Use();
	
	//glm::mat4 matWVP = proj * view * GetModelMatrix();
	//glUniformMatrix4fv(glGetUniformLocation(factoryShader.Program, "matWVP"), 1, GL_FALSE, glm::value_ptr(matWVP));
	//test
}

