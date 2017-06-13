/////////////////////
// Physics.cpp
/////////////////////

#include "Physics.h"

// Instantiate static vars
vector<Collider *> Physics::objects;
vector<pair<int, pair<int, int>>> Physics::recentCollisions;
bool Physics::debug = false;
const float Physics::gravity = .0002f;

void Physics::AddObject(Collider* c) {
	objects.push_back(c);
}

void Physics::RemoveObject(Collider* c) {
	for (int i = 0; i < objects.size(); i++) {
		if (c == objects[i]) {
			objects.erase(objects.begin() + i);
		}
	}
}

bool Contains(vector<int> v, int num) {
	bool c = false;
	for (int i = 0; i < v.size(); i++) {
		if (v[i] == num) {
			c = true;
		}
	}
	return c;
}

void Physics::Update(const glm::mat4 & projection, const glm::mat4 & modelview) {

	// Update recent collisions
	for (int i = 0; i < recentCollisions.size(); i++) {
		if (recentCollisions[i].first > 90) {
			recentCollisions.erase(recentCollisions.begin() + i);
		}
		else {
			recentCollisions[i].first++;
		}
	}

	// Apply gravity
	for (int i = 0; i < objects.size(); i++) {
		Collider* c = objects[i];
		c->ApplyForce(glm::vec3(0, -gravity * c->gravityScale, 0));
	}
	

	// Check for collisions
	vector<int> collisionTests;

	for (int i = 0; i < objects.size(); i++) {
		Collider * c1 = objects[i];
		if (c1->isStatic)
			continue;
		if (c1->active == false) {
			objects.erase(objects.begin() + i);
			continue;
		}

		if (Contains(collisionTests, i)) {
			continue;
		}

		for (int j = 0; j < objects.size(); j++) {
			Collider * c2 = objects[j];
			if (c1 != c2) {

				// check our recent collisions to avoid duplicates
				bool tooSoon = false;
				for (int z = 0; z < recentCollisions.size(); z++) {
					if (i == recentCollisions[z].second.first && j == recentCollisions[z].second.second) {
						tooSoon = true;;
					}
				}
				if (tooSoon)
					continue;

				bool wasCollision = c1->CheckForCollision(c2);
				if (wasCollision) {
					collisionTests.push_back(j);
					if (c2->isStatic == false) {
						recentCollisions.push_back(make_pair(0, make_pair(i, j)));
					}
				}
			}
		}

	}
	
	// Apply all of our velocity changes
	for (int i = 0; i < objects.size(); i++) {
		Collider* c = objects[i];
		c->Update();
	}

	// Render the colliders if we're in debug mode
	if (debug) {
		for (int i = 0; i < objects.size(); i++) {
			Collider *c = objects[i];
			c->ViewCollider(projection, modelview);
		}
	}
}