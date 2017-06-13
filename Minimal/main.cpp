/************************************************************************************

Authors     :   Bradley Austin Davis <bdavis@saintandreas.org>
Copyright   :   Copyright Brad Davis. All Rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

************************************************************************************/


#include <iostream>
#include <memory>
#include <string>
#include <exception>
#include <algorithm>
#include <functional>
#include <Windows.h>

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "GameScene.h"
#include "LeapApp.h"

#define __STDC_FORMAT_MACROS 1

#define FAIL(X) throw std::runtime_error(X)

///////////////////////////////////////////////////////////////////////////////
//
// GLM is a C++ math library meant to mirror the syntax of GLSL 
//

#include "rpc/client.h"

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

///////////////////////////////////////////////////////////////////////////////
//
// GLEW gives cross platform access to OpenGL 3.x+ functionality.  
//

#include <GL/glew.h>

bool checkFramebufferStatus(GLenum target = GL_FRAMEBUFFER) {
	GLuint status = glCheckFramebufferStatus(target);
	switch (status) {
	case GL_FRAMEBUFFER_COMPLETE:
		return true;
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		std::cerr << "framebuffer incomplete attachment" << std::endl;
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		std::cerr << "framebuffer missing attachment" << std::endl;
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		std::cerr << "framebuffer incomplete draw buffer" << std::endl;
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		std::cerr << "framebuffer incomplete read buffer" << std::endl;
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		std::cerr << "framebuffer incomplete multisample" << std::endl;
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		std::cerr << "framebuffer incomplete layer targets" << std::endl;
		break;

	case GL_FRAMEBUFFER_UNSUPPORTED:
		std::cerr << "framebuffer unsupported internal format or image" << std::endl;
		break;

	default:
		std::cerr << "other framebuffer error" << std::endl;
		break;
	}

	return false;
}

bool checkGlError() {
	GLenum error = glGetError();
	if (!error) {
		return false;
	}
	else {
		switch (error) {
		case GL_INVALID_ENUM:
			std::cerr << ": An unacceptable value is specified for an enumerated argument.The offending command is ignored and has no other side effect than to set the error flag.";
			break;
		case GL_INVALID_VALUE:
			std::cerr << ": A numeric argument is out of range.The offending command is ignored and has no other side effect than to set the error flag";
			break;
		case GL_INVALID_OPERATION:
			std::cerr << ": The specified operation is not allowed in the current state.The offending command is ignored and has no other side effect than to set the error flag..";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cerr << ": The framebuffer object is not complete.The offending command is ignored and has no other side effect than to set the error flag.";
			break;
		case GL_OUT_OF_MEMORY:
			std::cerr << ": There is not enough memory left to execute the command.The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
			break;
		case GL_STACK_UNDERFLOW:
			std::cerr << ": An attempt has been made to perform an operation that would cause an internal stack to underflow.";
			break;
		case GL_STACK_OVERFLOW:
			std::cerr << ": An attempt has been made to perform an operation that would cause an internal stack to overflow.";
			break;
		}
		return true;
	}
}

void glDebugCallbackHandler(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg, GLvoid* data) {
	OutputDebugStringA(msg);
	std::cout << "debug call: " << msg << std::endl;
}

//////////////////////////////////////////////////////////////////////
//
// GLFW provides cross platform window creation
//

#include <GLFW/glfw3.h>

namespace glfw {
	inline GLFWwindow * createWindow(const uvec2 & size, const ivec2 & position = ivec2(INT_MIN)) {
		GLFWwindow * window = glfwCreateWindow(size.x, size.y, "glfw", nullptr, nullptr);
		if (!window) {
			FAIL("Unable to create rendering window");
		}
		if ((position.x > INT_MIN) && (position.y > INT_MIN)) {
			glfwSetWindowPos(window, position.x, position.y);
		}
		return window;
	}
}

// A class to encapsulate using GLFW to handle input and render a scene
class GlfwApp {

protected:
	uvec2 windowSize;
	ivec2 windowPosition;
	GLFWwindow * window{ nullptr };
	unsigned int frame{ 0 };

public:
	GlfwApp() {
		// Initialize the GLFW system for creating and positioning windows
		if (!glfwInit()) {
			FAIL("Failed to initialize GLFW");
		}
		glfwSetErrorCallback(ErrorCallback);
	}

	virtual ~GlfwApp() {
		if (nullptr != window) {
			glfwDestroyWindow(window);
		}
		glfwTerminate();
	}

