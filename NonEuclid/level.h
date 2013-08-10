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

	Level();

	void BuildShader(sf::Vector2f resolution);

	sf::Shader *shader;

	lua_State* script;

	void SetCamera(sf::Vector3f pos, sf::Vector3f up, sf::Vector3f look);

	std::map<const char*, Object*> objects;

	void AddObject(Object* newobject);

	void LoadXML(const char* filename);
};

#endif