#include "HUD.h"

Hud::Hud()
{
	if (!m_font.loadFromFile("c:/windows/fonts/MotorwerkOblique.ttf"))
	{
		std::string s("error loading font from file");
		throw std::exception(s.c_str());
	}

	m_text.setFont(m_font);
}

Hud::~Hud()
{
}

void Hud::update(double t)
{
	// timer part
	timer_mis += t * 100;

	if (timer_mis >= 100) // when millisecond great than 100, second add 1
	{
		timer_sec++;
		timer_mis = 0;
	}

	if (timer_sec >= 60) // when second over 60, minute add 1
	{
		timer_min++;
		timer_sec = 0;
	}

	m_text.setString(intToString(timer_min) + "::"
		+ intToString(timer_sec) + "::"
		+ intToString(timer_mis)); // convert minute to string
}

void Hud::render(sf::RenderWindow & window)
{
	window.draw(m_text);
}

std::string Hud::intToString(int num) {
	char numString[10];
	sprintf_s(numString, "%i", num);
	return numString;
}