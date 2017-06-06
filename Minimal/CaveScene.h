
#pragma once
/////////////////////
// CaveScene.h
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

#define FAIL(X) throw std::runtime_error(X)
#define EYE_DISTANCE 0.0f

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

#include "Skybox.h"
#include "TexCube.h"
#include "Controller.h"
#include "CaveWall.h"

using namespace glm;


// a class for encapsulating building and rendering an RGB cube
class CaveScene {

private:

	// sky
	Skybox skyboxLeft = Skybox(true, EYE_DISTANCE);
	Skybox skyboxRight = Skybox(false, EYE_DISTANCE);

	// calobration cube
	TexCube texCube;

	// controllers
	Controller leftController;
	Controller rightController;

	// cave walls
	CaveWall caveWall_left;
	CaveWall caveWall_right;
	CaveWall caveWall_bottom;

	int gameState = 0;
	GLboolean l_thumb_down = false;

public:

	GLuint _fbo;
	glm::vec4 viewport;

	struct hmdData {
		glm::vec3 hmdPos;
		glm::vec3 leftControllerPos;
		glm::vec3 rightControllerPos;
		glm::vec4 leftControllerOrientation;
		glm::vec4 rightControllerOrientation;
		ovrInputState inputState;
	} hmdData;

	CaveScene() {}

	void HandleInput() {
		// Get the left stick
		if (hmdData.inputState.Thumbstick[ovrHand_Left].x > 0) {
			texCube.ScaleDown();
		}
		else if (hmdData.inputState.Thumbstick[ovrHand_Left].x < 0) {
			texCube.ScaleUp();
		}

		// Get the right stick
		float xMov = hmdData.inputState.Thumbstick[ovrHand_Right].x;
		float yMov = hmdData.inputState.Thumbstick[ovrHand_Right].y;

		if (abs(xMov) > abs(yMov)) {
			if (xMov > 0) {
				texCube.MoveLeft();
			}
			else if (xMov < 0){
				texCube.MoveRight();
			}
		}
		else {
			if (yMov > 0) {
				texCube.MoveUp();
			}
			else if (yMov < 0) {
				texCube.MoveDown();
			}
		}

		// Resets scale on left thumb press
		if (hmdData.inputState.Buttons & ovrButton_LThumb && !l_thumb_down) {
			texCube.ResetScale();
			l_thumb_down = true;
		}
		else if (!hmdData.inputState.Buttons && l_thumb_down) {
			l_thumb_down = false;
		}
	}

	// Controlls for the left controller
	void SetUpLeftController() {
		leftController.inputState = hmdData.inputState;
		leftController.btn1 = ovrTouch_X;
		leftController.btn2 = ovrTouch_Y;
		leftController.hand = ovrHand_Left;
		leftController.position = hmdData.leftControllerPos;
		leftController.rotation = hmdData.leftControllerOrientation;
	}

	// Controlls for the right controller
	void SetUpRightController() {
		rightController.inputState = hmdData.inputState;
		rightController.btn1 = ovrTouch_A;
		rightController.btn2 = ovrTouch_B;
		rightController.hand = ovrHand_Right;
		rightController.position = hmdData.rightControllerPos;
		rightController.rotation = hmdData.rightControllerOrientation;
	}

	void render(const mat4 & projection, const mat4 & modelview, const vec4 viewport, const int eyeNum) {

		HandleInput();

		// render to cave wall
		glViewport(0, 0, 1000, 1000);

		// Setup the Cave geometry
		glm::mat4 leftRot2 = glm::rotate(glm::mat4(), 0.785398f, glm::vec3(0, 1, 0));
		caveWall_left.rotation = leftRot2;

		glm::mat4 rightRot2 = glm::rotate(glm::mat4(), -0.785398f, glm::vec3(0, 1, 0));
		caveWall_right.rotation = rightRot2;

		glm::mat4 bottomRot1 = glm::rotate(glm::mat4(), -1.5708f, glm::vec3(1, 0, 0));
		glm::mat4 bottomRot2 = glm::rotate(glm::mat4(), -0.785398f, glm::vec3(0, 1, 0));
		caveWall_bottom.rotation = bottomRot2 * bottomRot1;

		// Render the left wall
		glBindFramebuffer(GL_FRAMEBUFFER, caveWall_left.framebuffer);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		skyboxLeft.Render(modelview, caveWall_left.GenerateProjection(modelview));
		texCube.Render(modelview, caveWall_left.GenerateProjection(modelview));
		
		// Render the right wall
		glBindFramebuffer(GL_FRAMEBUFFER, caveWall_right.framebuffer);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		skyboxLeft.Render(modelview, caveWall_right.GenerateProjection(modelview));
		texCube.Render(modelview, caveWall_right.GenerateProjection(modelview));

		// Render the ground wall
		glBindFramebuffer(GL_FRAMEBUFFER, caveWall_bottom.framebuffer);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		skyboxLeft.Render(modelview, caveWall_bottom.GenerateProjection(modelview));
		texCube.Render(modelview, caveWall_bottom.GenerateProjection(modelview));

		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		glViewport(viewport.x, viewport.y, viewport.z, viewport.w);

		// Render the objects to oculus space

		caveWall_left.Render(modelview, projection, eyeNum);
		caveWall_right.Render(modelview, projection, eyeNum);
		caveWall_bottom.Render(modelview, projection, eyeNum);

		if (hmdData.inputState.Buttons & ovrTouch_A) {
			caveWall_right.RenderDebug(modelview, projection);
			caveWall_left.RenderDebug(modelview, projection);
		}

		//skyboxLeft.Render(modelview, projection);
		//skyboxRight.Render(modelview, projection);

	}

};