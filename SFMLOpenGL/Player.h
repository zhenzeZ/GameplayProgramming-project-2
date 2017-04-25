#include "Game.h"

#ifndef PLAYER_H
#define PLAYER_H

#pragma once

class Player
{
public:
	Player();
	~Player();

	void update(double t);

	glm::vec3 getPlayerPos();

	float getdistancePosX();
	float getdistancePosY();
private:
	sf::Vector3f m_motion;
	float m_gravity;
	float m_acceleration;
	float m_time;

	glm::vec3 playerPos;
	glm::vec3 distance;

	bool jump;
	int timesToJump;
	int pixelsToMetres;
};

#endif