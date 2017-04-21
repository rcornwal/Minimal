#pragma once
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

class CO2Molecule {
public:
	CO2Molecule();
	void Render(glm::mat4 view, glm::mat4 proj);
	bool active = false; // makes sure the molecule is active before spawning
	void ChangeToO2();
private:
	/* Data */
	Model co2Model;
	Model o2Model;

	GLchar* pathToO2 = "./Models/o2/o2.obj";
	GLchar* pathToFactory = "./Models/co2/co2.obj";
	GLchar* vertexShaderPath = "./Models/co2/co2.vs";
	GLchar* fragShaderPath = "./Models/co2/co2.frag";
	Shader co2Shader;

	float y_move = 0.05f;
	float x_move = 0.05f;
	float z_move = 0.05f;

	bool init = true;
	int tick = 0;

	glm::vec3 spawn_point = glm::vec3(0.0f, -0.75f, -1.5f);
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 model;

	glm::vec3 color;

	/* Functions */
	glm::mat4 GetModelMatrix();
};