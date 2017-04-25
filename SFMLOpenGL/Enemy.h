#include "Game.h"
#ifndef ENEMY_H
#define ENEMY_H

#pragma once
class Enemy
{
public:
	Enemy();
	~Enemy();
	void update();
	glm::vec3 getEnemyPos();

private:
	glm::vec3 enemyPos;
};
#endif 
