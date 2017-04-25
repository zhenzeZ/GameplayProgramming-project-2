#include "Player.h"

Player::Player():
	m_motion(0.0f, 0.0f,0.0f),
	playerPos(0,0,0),
	distance(0,0,0),
	m_gravity(-9.8f),
	m_acceleration(0.0f),
	jump(false),
	timesToJump(2),
	pixelsToMetres(10),
	m_time(0)
{

}

Player::~Player()
{

}

void Player::update(double t)
{

	m_time = t;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_motion.x = -20.0f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_motion.x = 20.0f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !jump && timesToJump > 0)
	{
		m_motion.y = 50.0f;
		jump = true;
		timesToJump--;
	}

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && jump)
	{
		jump = false;
	}

	if (m_motion.y <= -100.0f)
	{
		timesToJump = 2;
	}

	m_acceleration = m_motion.x * 0.8 * m_gravity; // a = m * res * g

	m_motion.x = m_motion.x - m_acceleration * m_time; // v = v - a * t

	if (m_motion.x < 5 && m_motion.x > -5)
	{
		m_motion.x = 0;
	}
	
	if (playerPos.y >= 0 || jump)
	{
		m_motion.y = m_motion.y + m_gravity * m_time; // v = v + a *t
	}
	else if (playerPos.y < 0)
	{
		m_motion.y = 0;
		playerPos.y = 0;
	}
	distance.x = (m_motion.x * m_time + 0.5 * m_acceleration * m_time * m_time) / pixelsToMetres;
	distance.y = (m_motion.y * m_time + 0.5 * m_gravity * m_time * m_time) / pixelsToMetres;

	playerPos.x += distance.x;
	playerPos.y += distance.y;

}

float Player::getdistancePosX()
{
	return distance.x;
}

float Player::getdistancePosY()
{
	return distance.y;
}

glm::vec3 Player::getPlayerPos()
{
	return playerPos;
}


