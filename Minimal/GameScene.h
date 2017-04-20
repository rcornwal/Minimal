/////////////////////
// GameScene.h
/////////////////////
#include <iostream>
#include <memory>
#include <exception>
#include <algorithm>
#include <Windows.h>

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define MAX_MOLECULES 10

#define FAIL(X) throw std::runtime_error(X)

// Import the most commonly used types into the default namespace
using glm::ivec3;
using glm::ivec2;
using glm::uvec2;
using glm::mat3;
using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::quat;

#include "Factory.h"
#include "Controller.h"
#include "CO2Molecule.h"

// a class for encapsulating building and rendering an RGB cube
class GameScene {

private:

	Factory factoryModel;
	CO2Molecule moleculeContainer[MAX_MOLECULES];
	CO2Molecule molecule;
	Controller leftController;
	Controller rightController;

	int lastUsedMolecule = 0;
	int tick = 0;

public:

	struct hmdData {
		glm::vec3 hmdPos;
		glm::vec3 leftControllerPos;
		glm::vec3 rightControllerPos;
		glm::vec4 leftControllerOrientation;
		glm::vec4 rightControllerOrientation;
	} hmdData;

	GameScene() {}

	void render(const mat4 & projection, const mat4 & modelview) {

		//factoryModel.Render(modelview, projection);
		
		// Set a new molecule to active every second (oculus should have 90 fps)
		if (tick == 1000) {
			moleculeContainer[lastUsedMolecule].active = true;
			lastUsedMolecule++;
			lastUsedMolecule %= MAX_MOLECULES;
			tick = 0;
		}
		tick++;
		
		// Render all the active molecules
		for (int i = 0; i < MAX_MOLECULES; ++i) {
			if (moleculeContainer[i].active) {
				moleculeContainer[i].Render(modelview, projection);
			}
		}

		

		leftController.position = hmdData.leftControllerPos;
		leftController.rotation = hmdData.leftControllerOrientation;
		leftController.Render(modelview, projection);

		rightController.position = hmdData.rightControllerPos;
		rightController.rotation = hmdData.rightControllerOrientation;
		rightController.Render(modelview, projection);
	}
};