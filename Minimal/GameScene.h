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

#include <OVR_CAPI.h>
#include <OVR_CAPI_GL.h>

#define MAX_MOLECULES 200

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
	CO2Molecule startingMolecules[5];
	
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
		ovrInputState inputState;
	} hmdData;

	GameScene() {
		Model co2M("./Models/co2/co2.obj");
		Model o2M("./Models/o2/o2.obj");
		for (int i = 0; i < MAX_MOLECULES; i++) {
			moleculeContainer[i] = CO2Molecule(co2M, o2M);
		}
		for (int i = 0; i < 5; i++) {
			startingMolecules[i] = CO2Molecule(co2M, o2M);
			startingMolecules[i].active = true;
		}
	}

	void render(const mat4 & projection, const mat4 & modelview) {

		factoryModel.Render(modelview, projection);
		
		// Set a new molecule to active every second (oculus should have 90 fps)

		if (tick == 500) {
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

		for (int i = 0; i < 5; ++i) {
			startingMolecules[i].Render(modelview, projection);
		}

		// Controlls for the left controller
		leftController.inputState = hmdData.inputState;
		leftController.btn1 = ovrTouch_X;
		leftController.btn2 = ovrTouch_Y;
		leftController.hand = ovrHand_Left;

		leftController.position = hmdData.leftControllerPos;
		leftController.rotation = hmdData.leftControllerOrientation;
		leftController.Render(modelview, projection);

		// Controlls for the right controller
		rightController.inputState = hmdData.inputState;
		rightController.btn1 = ovrTouch_A;
		rightController.btn2 = ovrTouch_B;
		rightController.hand = ovrHand_Right;

		rightController.position = hmdData.rightControllerPos;
		rightController.rotation = hmdData.rightControllerOrientation;
		rightController.Render(modelview, projection);
	}
};