#include "Projectile.h"

Projectile::Projectile()
{
	speed = 2.5f;
	lifeTime = 0;
	scale = 0.05f;
	meshName = "sphere";
	damage = 40;
}

Projectile::~Projectile()
{
}

void Projectile::Set(glm::vec3 position, glm::vec3 forward)
{
	this->position = position;
	this->forward = forward;
}