	virtual int run() {
		preCreate();

		window = createRenderingTarget(windowSize, windowPosition);

		if (!window) {
			std::cout << "Unable to create OpenGL window" << std::endl;
			return -1;
		}

		postCreate();

		initGl();

		while (!glfwWindowShouldClose(window)) {
			++frame;
			glfwPollEvents();
			update();
			draw();
			finishFrame();
		}

		shutdownGl();

		return 0;
	}


protected:
	virtual GLFWwindow * createRenderingTarget(uvec2 & size, ivec2 & pos) = 0;

	virtual void draw() = 0;

	void preCreate() {
		glfwWindowHint(GLFW_DEPTH_BITS, 16);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	}


	void postCreate() {
		glfwSetWindowUserPointer(window, this);
		glfwSetKeyCallback(window, KeyCallback);
		glfwSetMouseButtonCallback(window, MouseButtonCallback);
		glfwMakeContextCurrent(window);

		// Initialize the OpenGL bindings
		// For some reason we have to set this experminetal flag to properly
		// init GLEW if we use a core context.
		glewExperimental = GL_TRUE;
		if (0 != glewInit()) {
			FAIL("Failed to initialize GLEW");
		}
		glGetError();

		if (GLEW_KHR_debug) {
			GLint v;
			glGetIntegerv(GL_CONTEXT_FLAGS, &v);
			if (v & GL_CONTEXT_FLAG_DEBUG_BIT) {
				//glDebugMessageCallback(glDebugCallbackHandler, this);
			}
		}
	}

	virtual void initGl() {
	}

	virtual void shutdownGl() {
	}

	virtual void finishFrame() {
		glfwSwapBuffers(window);
	}

	virtual void destroyWindow() {
		glfwSetKeyCallback(window, nullptr);
		glfwSetMouseButtonCallback(window, nullptr);
		glfwDestroyWindow(window);
	}

	virtual void onKey(int key, int scancode, int action, int mods) {
		if (GLFW_PRESS != action) {
			return;
		}

		switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, 1);
			return;
		}
	}

	virtual void update() {}

	virtual void onMouseButton(int button, int action, int mods) {}

protected:
	virtual void viewport(const ivec2 & pos, const uvec2 & size) {
		glViewport(pos.x, pos.y, size.x, size.y);
	}

private:

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		GlfwApp * instance = (GlfwApp *)glfwGetWindowUserPointer(window);
		instance->onKey(key, scancode, action, mods);
	}

	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		GlfwApp * instance = (GlfwApp *)glfwGetWindowUserPointer(window);
		instance->onMouseButton(button, action, mods);
	}

	static void ErrorCallback(int error, const char* description) {
		FAIL(description);
	}
};

//////////////////////////////////////////////////////////////////////
//
// The Oculus VR C API provides access to information about the HMD
//

#include <OVR_CAPI.h>
#include <OVR_CAPI_GL.h>

namespace ovr {

	// Convenience method for looping over each eye with a lambda
	template <typename Function>
	inline void for_each_eye(Function function) {
		for (ovrEyeType eye = ovrEyeType::ovrEye_Left;
			eye < ovrEyeType::ovrEye_Count;
			eye = static_cast<ovrEyeType>(eye + 1)) {
			function(eye);
		}
	}

	template <typename Function>
	inline void for_left_eye(Function function) {
		function(ovrEyeType::ovrEye_Left);
	}

	template <typename Function>
	inline void for_right_eye(Function function) {
		function(ovrEyeType::ovrEye_Right);
	}

	inline mat4 toGlm(const ovrMatrix4f & om) {
		return glm::transpose(glm::make_mat4(&om.M[0][0]));
	}

	inline mat4 toGlm(const ovrFovPort & fovport, float nearPlane = 0.01f, float farPlane = 10000.0f) {
		return toGlm(ovrMatrix4f_Projection(fovport, nearPlane, farPlane, true));
	}

	inline vec3 toGlm(const ovrVector3f & ov) {
		return glm::make_vec3(&ov.x);
	}

	inline vec2 toGlm(const ovrVector2f & ov) {
		return glm::make_vec2(&ov.x);
	}

	inline uvec2 toGlm(const ovrSizei & ov) {
		return uvec2(ov.w, ov.h);
	}

	inline quat toGlm(const ovrQuatf & oq) {
		return glm::make_quat(&oq.x);
	}

	inline mat4 toGlm(const ovrPosef & op) {
		mat4 orientation = glm::mat4_cast(toGlm(op.Orientation));
		mat4 translation = glm::translate(mat4(), ovr::toGlm(op.Position));
		return translation * orientation;
	}

	inline mat4 toGlm(const ovrPosef & op, GLuint eye, GLuint _tracking_state, glm::mat4 &origOrientation, glm::vec3 origEyePoses[2]) {

		mat4 orientation = origOrientation;
		if (_tracking_state == 0) {
			orientation = glm::mat4_cast(toGlm(op.Orientation));
			origOrientation = orientation;
		}

		mat4 translation = glm::translate(mat4(), origEyePoses[eye]);
		if (_tracking_state == 0) {
			translation = glm::translate(mat4(), ovr::toGlm(op.Position));
			origEyePoses[eye] = ovr::toGlm(op.Position);
		}

		return translation * orientation;
	}

