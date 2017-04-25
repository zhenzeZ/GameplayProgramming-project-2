#pragma once

#include "Game.h"
#include <SFML/Graphics.hpp>

class Hud
{
public:
	Hud();
	~Hud();

	void update(double t);
	void render(sf::RenderWindow & window);
private:

	int timer_mis = 0; // millisecond
	int timer_sec = 0; // second
	int timer_min = 0; // minute

	sf::Font m_font;
	sf::Text m_text;

	std::string intToString(int num);
};