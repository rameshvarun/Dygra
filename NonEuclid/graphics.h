#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "dependencies.h"

namespace graphics
{
	void initialize(int width, int height, bool startFullscreen);

	sf::RenderWindow* getWindow();

	sf::RectangleShape* getShape();

	sf::Vector2f getResolution();

	void toggleFullscreen();

	sf::Font* getFont(std::string fontname);
}

#endif