	inline ovrMatrix4f fromGlm(const mat4 & m) {
		ovrMatrix4f result;
		mat4 transposed(glm::transpose(m));
		memcpy(result.M, &(transposed[0][0]), sizeof(float) * 16);
		return result;
	}

	inline ovrVector3f fromGlm(const vec3 & v) {
		ovrVector3f result;
		result.x = v.x;
		result.y = v.y;
		result.z = v.z;
		return result;
	}

	inline ovrVector2f fromGlm(const vec2 & v) {
		ovrVector2f result;
		result.x = v.x;
		result.y = v.y;
		return result;
	}

	inline ovrSizei fromGlm(const uvec2 & v) {
		ovrSizei result;
		result.w = v.x;
		result.h = v.y;
		return result;
	}

	inline ovrQuatf fromGlm(const quat & q) {
		ovrQuatf result;
		result.x = q.x;
		result.y = q.y;
		result.z = q.z;
		result.w = q.w;
		return result;
	}
}

class RiftManagerApp {
protected:
	ovrSession _session;
	ovrHmdDesc _hmdDesc;
	ovrGraphicsLuid _luid;

public:
	RiftManagerApp() {
		if (!OVR_SUCCESS(ovr_Create(&_session, &_luid))) {
			FAIL("Unable to create HMD session");
		}

		_hmdDesc = ovr_GetHmdDesc(_session);
	}

	~RiftManagerApp() {
		ovr_Destroy(_session);
		_session = nullptr;
	}
};

class RiftApp : public GlfwApp, public RiftManagerApp {
public:

private:
	
	GLuint _depthBuffer{ 0 };
	ovrTextureSwapChain _eyeTexture;

	GLuint _mirrorFbo{ 0 };
	ovrMirrorTexture _mirrorTexture;

	ovrEyeRenderDesc _eyeRenderDescs[2];

	mat4 _eyeProjections[2];

	ovrLayerEyeFov _sceneLayer;
	ovrViewScaleDesc _viewScaleDesc;

	uvec2 _renderTargetSize;

	ovrEyeRenderDesc EyeRenderDesc[2];
	float IOD = 0.0f;

	glm::mat4 origOrientation = glm::mat4(1.0);
	glm::vec3 origEyePoses[2];

	GLuint _scene_state = 0;
	GLuint _view_state = 0;
	GLuint _tracking_state = 0;
	GLboolean a_button_down = false;
	GLboolean r_thumb_down = false;
	GLboolean x_button_down = false;
	GLboolean b_button_down = false;

public:

	
	void ConfigureRendering(const ovrFovPort * fov = 0)
	{
		// If any values are passed as NULL, then we use the default basic case
		if (!fov) fov = ovr_GetHmdDesc(_session).DefaultEyeFov;
		EyeRenderDesc[0] = ovr_GetRenderDesc(_session, ovrEye_Left, fov[0]);
		EyeRenderDesc[1] = ovr_GetRenderDesc(_session, ovrEye_Right, fov[1]);
	}

	ovrTrackingState GetEyePoses(ovrPosef * useEyeRenderPose = 0, float * scaleIPD = 0, float * newIPD = 0)
	{
		// Get both eye poses simultaneously, with IPD offset already included. 
		ovrVector3f useHmdToEyeOffset[2] = { EyeRenderDesc[0].HmdToEyeOffset, EyeRenderDesc[1].HmdToEyeOffset };

		// If any values are passed as NULL, then we use the default basic case
		//if (!useEyeRenderPose) useEyeRenderPose = EyeRenderPose;
		if (scaleIPD)
		{
			useHmdToEyeOffset[0].x *= *scaleIPD;
			useHmdToEyeOffset[1].x *= *scaleIPD;
		}
		if (newIPD)
		{
			useHmdToEyeOffset[0].x = -(*newIPD * 0.5f);
			useHmdToEyeOffset[1].x = +(*newIPD * 0.5f);
		}

		double ftiming = ovr_GetPredictedDisplayTime(_session, 0);
		ovrTrackingState trackingState = ovr_GetTrackingState(_session, ftiming, ovrTrue);

		ovr_CalcEyePoses(trackingState.HeadPose.ThePose, useHmdToEyeOffset, useEyeRenderPose);

		return(trackingState);
	}
	

