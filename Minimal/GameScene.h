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

// a class for encapsulating building and rendering an RGB cube
class GameScene {

private:

	Factory factoryModel;
	Controller leftController;
	Controller rightController;

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

		factoryModel.Render(modelview, projection);

		leftController.position = hmdData.leftControllerPos;
		leftController.rotation = hmdData.leftControllerOrientation;
		leftController.Render(modelview, projection);

		rightController.position = hmdData.rightControllerPos;
		rightController.rotation = hmdData.rightControllerOrientation;
		rightController.Render(modelview, projection);
	}
};