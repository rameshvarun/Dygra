#ifndef OBJECTS
#define OBJECTS

#include <SFML/Graphics.hpp>

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>

#include "template.h"

using namespace templating;

using namespace sf;

void replaceAll(std::string& str, const std::string& from, const std::string& to);

class Object
{
public:

	std::string name;
	bool uniform;

	const char* type;

	virtual std::string GetIntersectCode();

	std::map<const char*, Object*> objects;

	bool castshadows;
	bool recieveshadows;

	virtual Node* getContext();
};

class Sphere : public Object
{
public:
	Sphere(std::string n, float xpos, float ypos, float zpos, float r, float reflect, bool u, bool cast, bool recieve);

	float x;
	float y;
	float z;
	float radius;
	
	float reflectivity;

	virtual std::string GetIntersectCode();

	virtual Node* getContext();
};

class Plane : public Object
{
public:
	Plane(std::string n, float xpos, float ypos, float zpos, float wpos, float reflect, bool u, bool recieve);

	float x;
	float y;
	float z;
	float w;

	float reflectivity;

	virtual std::string GetIntersectCode();

	virtual Node* getContext();
};


class SpherePortal : public Object
{
public:
	SpherePortal(std::string n, float r, float xpos1, float ypos1, float zpos1, float xpos2, float ypos2, float zpos2, bool u);

	float radius;

	float x1;
	float y1;
	float z1;
	
	float x2;
	float y2;
	float z2;

	bool cameraInside;

	virtual std::string GetIntersectCode();

	virtual Node* getContext();
};

class SphereAberration : public Object
{
public:
	SphereAberration(std::string n, float r, float xpos, float ypos, float zpos, float sx, float sy, float sz, bool u);

	float radius;

	float x;
	float y;
	float z;
	
	float scalex;
	float scaley;
	float scalez;

	bool cameraInside;

	virtual std::string GetIntersectCode();

	virtual Node* getContext();
};


class Box : public Object
{
public:
	Box(std::string n, float xpos1, float ypos1, float zpos1, float xpos2, float ypos2, float zpos2, bool u, bool cast, bool recieve);

	float x1;
	float y1;
	float z1;
	
	float x2;
	float y2;
	float z2;

	virtual std::string GetIntersectCode();

	virtual Node* getContext();
};

class BoxPortal : public Object
{
public:
	BoxPortal(const char* n,  Vector3f min_1, Vector3f min_2, Vector3f s, bool u);

	Vector3f min1;

	Vector3f min2;

	Vector3f size;

	bool cameraInside;

	virtual std::string GetIntersectCode();

	virtual Node* getContext();
};

class BoxAberration : public Object
{
public:
	BoxAberration(std::string n, Vector3f minimum, Vector3f maximum, Vector3f s, bool u);

	Vector3f min;
	Vector3f max;
	Vector3f scale;

	bool cameraInside;

	virtual std::string GetIntersectCode();

	virtual Node* getContext();
};



#endif