	RiftApp() {
		using namespace ovr;
		_viewScaleDesc.HmdSpaceToWorldScaleInMeters = 1.0f;
		
		memset(&_sceneLayer, 0, sizeof(ovrLayerEyeFov));
		_sceneLayer.Header.Type = ovrLayerType_EyeFov;
		_sceneLayer.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft;
		
		ovr::for_each_eye([&](ovrEyeType eye) {
			ovrEyeRenderDesc& erd = _eyeRenderDescs[eye] = ovr_GetRenderDesc(_session, eye, _hmdDesc.DefaultEyeFov[eye]);
			ovrMatrix4f ovrPerspectiveProjection =
				ovrMatrix4f_Projection(erd.Fov, 0.01f, 1000.0f, ovrProjection_ClipRangeOpenGL);
			_eyeProjections[eye] = ovr::toGlm(ovrPerspectiveProjection);
			_viewScaleDesc.HmdToEyeOffset[eye] = erd.HmdToEyeOffset;

			ovrFovPort & fov = _sceneLayer.Fov[eye] = _eyeRenderDescs[eye].Fov;
			auto eyeSize = ovr_GetFovTextureSize(_session, eye, fov, 1.0f);
			_sceneLayer.Viewport[eye].Size = eyeSize;
			_sceneLayer.Viewport[eye].Pos = { (int)_renderTargetSize.x, 0 };

			_renderTargetSize.y = std::max(_renderTargetSize.y, (uint32_t)eyeSize.h);
			_renderTargetSize.x += eyeSize.w;
		});

		// Make the on screen window 1/4 the resolution of the render target
		_mirrorSize = _renderTargetSize;
		_mirrorSize /= 4;

	}

protected:
	uvec2 _mirrorSize;
	GLuint _fbo{ 0 };

	GLFWwindow * createRenderingTarget(uvec2 & outSize, ivec2 & outPosition) override {
		return glfw::createWindow(_mirrorSize);
	}

	void initGl() override {
		GlfwApp::initGl();

		// Disable the v-sync for buffer swap
		glfwSwapInterval(0);

		ovrTextureSwapChainDesc desc = {};
		desc.Type = ovrTexture_2D;
		desc.ArraySize = 1;
		desc.Width = _renderTargetSize.x;
		desc.Height = _renderTargetSize.y;
		desc.MipLevels = 1;
		desc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
		desc.SampleCount = 1;
		desc.StaticImage = ovrFalse;
		ovrResult result = ovr_CreateTextureSwapChainGL(_session, &desc, &_eyeTexture);
		_sceneLayer.ColorTexture[0] = _eyeTexture;
		if (!OVR_SUCCESS(result)) {
			FAIL("Failed to create swap textures");
		}

		int length = 0;
		result = ovr_GetTextureSwapChainLength(_session, _eyeTexture, &length);
		if (!OVR_SUCCESS(result) || !length) {
			FAIL("Unable to count swap chain textures");
		}
		for (int i = 0; i < length; ++i) {
			GLuint chainTexId;
			ovr_GetTextureSwapChainBufferGL(_session, _eyeTexture, i, &chainTexId);
			glBindTexture(GL_TEXTURE_2D, chainTexId);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		glBindTexture(GL_TEXTURE_2D, 0);

		// Set up the framebuffer object
		glGenFramebuffers(1, &_fbo);
		glGenRenderbuffers(1, &_depthBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
		glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, _renderTargetSize.x, _renderTargetSize.y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		ovrMirrorTextureDesc mirrorDesc;
		memset(&mirrorDesc, 0, sizeof(mirrorDesc));
		mirrorDesc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
		mirrorDesc.Width = _mirrorSize.x;
		mirrorDesc.Height = _mirrorSize.y;
		if (!OVR_SUCCESS(ovr_CreateMirrorTextureGL(_session, &mirrorDesc, &_mirrorTexture))) {
			FAIL("Could not create mirror texture");
		}
		glGenFramebuffers(1, &_mirrorFbo);
	}

	void onKey(int key, int scancode, int action, int mods) override {
		if (GLFW_PRESS == action) switch (key) {
		case GLFW_KEY_R:
			ovr_RecenterTrackingOrigin(_session);
			return;
		}

		GlfwApp::onKey(key, scancode, action, mods);
	}

	void draw() final override {

		ovrInputState inputState;
		ovr_GetInputState(_session, ovrControllerType_Touch, &inputState);

		// Modifies IOD distance
		ovrPosef eyePoses[2];

		ovrVector3f left_offset = _viewScaleDesc.HmdToEyeOffset[0];
		ovrVector3f right_offset = _viewScaleDesc.HmdToEyeOffset[1];

		// Right might be left, left might be right, my bad lol
		ovrVector3f offsets[2] = { left_offset, right_offset };
		ovr_GetEyePoses(_session, frame, true, offsets, eyePoses, &_sceneLayer.SensorSampleTime);
		
		int curIndex;
		ovr_GetTextureSwapChainCurrentIndex(_session, _eyeTexture, &curIndex);
		GLuint curTexId;
		ovr_GetTextureSwapChainBufferGL(_session, _eyeTexture, curIndex, &curTexId);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, curTexId, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0, 0, 0);

		// Render to each eye accordingly
		renderToEyes(eyePoses);

		// render the frame to the window
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		ovr_CommitTextureSwapChain(_session, _eyeTexture);
		ovrLayerHeader* headerList = &_sceneLayer.Header;
		ovr_SubmitFrame(_session, frame, &_viewScaleDesc, &headerList, 1);

		// for desktop display
		GLuint mirrorTextureId;
		ovr_GetMirrorTextureBufferGL(_session, _mirrorTexture, &mirrorTextureId);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _mirrorFbo);
		glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirrorTextureId, 0);
		glBlitFramebuffer(0, 0, _mirrorSize.x, _mirrorSize.y, 0, _mirrorSize.y, _mirrorSize.x, 0, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	}

