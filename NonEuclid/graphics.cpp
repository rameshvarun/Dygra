#include "graphics.h"

sf::RenderWindow* window;

sf::RectangleShape* shape;

bool fullscreen;

#define WINDOW_TITLE "Non Eucldiean Raytracer"
int WINDOW_WIDTH = 640;
int WINDOW_HEGIHT = 480;

sf::Font* FreeSansBold;

sf::Font* graphics::getFont(std::string fontname)
{
	if( fontname.compare("FreeSansBold") == 0 )
	{
		return FreeSansBold;
	}
}

void graphics::initialize(int width, int height, bool startFullscreen)
{
	WINDOW_WIDTH = width;
	WINDOW_HEGIHT = height;

	if(startFullscreen)
	{
		window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEGIHT),  WINDOW_TITLE, Style::Fullscreen);
	}
	else
	{
		window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEGIHT),  WINDOW_TITLE);
	}

	BOOST_LOG_TRIVIAL(debug) << "Window created.";


	window->setVerticalSyncEnabled(true);
	window->setMouseCursorVisible(false);

	shape = new sf::RectangleShape(sf::Vector2f(window->getSize().x,window->getSize().y));

	fullscreen = startFullscreen;

	//Load fonts
	FreeSansBold = new sf::Font();
	FreeSansBold->loadFromFile( "../content/font/FreeSansBold.ttf" );
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

void graphics::toggleFullscreen()
{
	fullscreen = !fullscreen;

	if(fullscreen)
	{
		window->create( sf::VideoMode(WINDOW_WIDTH, WINDOW_HEGIHT) , WINDOW_TITLE ,Style::Fullscreen);
	}
	else
	{
		window->create( sf::VideoMode(WINDOW_WIDTH, WINDOW_HEGIHT) , WINDOW_TITLE);
	}
}