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

	float textTime;
	sf::Text displayText;

	bool software;

	void BuildShader();
	void SetCamera(sf::Vector3f pos, sf::Vector3f up, sf::Vector3f look);
	void AddObject(Object* newobject);
	void LoadXML(const char* filename);
	std::string run();

	sf::Vector3f getPos();
	void setPos(Vector3f newPos);

	sf::Vector3f pos;

	void showText(float timeToDisplay, const char* textToDisplay, int charSize);
};

#endif