#include "AnaglyphCam.h"

AnaglyphCam::AnaglyphCam(float iod, float ratio, float fov, float convergence, float near, float far) {
	this->EyeSeparation = iod;
	this->AspectRatio = ratio;
	this->FOV = fov;
	this->Convergence = convergence;
	this->NearClip = near;
	this->FarClip = far;
}

glm::mat4 AnaglyphCam::LeftFrustum() {
	float top, bottom, left, right;
	float a, b, c;

	CalculateABC(&a, &b, &c);

	bottom = NearClip * tan(FOV / 2);
	top = -bottom;
	left = -b * NearClip / Convergence;
	right = c * NearClip / Convergence;

	return glm::frustum(left, right, top, bottom, NearClip, FarClip);
}

glm::mat4 AnaglyphCam::RightFrustum() {
	float top, bottom, left, right;
	float a, b, c;

	CalculateABC(&a, &b, &c);

	bottom = NearClip * tan(FOV / 2);
	top = -bottom;
	left = -c * NearClip / Convergence;
	right = b * NearClip / Convergence;

	return glm::frustum(left, right, top, bottom, NearClip, FarClip);
}

float AnaglyphCam::getIOD() {
	return this->EyeSeparation;
}

void AnaglyphCam::CalculateABC(float * a, float * b, float * c) {
	*a = AspectRatio * tan(FOV / 2) * Convergence;
	*b = *a - EyeSeparation / 2;
	*c = *a + EyeSeparation / 2;
}