	void renderToEyes(ovrPosef * eyePoses) {

		ovrInputState inputState;
		ovr_GetInputState(_session, ovrControllerType_Touch, &inputState);

		// tracking toggling
		if (inputState.Buttons & ovrTouch_B && !b_button_down) {
			_tracking_state = (_tracking_state + 1) % 2;
			b_button_down = true;
		}
		else if (!inputState.Buttons && b_button_down) {
			b_button_down = false;
		}
		

		// 3D stereo rendering
		
		ovr::for_left_eye([&](ovrEyeType eye) {
			const auto& vp = _sceneLayer.Viewport[eye];
			glm::vec4 viewport(vp.Pos.x, vp.Pos.y, vp.Size.w, vp.Size.h);
			glViewport(vp.Pos.x, vp.Pos.y, vp.Size.w, vp.Size.h);
			_sceneLayer.RenderPose[eye] = eyePoses[eye];
			renderScene(_eyeProjections[eye], ovr::toGlm(eyePoses[eye], 0, _tracking_state, origOrientation, origEyePoses), viewport, 0);

		});
		
		
		ovr::for_right_eye([&](ovrEyeType eye) {
			const auto& vp = _sceneLayer.Viewport[eye];
			glm::vec4 viewport(vp.Pos.x, vp.Pos.y, vp.Size.w, vp.Size.h);
			glViewport(vp.Pos.x, vp.Pos.y, vp.Size.w, vp.Size.h);
			_sceneLayer.RenderPose[eye] = eyePoses[eye];
			renderScene(_eyeProjections[eye], ovr::toGlm(eyePoses[eye], 1, _tracking_state, origOrientation, origEyePoses), viewport, 1);
		});
		
		
		/*
		ovr::for_each_eye([&](ovrEyeType eye) {
			const auto& vp = _sceneLayer.Viewport[eye];
			glViewport(vp.Pos.x, vp.Pos.y, vp.Size.w, vp.Size.h);
			_sceneLayer.RenderPose[eye] = eyePoses[eye];
			renderScene(_eyeProjections[eye], ovr::toGlm(eyePoses[eye]));
		});
		*/

	}

	virtual void renderScene(const glm::mat4 & projection, const glm::mat4 & headPose, const glm::vec4 viewport, const int eyeNum) = 0;
};

//////////////////////////////////////////////////////////////////////
// An example application that renders a simple cube
//////////////////////////////////////////////////////////////////////

#include "rpc/client.h"
#include "WinBanner.h"
#include "irrKlang.h"
using namespace irrklang;
ISoundEngine *SoundEngine = createIrrKlangDevice();
int player = 2;

rpc::client c("localhost", 8080);
class ExampleApp : public RiftApp {
	std::shared_ptr<GameScene> caveScene;
	bool soundplayed = false;
public:
	ExampleApp() { }

protected:

	glm::vec3 headPos() {
		ovrTrackingState hmdState = GetEyePoses();
		float x = hmdState.HeadPose.ThePose.Position.x;
		float y = hmdState.HeadPose.ThePose.Position.y;
		float z = hmdState.HeadPose.ThePose.Position.z;
		glm::vec3 pos(x, y, z);
		return pos;
	}

	glm::vec4 headOrientation() {
		ovrTrackingState hmdState = GetEyePoses();
		float x = hmdState.HeadPose.ThePose.Orientation.x;
		float y = hmdState.HeadPose.ThePose.Orientation.y;
		float z = hmdState.HeadPose.ThePose.Orientation.z;
		float w = hmdState.HeadPose.ThePose.Orientation.w;
		glm::vec4 rot(x, y, z, w);
		return rot;
	}

