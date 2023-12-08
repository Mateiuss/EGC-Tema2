#include "Tanc.h"
#include <iostream>

Tanc::Tanc()
{
}

Tanc::Tanc(glm::vec3 position, glm::vec3 forward, float angle)
{
	this->position = position;
	this->forward_body = forward;
	this->body_angle = angle;
	this->forward_turret = forward;
	this->turret_angle = angle;
	this->senile = "senile";
	this->corp = "corp";
	this->turela = "turela";
	this->tun = "tun";
	this->timeFromLastShot = 2;
	this->radius = 0.45f;
	this->hp = 100;
	this->timeFromLastMove = 0;
	this->get_random_moving_state();
	this->range = 7.5f;
}

Tanc::~Tanc()
{
}

void Tanc::rotate_all(float angle)
{
	glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1), glm::radians(angle), glm::vec3(0, 1, 0));
	this->body_angle += angle;
	this->turret_angle += angle;
	this->forward_body = glm::vec3(rotation_matrix * glm::vec4(this->forward_body, 1));
	this->forward_turret = glm::vec3(rotation_matrix * glm::vec4(this->forward_turret, 1));
}

void Tanc::set_colors(glm::vec3 senile_color, glm::vec3 corp_color, glm::vec3 turela_color, glm::vec3 tun_color)
{
	this->senile_color = senile_color;
	this->corp_color = corp_color;
	this->turela_color = turela_color;
	this->tun_color = tun_color;
}

void Tanc::translate_all(float distance)
{
	this->position += glm::normalize(this->forward_body) * distance;
}

void Tanc::rotate_turret(float angle)
{
	glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1), glm::radians(angle), glm::vec3(0, 1, 0));
	this->turret_angle += angle;
	this->forward_turret = glm::vec3(rotation_matrix * glm::vec4(this->forward_turret, 1));
}

Projectile Tanc::shoot()
{
	Projectile p;
	p.Set(this->position + glm::normalize(this->forward_turret) * glm::float32_t(0.7f) + glm::vec3(0, 0.25, 0), this->forward_turret);
	this->timeFromLastShot = 0;
	return p;
}

void Tanc::get_random_moving_state()
{
	timeFromLastMove = 0;

	int random = rand() % 9;
	switch (random)
	{
	case 0:
		this->movingState = FORWARD;
		break;
	case 1:
		this->movingState = BACKWARD;
		break;
	case 2:
		this->movingState = ROTATE_LEFT;
		break;
	case 3:
		this->movingState = ROTATE_RIGHT;
		break;
	case 4:
		this->movingState = ROTATE_RIGHT_WHILE_FORWARD;
		break;
	case 5:
		this->movingState = ROTATE_LEFT_WHILE_FORWARD;
		break;
	case 6:
		this->movingState = ROTATE_RIGHT_WHILE_BACKWARD;
		break;
	case 7:
		this->movingState = ROTATE_LEFT_WHILE_BACKWARD;
		break;
	default:
		this->movingState = STOP;
		break;
	}
}

void Tanc::move(float delta)
{
	timeFromLastMove += delta;

	switch (this->movingState)
	{
	case FORWARD:
		this->translate_all(delta * 1.5f);
		break;
	case BACKWARD:
		this->translate_all(-delta * 1.5f);
		break;
	case ROTATE_LEFT:
		this->rotate_all(delta * 30);
		break;
	case ROTATE_RIGHT:
		this->rotate_all(-delta * 30);
		break;
	case ROTATE_RIGHT_WHILE_FORWARD:
		this->rotate_all(-delta * 30);
		this->translate_all(delta * 1.5f);
		break;
	case ROTATE_LEFT_WHILE_FORWARD:
		this->rotate_all(delta * 30);
		this->translate_all(delta * 1.5f);
		break;
	case ROTATE_RIGHT_WHILE_BACKWARD:
		this->rotate_all(-delta * 30);
		this->translate_all(-delta * 1.5f);
		break;
	case ROTATE_LEFT_WHILE_BACKWARD:
		this->rotate_all(delta * 30);
		this->translate_all(-delta * 1.5f);
		break;
	default:
		break;
	}
}

void Tanc::attack(float delta, glm::vec3 pos, std::vector<Projectile>& projectiles)
{
	// Check if the player is in range
	if (glm::distance(this->position, pos) > this->range) return;

	if (glm::distance(this->position, pos) < 4.f) this->movingState = STOP;

	// Move the turret towards the player
	glm::vec3 direction = glm::normalize(pos - this->position);
	float angle = glm::degrees(acos(glm::dot(direction, this->forward_turret)));

	if (angle > 2.f)
	{
		glm::vec3 cross = glm::cross(this->forward_turret, direction);
		float rotationDirection = glm::sign(cross.y);

		this->rotate_turret(rotationDirection * delta * 30);
	}
	else if (this->timeFromLastShot > 2)
	{
		projectiles.push_back(this->shoot());
		this->timeFromLastShot = 0;
	}
	else
	{
		this->timeFromLastShot += delta;
	}
}

