#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "dependencies.h"

namespace graphics
{
	void initialize();

	sf::RenderWindow* getWindow();

	sf::RectangleShape* getShape();

	sf::Vector2f getResolution();
}

#endif