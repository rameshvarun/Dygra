#include "graphics.h"

sf::RenderWindow* window;

sf::RectangleShape* shape;

void graphics::initialize()
{
	window = new sf::RenderWindow(sf::VideoMode(640, 480), "SFML Shader");
	window->setVerticalSyncEnabled(true);

	shape = new sf::RectangleShape(sf::Vector2f(window->getSize().x,window->getSize().y));
}

sf::RenderWindow* graphics::getWindow()
{
	return window;
}

sf::RectangleShape* graphics::getShape()
{
	return shape;
}

sf::Vector2f graphics::getResolution()
{
	return sf::Vector2f(window->getSize().x,window->getSize().y);
}