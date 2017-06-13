/////////////////////
// Collider.cpp
/////////////////////

#include "Collider.h"

/////////////////////////////////////////////////////
glm::vec3 Collider::centerPos = glm::vec3(0, 0, 0);

void print(glm::vec3 v) {
	OutputDebugString("(");
	OutputDebugString(to_string(v.x).c_str());
	OutputDebugString(", ");
	OutputDebugString(to_string(v.y).c_str());
	OutputDebugString(", ");	
	OutputDebugString(to_string(v.z).c_str());
	OutputDebugString(")\n");
}

glm::vec3 proj(glm::vec3 u, glm::vec3 v) {
	float d = glm::dot(u, v) / glm::dot(v, v);
	glm::vec3 r = v * d;
	return r;
}

void Collider::SetBlueDelay(float t) {
	float fps = 90.0f;
	if (curTime/fps > t) {
		curTime = 0.0f;
		colliderObject->SetBlue();
	}
	else {
		curTime++;
	}
}

void Collider::SetColorDelay(Collider* otherObject) {
	if (otherObject->curCTime > 25.0f) {
		otherObject->curCTime = 0.0f;
		otherObject->destroyedBall = false;
	}
	else {
		otherObject->curCTime++;
	}
}

Collider::Collider() {
	curTime = 0.0f;
	curCTime = 0.0f;
}

Collider::Collider(Collider::type t, glm::vec3 p, glm::vec4 r, glm::vec3 s) {

	pos = p;
	rot = r;
	scale = s;
	vel = glm::vec3(0, 0, 0);

	switch (t) {
	case Collider::PlaneType:
		colliderObject = new Plane();
		break;
	case Collider::SphereType:
		colliderObject = new Sphere();
		break;
	}
	colliderType = t;
	active = true;
	isStatic = false;
	gravityScale = 1.0f;
	continuous = false;
	offset = true;
}

void Collider::ApplyForce(glm::vec3 v) {
	if (active == false || isStatic)
		return;
	vel = glm::vec3(vel.x + v.x, vel.y + v.y, vel.z + v.z);
}

void Collider::SetRadius(float r) {
	if (colliderType != Collider::SphereType) {
		return;
	}
	radius = r;
	colliderObject->SetRadius(r);
}

void Collider::SetNormal(glm::vec3 n) {
	if (colliderType != Collider::PlaneType)
		return;
	normal = n;
}

void Collider::SetPoint(glm::vec3 p) {
	if (colliderType != Collider::PlaneType)
		return;
	point = p;
}

void Collider::SetXBounds(float minX, float maxX) {
	if (colliderType == Collider::SphereType)
		return;
	xBounds = glm::vec2(minX, maxX);
	colliderObject->SetLength(maxX - minX);
}
void Collider::SetZBounds(float minZ, float maxZ) {
	if (colliderType == Collider::SphereType)
		return;
	zBounds = glm::vec2(minZ, maxZ);
	colliderObject->SetHeight(maxZ - minZ);
}

bool Collider::CheckForCollision(Collider * otherObject) {

	if (active == false || isStatic)
		return false;

	bool collision = false;
	float damping = -.95f;
	switch (colliderType) {
	case Collider::SphereType:
		switch (otherObject->colliderType) {

		// Sphere / Plane Collision
		case Collider::PlaneType:
			collision = CollisionTests::SpherePlane(offset ? pos - centerPos : pos, radius, otherObject->point - centerPos, otherObject->normal);
			if (collision == true) {

				// Check if the collision was in our bounds
				

				glm::vec3 u = otherObject->normal * (glm::dot(vel, otherObject->normal));
				glm::vec3 w = vel - u;
				glm::vec3 newVel = (w - u);
				vel = newVel * (.85f);

				Update();
				colliderObject->SetRed();
				otherObject->colliderObject->SetRed();
				
				// Deactive the object if its not moving enough (optimization)
				if (continuous == false) {
					float minVel = .009f;
					glm::vec3 absVel = glm::vec3(fabs(newVel.x), fabs(newVel.y), fabs(newVel.z));
					if (absVel.x < minVel && absVel.y < minVel && absVel.z < minVel) {
						vel = glm::vec3(vel.x, 0, vel.z);
						active = false;
						colliderObject->SetBlue();
					}
				}
				
			}
			else { 
				otherObject->SetBlueDelay(.2f);
				SetBlueDelay(.2f); 
			}
			break;

		// Sphere / Sphere Collision
		case Collider::SphereType:
			glm::vec3 p1 = offset ? pos - centerPos : pos;
			glm::vec3 p2 = otherObject->offset ? otherObject->pos - centerPos : otherObject->pos;
			collision = CollisionTests::SphereSphere(p1, radius, p2, otherObject->radius);

			if (otherObject->destroyedBall == true) {
				SetColorDelay(otherObject);
			}

			if (collision) {
				if (otherObject->active == false)
					otherObject->active = true;

				// Collide with hotspot
				if (otherObject->isStatic == true) {
					vel = glm::vec3(0, 0, 0);
					active = false;
					otherObject->destroyedBall = true;
				}

				// sphere 1
				glm::vec3 nv1 = vel;
				nv1 += proj(otherObject->vel, p2 - p1);
				nv1 -= proj(vel, p1 - p2);

				// sphere 2
				glm::vec3 nv2 = otherObject->vel;
				nv2 += proj(vel, p2 - p1);
				nv2 -= proj(otherObject->vel, p1 - p2);

				vel = nv1 * .85f;
				Update();

				otherObject->vel = nv2 * .85f;
				otherObject->Update();

				otherObject->colliderObject->SetRed();
				colliderObject->SetRed();
			}
			break;
		}
		break;
	}

	return collision;
}

void Collider::Update() {
	if (active == false)
		return;

	// visual representation
	colliderObject->position = offset?pos-centerPos:pos;
	colliderObject->rotation = rot;
	colliderObject->modelScale = scale;

	// Update collider position based off of current velocity
	pos = glm::vec3(pos.x + vel.x, pos.y + vel.y, pos.z + vel.z);

}

void Collider::ViewCollider(const glm::mat4 & projection, const glm::mat4 & modelview) {
	colliderObject->Render(projection, modelview);
}