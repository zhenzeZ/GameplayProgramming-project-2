#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <GL/glew.h>
#include <GL/wglew.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include <Debug.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;
using namespace sf;
using namespace glm;

class Game
{
public:
	Game();
	Game(sf::ContextSettings settings);
	~Game();
	void run();
private:
	Window window;

	void initialize();
	void update();
	void render();
	void unload();
	void mouseMovement();
	void leftViewports();
	void rightViewports();
	void renderCube(mat4 &npc);

	bool firstMouse;
	bool isRunning;
	bool gameover = false;

	GLfloat lastX;
	GLfloat lastY;
	GLfloat diertionX;
	GLfloat diertionY;

	sf::Vector2i m_mousePos;

	sf::View leftView;
	sf::View RightView;

	sf::Clock m_clock;
	sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

};

#endif