	glm::vec3 leftControllerPos() {
		ovrTrackingState hmdState = GetEyePoses();
		float x = hmdState.HandPoses[ovrHand_Left].ThePose.Position.x;
		float y = hmdState.HandPoses[ovrHand_Left].ThePose.Position.y;
		float z = hmdState.HandPoses[ovrHand_Left].ThePose.Position.z;
		glm::vec3 pos(x, y, z);
		return pos;
	}

	glm::vec3 rightControllerPos() {
		ovrTrackingState hmdState = GetEyePoses();
		float x = hmdState.HandPoses[ovrHand_Right].ThePose.Position.x;
		float y = hmdState.HandPoses[ovrHand_Right].ThePose.Position.y;
		float z = hmdState.HandPoses[ovrHand_Right].ThePose.Position.z;
		glm::vec3 pos(x, y, z);
		return pos;
	}

	glm::vec4 leftControllerOrientation() {
		ovrTrackingState hmdState = GetEyePoses();
		float x = hmdState.HandPoses[ovrHand_Left].ThePose.Orientation.x;
		float y = hmdState.HandPoses[ovrHand_Left].ThePose.Orientation.y;
		float z = hmdState.HandPoses[ovrHand_Left].ThePose.Orientation.z;
		float w = hmdState.HandPoses[ovrHand_Left].ThePose.Orientation.w;
		glm::vec4 rot(x, y, z, w);
		return rot;
	}

	glm::vec4 rightControllerOrientation() {
		ovrTrackingState hmdState = GetEyePoses();
		float x = hmdState.HandPoses[ovrHand_Right].ThePose.Orientation.x;
		float y = hmdState.HandPoses[ovrHand_Right].ThePose.Orientation.y;
		float z = hmdState.HandPoses[ovrHand_Right].ThePose.Orientation.z;
		float w = hmdState.HandPoses[ovrHand_Right].ThePose.Orientation.w;
		glm::vec4 rot(x, y, z, w);
		return rot;
	}

	bool triggerSqueezed() {
		ovrInputState inputState;
		ovr_GetInputState(_session, ovrControllerType_Touch, &inputState);
		if (inputState.IndexTrigger[ovrHand_Right] > .5f) {
			return true;
		}
		return false;
	}

	void initGl() override {
		RiftApp::initGl();
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		ovr_RecenterTrackingOrigin(_session);

		caveScene = std::shared_ptr<GameScene>(new GameScene());
	}

	void shutdownGl() override {
		c.call("RemovePlayer");
		caveScene.reset();
	}

	void GetServerInformation() {
		
		if (player == 1) {
			caveScene->player2Data.headPos = glm::vec3(c.call("GetHeadPos_P2.x").as<float>(), c.call("GetHeadPos_P2.y").as<float>(), c.call("GetHeadPos_P2.z").as<float>());
			caveScene->player2Data.headOrientation = glm::vec4(c.call("GetHeadOrientation_P2.x").as<float>(), c.call("GetHeadOrientation_P2.y").as<float>(), c.call("GetHeadOrientation_P2.z").as<float>(), c.call("GetHeadOrientation_P2.w").as<float>());
			caveScene->player2Data.leftHandPos = glm::vec3(c.call("GetLeftHandPos_P2.x").as<float>(), c.call("GetLeftHandPos_P2.y").as<float>(), c.call("GetLeftHandPos_P2.z").as<float>());
			caveScene->player2Data.leftHandOrientation = glm::vec4(c.call("GetLeftHandOrientation_P2.x").as<float>(), c.call("GetLeftHandOrientation_P2.y").as<float>(), c.call("GetLeftHandOrientation_P2.z").as<float>(), c.call("GetLeftHandOrientation_P2.w").as<float>());
			caveScene->player2Data.rightHandPos = glm::vec3(c.call("GetRightHandPos_P2.x").as<float>(), c.call("GetRightHandPos_P2.y").as<float>(), c.call("GetRightHandPos_P2.z").as<float>());
			caveScene->player2Data.rightHandOrientation = glm::vec4(c.call("GetRightHandOrientation_P2.x").as<float>(), c.call("GetRightHandOrientation_P2.y").as<float>(), c.call("GetRightHandOrientation_P2.z").as<float>(), c.call("GetRightHandOrientation_P2.w").as<float>());
			caveScene->player2Data.triggerSqueezed = c.call("GetTriggerSqueezed_P2").as<bool>();
		}
		else if (player == 2) {
	
			caveScene->player2Data.headPos = glm::vec3(c.call("GetHeadPos_P1.x").as<float>(), c.call("GetHeadPos_P1.y").as<float>(), c.call("GetHeadPos_P1.z").as<float>());
			caveScene->player2Data.headOrientation = glm::vec4(c.call("GetHeadOrientation_P1.x").as<float>(), c.call("GetHeadOrientation_P1.y").as<float>(), c.call("GetHeadOrientation_P1.z").as<float>(), c.call("GetHeadOrientation_P1.w").as<float>());
			caveScene->player2Data.leftHandPos = glm::vec3(c.call("GetLeftHandPos_P1.x").as<float>(), c.call("GetLeftHandPos_P1.y").as<float>(), c.call("GetLeftHandPos_P1.z").as<float>());
			caveScene->player2Data.leftHandOrientation = glm::vec4(c.call("GetLeftHandOrientation_P1.x").as<float>(), c.call("GetLeftHandOrientation_P1.y").as<float>(), c.call("GetLeftHandOrientation_P1.z").as<float>(), c.call("GetLeftHandOrientation_P1.w").as<float>());
			caveScene->player2Data.rightHandPos = glm::vec3(c.call("GetRightHandPos_P1.x").as<float>(), c.call("GetRightHandPos_P1.y").as<float>(), c.call("GetRightHandPos_P1.z").as<float>());
			caveScene->player2Data.rightHandOrientation = glm::vec4(c.call("GetRightHandOrientation_P1.x").as<float>(), c.call("GetRightHandOrientation_P1.y").as<float>(), c.call("GetRightHandOrientation_P1.z").as<float>(), c.call("GetRightHandOrientation_P1.w").as<float>());
			caveScene->player2Data.triggerSqueezed = c.call("GetTriggerSqueezed_P1").as<bool>();
		}
	}

