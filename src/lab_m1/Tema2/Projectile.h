#pragma once
#include "components/simple_scene.h"
#include <string>

class Projectile
{
public:

	Projectile();
	~Projectile();

	void Set(glm::vec3 position, glm::vec3 forward);

	glm::vec3 position;
	glm::vec3 forward;
	float speed;
	float lifeTime;
	float scale;
	std::string meshName;
};

