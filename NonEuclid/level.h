#ifndef LEVEL
#define LEVEL

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <map>

#include "objects.h"

#include "scripting.h"

class Level
{
public:

	sf::Shader *shader;
	lua_State* script;
	std::string renderer;
	std::map<const char*, Object*> objects;


	Level(std::string rendererType);


	float intersect(Vector3f ro, Vector3f rd);

	//Related to softwarer renderer
	sf::Texture softwareTexture;
	sf::Sprite softwareSprite;
	bool software;
	void softRender();

	void BuildShader();
	void SetCamera(sf::Vector3f pos, sf::Vector3f up, sf::Vector3f look);
	void AddObject(Object* newobject);
	void LoadXML(const char* filename);
	std::string run();

	//Functions exposed to script
	sf::Vector3f getPos();
	void setPos(Vector3f newPos);

	//Camera info
	sf::Vector3f pos;
	sf::Vector3f up;
	sf::Vector3f look;

	//Displaying hint text on screen
	float textTime;
	sf::Text displayText;
	void showText(float timeToDisplay, const char* textToDisplay, int charSize);
};

#endif