	void SaveServerInformation() {

		if (player == 1) {
			c.call("SetHeadPos_P1.x", caveScene->player1Data.headPos.x); c.call("SetHeadPos_P1.y", caveScene->player1Data.headPos.y); c.call("SetHeadPos_P1.z", caveScene->player1Data.headPos.z);
			c.call("SetHeadOrientation_P1.x", caveScene->player1Data.headOrientation.x); c.call("SetHeadOrientation_P1.y", caveScene->player1Data.headOrientation.y); c.call("SetHeadOrientation_P1.z", caveScene->player1Data.headOrientation.z); c.call("SetHeadOrientation_P1.w", caveScene->player1Data.headOrientation.w);
			c.call("SetLeftHandPos_P1.x", caveScene->player1Data.leftHandPos.x); c.call("SetLeftHandPos_P1.y", caveScene->player1Data.leftHandPos.y); c.call("SetLeftHandPos_P1.z", caveScene->player1Data.leftHandPos.z);
			c.call("SetLeftHandOrientation_P1.x", caveScene->player1Data.leftHandOrientation.x); c.call("SetLeftHandOrientation_P1.y", caveScene->player1Data.leftHandOrientation.y); c.call("SetLeftHandOrientation_P1.z", caveScene->player1Data.leftHandOrientation.z); c.call("SetLeftHandOrientation_P1.w", caveScene->player1Data.leftHandOrientation.w);
			c.call("SetRightHandPos_P1.x", caveScene->player1Data.rightHandPos.x); c.call("SetRightHandPos_P1.y", caveScene->player1Data.rightHandPos.y); c.call("SetRightHandPos_P1.z", caveScene->player1Data.rightHandPos.z);
			c.call("SetRightHandOrientation_P1.x", caveScene->player1Data.rightHandOrientation.x); c.call("SetRightHandOrientation_P1.y", caveScene->player1Data.rightHandOrientation.y); c.call("SetRightHandOrientation_P1.z", caveScene->player1Data.rightHandOrientation.z); c.call("SetRightHandOrientation_P1.w", caveScene->player1Data.rightHandOrientation.w);
			c.call("SetTriggerSqueezed_P1", caveScene->player1Data.triggerSqueezed);
		}

		else if (player == 2) {

			c.call("SetHeadPos_P2.x", caveScene->player1Data.headPos.x); c.call("SetHeadPos_P2.y", caveScene->player1Data.headPos.y); c.call("SetHeadPos_P2.z", caveScene->player1Data.headPos.z);
			c.call("SetHeadOrientation_P2.x", caveScene->player1Data.headOrientation.x); c.call("SetHeadOrientation_P2.y", caveScene->player1Data.headOrientation.y); c.call("SetHeadOrientation_P2.z", caveScene->player1Data.headOrientation.z); c.call("SetHeadOrientation_P2.w", caveScene->player1Data.headOrientation.w);
			c.call("SetLeftHandPos_P2.x", caveScene->player1Data.leftHandPos.x); c.call("SetLeftHandPos_P2.y", caveScene->player1Data.leftHandPos.y); c.call("SetLeftHandPos_P2.z", caveScene->player1Data.leftHandPos.z);
			c.call("SetLeftHandOrientation_P2.x", caveScene->player1Data.leftHandOrientation.x); c.call("SetLeftHandOrientation_P2.y", caveScene->player1Data.leftHandOrientation.y); c.call("SetLeftHandOrientation_P2.z", caveScene->player1Data.leftHandOrientation.z); c.call("SetLeftHandOrientation_P2.w", caveScene->player1Data.leftHandOrientation.w);
			c.call("SetRightHandPos_P2.x", caveScene->player1Data.rightHandPos.x); c.call("SetRightHandPos_P2.y", caveScene->player1Data.rightHandPos.y); c.call("SetRightHandPos_P2.z", caveScene->player1Data.rightHandPos.z);
			c.call("SetRightHandOrientation_P2.x", caveScene->player1Data.rightHandOrientation.x); c.call("SetRightHandOrientation_P2.y", caveScene->player1Data.rightHandOrientation.y); c.call("SetRightHandOrientation_P2.z", caveScene->player1Data.rightHandOrientation.z); c.call("SetRightHandOrientation_P2.w", caveScene->player1Data.rightHandOrientation.w);
			c.call("SetTriggerSqueezed_P2", caveScene->player1Data.triggerSqueezed);
		}
	}

