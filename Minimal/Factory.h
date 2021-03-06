/////////////////////
// Factory.h
/////////////////////

#include <Windows.h>
#include "Shader.h"
#include "Model.h"

// Std includes
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

// GLM includes
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <GL/glew.h>

class Factory {
public:
	Factory();
	void Render(glm::mat4 view, glm::mat4 proj);
private:
	/* Data */
	Model factoryModel;
	GLchar* pathToFactory = "./Models/factory1/factory1.obj";
	GLchar* vertexShaderPath = "./Models/factory1/factory.vs";
	GLchar* fragShaderPath = "./Models/factory1/factory.frag";
	Shader factoryShader;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	/* Functions */
	glm::mat4 GetModelMatrix();
};