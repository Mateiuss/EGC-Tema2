#pragma once
#include "components/simple_scene.h"
#include "lab_m1/Tema2/Projectile.h"
#include <string>

class Tanc
{
public:
	Tanc();
	Tanc(glm::vec3 position, glm::vec3 forward, float angle);
	~Tanc();

	void rotate_all(float angle);
	void translate_all(float distance);

	void rotate_turret(float angle);

	void set_colors(glm::vec3 senile_color, glm::vec3 corp_color, glm::vec3 turela_color, glm::vec3 tun_color);

	Projectile shoot();

	glm::vec3 position;
	glm::vec3 forward_body;
	glm::vec3 forward_turret;
	float body_angle;
	float turret_angle;
	std::string senile;
	std::string corp;
	std::string turela;
	std::string tun;
	float timeFromLastShot;
	glm::vec3 senile_color;
	glm::vec3 corp_color;
	glm::vec3 turela_color;
	glm::vec3 tun_color;
	float radius;
	int hp;
};

