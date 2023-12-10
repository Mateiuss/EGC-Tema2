#pragma once
#include "components/simple_scene.h"
#include "lab_m1/Tema2/Projectile.h"
#include <string>

class Tanc
{
public:
	enum MovingState {
		FORWARD,
		BACKWARD,
		ROTATE_LEFT,
		ROTATE_RIGHT,
		STOP,
		ROTATE_RIGHT_WHILE_FORWARD,
		ROTATE_LEFT_WHILE_FORWARD,
		ROTATE_RIGHT_WHILE_BACKWARD,
		ROTATE_LEFT_WHILE_BACKWARD
	};

	Tanc();
	Tanc(glm::vec3 position, glm::vec3 forward, float angle);
	~Tanc();

	void rotate_all(float angle);
	void translate_all(float distance);

	void rotate_turret(float angle);

	void set_colors(glm::vec3 senile_color, glm::vec3 corp_color, glm::vec3 turela_color, glm::vec3 tun_color);

	void get_random_moving_state();
	void move(float delta);

	Projectile shoot();
	void attack(float delta, glm::vec3 pos, std::vector<Projectile> &projectiles);

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
	float timeFromLastMove;
	glm::vec3 senile_color;
	glm::vec3 corp_color;
	glm::vec3 turela_color;
	glm::vec3 tun_color;
	float radius;
	int hp;
	MovingState movingState;
	float range;
	float forward_speed;
	float rotation_speed;
};