	void print(glm::vec3 v) {
		OutputDebugString("(");
		OutputDebugString(to_string(v.x).c_str());
		OutputDebugString(", ");
		OutputDebugString(to_string(v.y).c_str());
		OutputDebugString(", ");
		OutputDebugString(to_string(v.z).c_str());
		OutputDebugString(")\n");
	}

	void renderScene(const glm::mat4 & projection, const glm::mat4 & headPose, glm::vec4 viewport, const int eyeNum) override {
		
		ovrInputState inputState;
		ovr_GetInputState(_session, ovrControllerType_Touch, &inputState);

		// pass in info about the hmd to our scene

		glm::vec4 headinfo = headOrientation();
		glm::quat q = glm::angleAxis(1.5708f, glm::vec3(1.0f, 0.0f, 0.0f));
		q = q * glm::angleAxis(1.5708f, glm::vec3(0.0f, 1.0f, 0.0f));
		q = q * glm::angleAxis(-1.5708f, glm::vec3(0.0f, 0.0f, 1.0f));

		caveScene->player1Data.headPos = glm::mat3(glm::toMat4(q)) * headPos();
		caveScene->player1Data.headOrientation = adjust(headOrientation(), q);
		caveScene->player1Data.leftHandPos = glm::mat3(glm::toMat4(q)) * leftControllerPos();
		caveScene->player1Data.rightHandPos = glm::mat3(glm::toMat4(q)) * rightControllerPos();
		caveScene->player1Data.leftHandOrientation = adjust(leftControllerOrientation(), q);
		caveScene->player1Data.rightHandOrientation = adjust(rightControllerOrientation(), q);
		caveScene->player1Data.triggerSqueezed = triggerSqueezed() && (c.call("CheckWinState").as<int>() == 0);

		if (c.call("CheckWinState").as<int>() == 1 && !soundplayed) {
			SoundEngine->play2D("audio/player1wins.mp3", GL_TRUE);
			soundplayed = true;
		}
		else if (c.call("CheckWinState").as<int>() == 2 && !soundplayed) {
			SoundEngine->play2D("audio/player2wins.mp3", GL_TRUE);
			soundplayed = true;
		}

		// get the information about the other player from the server
		GetServerInformation();

		// render the scene
		caveScene->render(projection, glm::inverse(glm::toMat4(q) * headPose), player);

		// set the updated values back to the server
		SaveServerInformation();

	}

	glm::vec4 adjust(glm::vec4 original, glm::quat to_rotate) {
		glm::quat qorig = glm::quat(original.w, original.x, original.y, original.z);
		glm::quat adjusted = to_rotate * qorig;
		return glm::vec4(adjusted.x, adjusted.y, adjusted.z, adjusted.w);
	}

};

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	int result = -1;

	c.call("AddPlayer");
	player = c.call("GetNumPlayers").as<int>();

	if (player == 2) {
		try {
			if (!OVR_SUCCESS(ovr_Initialize(nullptr))) {
				FAIL("Failed to initialize the Oculus SDK");
			}

			SoundEngine->play2D("./audio/spearofjustice.mp3", GL_TRUE);
			result = ExampleApp().run();
		}
		catch (std::exception & error) {
			OutputDebugStringA(error.what());
			std::cerr << error.what() << std::endl;
		}
		ovr_Shutdown();
	}
	else {
		result = LeapApp(player).run();
	}

	SoundEngine->drop();
	return result;
}