#include "Enemy.h"



Enemy::Enemy():
	enemyPos(-5, 0, -100)
{
	
}


Enemy::~Enemy()
{
}

void Enemy::update()
{
	enemyPos.z += 0.02;
	if (enemyPos.z > -2)
	{
		enemyPos.z = -100;
	}
}

vec3 Enemy::getEnemyPos()
{
	return enemyPos;
}