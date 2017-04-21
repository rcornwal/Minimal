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

using namespace glm;


// a class for encapsulating building and rendering an RGB cube
class GameScene {

private:

	Factory factoryModel;
	
	CO2Molecule moleculeContainer[MAX_MOLECULES];
	
	Controller leftController;
	Controller rightController;

	int lastUsedMolecule = 5;
	int tick = 0;
	int activeMolecules = 5;

	int gameState = 0;

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

	}

	float intersection(glm::vec3 rayOrigin, glm::vec3 rayDir, glm::vec3 moleculePos, float radius) {

		/*glm::vec3 w = rayOrigin - moleculePos;
		float A = glm::dot(rayDir, rayDir);
		float B = 2 * glm::dot(w, rayDir);
		float C = glm::dot(w, w) - (radius*radius);
		float D = B*B - 4.0f*A*C;
		return D >= 0.0f ? (-B - sqrt(D)) / (2.0f*A) : std::numeric_limits<float>::infinity();*/

		glm::vec3 dir = rayDir;
		glm::vec3 eye = rayOrigin;
		glm::vec3 center = moleculePos;
		
 
		float dis = pow(dot(dir, eye - center), 2) - dot(dir, dir) * (dot(eye - center, eye - center) - pow(radius, 2));

		if (dis >= 0) {
			float t1 = glm::dot(-dir, eye - center) + glm::sqrt(dis) / glm::dot(dir, dir);
			float t2 = glm::dot(-dir, eye - center) - glm::sqrt(dis) / glm::dot(dir, dir);

			if (t1 == t2) {
				return t1;
			}

			// If this point is reached, there are two intersection points. Pick the
			// smaller t value, because it means it's the one that's closer to the camera.
			// However, make sure the t value isn't negative
			else if (t1 < 0 && t2 < 0) {
				return std::numeric_limits<float>::infinity();
			}
			else if (t1 < 0) return t2;
			else if (t2 < 0) return t1;

			float closer = t1 < t2 ? t1 : t2;
			return closer;

		}

		return std::numeric_limits<float>::infinity();

	}

	void checkMoleculeIntersection() {

		glm::vec3 red(1, 0, 0);
		glm::vec3 green(0, 0, 1);

		glm::vec3 rayOrigin;
		glm::vec3 rayDir;
		float rayDist;
		glm::vec3 moleculePos;
		float radius = 0.1f;

		float intersectionDist;

		// Cycle through all our molecules
		for (int i = 0; i < MAX_MOLECULES; ++i) {
			if (moleculeContainer[i].active) {

				int numSelecting = 0;

				moleculePos = moleculeContainer[i].position;

				// left controller ray
				rayOrigin = leftController.ray.origin;
				rayDir = leftController.ray.dir;
				rayDist = leftController.ray.dist;
				intersectionDist = intersection(rayOrigin, rayDir, moleculePos, radius);
				if (intersectionDist < rayDist && intersectionDist > 0.0f && leftController.GetColor()==red) {
					numSelecting++;
				}

				// right controller ray
				rayOrigin = rightController.ray.origin;
				rayDir = rightController.ray.dir;
				rayDist = rightController.ray.dist;
				intersectionDist = intersection(rayOrigin, rayDir, moleculePos, radius);
				if (intersection(rayOrigin, rayDir, moleculePos, radius) < rayDist && rightController.GetColor() == red) {
					numSelecting++;
				}
				
				if (numSelecting == 2 && moleculeContainer[i].isCO2 && gameState == 0) {
					moleculeContainer[i].ChangeToO2();
					moleculeContainer[i].isCO2 = false;
					activeMolecules--;
				}
			}
		}
	}

	void resetGame() {
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		for (int i = 0; i < MAX_MOLECULES; i++) {
			moleculeContainer[i].active = false;
			moleculeContainer[i].isCO2 = true;
			moleculeContainer[i].ChangeToCO2();
			moleculeContainer[i].spawn_point = glm::vec3(0.0f, -0.75f, -1.5f);
			moleculeContainer[i].model = glm::mat4(1.0f);
			moleculeContainer[i].position = moleculeContainer[i].spawn_point;
			moleculeContainer[i].rotation = 0;
		}
		activeMolecules = 5;
		lastUsedMolecule = 5;
		tick = 0;
		gameState = 0;
	}

	void render(const mat4 & projection, const mat4 & modelview) {

		factoryModel.Render(modelview, projection);
		
		// Set a new molecule to active every second (oculus should have 90 fps)

		if (tick == 200) {
			moleculeContainer[lastUsedMolecule].active = true;
			activeMolecules++;
			lastUsedMolecule++;
			lastUsedMolecule %= MAX_MOLECULES;
			tick = 0;
		}
		if(gameState == 0) tick++;

		// Check game state for game over or win
		if (activeMolecules > 10 && gameState == 0) {
			// looooose

			for (int i = 0; i < MAX_MOLECULES; i++) {
				moleculeContainer[i].active = true;
				moleculeContainer[i].spawn_point = glm::vec3(rand() % 4 - 2, rand() % 4 - 2, rand() %4 - 2);
				moleculeContainer[i].position = moleculeContainer[i].spawn_point;
			}

			glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
			gameState = 2;
		}
		else if (activeMolecules <= 0 && gameState == 0) {
			// win!
			gameState = 1;
			glClearColor(135.0f / 255.0f, 206.0f / 255.0f, 250.0f / 255.0f, 1.0f);
		}

		// Render initial active molecules
		for (int i = 0; i < 5; ++i) {
			moleculeContainer[i].active = true;
		}
		
		// Render all the active molecules
		for (int i = 0; i < MAX_MOLECULES; ++i) {
			if (moleculeContainer[i].active) {
				moleculeContainer[i].Render(modelview, projection);
			}
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

		// Reset the game
		if (gameState != 0 && hmdData.inputState.Buttons & ovrTouch_A) {
			resetGame();
		}

		checkMoleculeIntersection();
	}
};