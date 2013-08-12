#include "objects.h"

#include "physfs.h"
#include "util.h"

void replaceAll(std::string& str, const std::string& from, const std::string& to)
{
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

std::string ShadowCode(std::map<const char*, Object*> objects, std::string name)
{
	std::stringstream shadow;

	for (std::map<const char*, Object*>::iterator it=objects.begin(); it!=objects.end(); ++it)
	{
		Object* obj = (*it).second;

		if(obj->name.compare( name ) != 0)
		{
			if(obj->type == "sphere" && obj->castshadows)
			{
				std::string shad = file_to_string( "shaders/raytracing/sphereshadow.frag" );

				replaceAll(shad, "{{NAME}}", obj->name);

				shadow << shad;
			}

			if(obj->type == "box" && obj->castshadows)
			{
				std::string shad = file_to_string( "shaders/raytracing/boxshadow.frag" );

				replaceAll(shad, "{{NAME}}", obj->name);

				shadow << shad;
			}
		}
	}

	return shadow.str();
}


#pragma region Object

Node* Object::getContext()
{
	Node* context = new Node();

	context->set("name", name.c_str() );
	context->set("type", type );

	if(uniform)
		context->set("uniform", "uniform" );

	context->set("castshadows", castshadows );
	context->set("recieveshadows", recieveshadows );

	if(recieveshadows)
	{
		context->set("shadowcode", ShadowCode(objects, name).c_str() );
	}

	return context;
}

float Object::intersect(Vector3f ro, Vector3f rd)
{
	return -1;
}

#pragma endregion

#pragma region Sphere

Sphere::Sphere(std::string n, float xpos, float ypos, float zpos, float r, float reflect, bool u, bool cast, bool recieve)
{
	name = n;
	x = xpos;
	y = ypos;
	z = zpos;
	uniform = u;

	reflectivity = reflect;

	radius = r;

	type = "sphere";

	castshadows = cast;
	recieveshadows = recieve;
}

Node* Sphere::getContext()
{
	Node* context = Object::getContext();

	context->set("reflectivity", reflectivity);
	context->set("radius", radius);

	context->set("pos", Vector3Node(x, y, z) );

	return context;
}

float Sphere::intersect(Vector3f ro, Vector3f rd)
{
	Vector3f d = ro - Vector3f(x, y, z);

	float b = dot(rd, d);

	float c = dot(d, d) - radius*radius;

	float t = b*b - c;

	if (t > 0)
	{
		t = - b -sqrt(t);
		
		return t;
	}

	return -1;
}

#pragma endregion

#pragma region Plane

Plane::Plane(std::string n, float xpos, float ypos, float zpos, float wpos, float reflect, bool u, bool recieve)
{
	name = n;
	x = xpos;
	y = ypos;
	z = zpos;
	w = wpos;
	uniform = u;

	reflectivity = reflect;

	type = "plane";

	castshadows = false;
	recieveshadows = recieve;
}

Node* Plane::getContext()
{
	Node* context = Object::getContext();

	context->set("reflectivity", reflectivity);

	context->set("x", x);
	context->set("y", y);
	context->set("z", z);
	context->set("w", w);

	return context;
}

float Plane::intersect(Vector3f ro, Vector3f rd)
{
	float dot1 = x*ro.x + y*ro.y + z*ro.z;
	float dot2 = x*rd.x + y*rd.y + z*rd.z;

	float t = -(dot1 + w)/dot2;

	if(t > 0.0)
		return t;
	else
		return -1;
}


#pragma endregion

#pragma region SpherePortal

SpherePortal::SpherePortal(std::string n, float r, float xpos1, float ypos1, float zpos1, float xpos2, float ypos2, float zpos2, bool u)
{
	name = n;
	radius = r;

	x1 = xpos1;
	y1 = ypos1;
	z1 = zpos1;

	x2 = xpos2;
	y2 = ypos2;
	z2 = zpos2;

	uniform = u;

	type = "sphereportal";

	cameraInside1 = false;
	cameraInside2 = false;
}

Node* SpherePortal::getContext()
{
	Node* context = Object::getContext();

	context->set("radius", radius);

	context->set("1", Vector3Node(x1, y1, z1) );
	context->set("2", Vector3Node(x2, y2, z2) );

	return context;
}

#pragma endregion


#pragma region SphereAberratopm

SphereAberration::SphereAberration(std::string n, float r, float xpos, float ypos, float zpos, float sx, float sy, float sz, bool u)
{
	name = n;
	radius = r;

	x = xpos;
	y = ypos;
	z = zpos;

	scalex = sx;
	scaley = sy;
	scalez = sz;

	uniform = u;

	type = "sphereaberration";

	cameraInside = false;
}

Node* SphereAberration::getContext()
{
	Node* context = Object::getContext();

	context->set("radius", radius);

	context->set("pos", Vector3Node(x, y, z) );
	context->set("scale", Vector3Node(scalex, scaley, scalez) );

	return context;
}

void SphereAberration::setScale( float x, float y, float z )
{
	scalex = x;
	scaley = y;
	scalez = z;

	std::string param = this->name + "_scale";

	shader->setParameter( param, Vector3f(scalex, scaley, scalez) );
}

void SphereAberration::setPos( float xPos, float yPos, float zPos )
{
	x = xPos;
	y = yPos;
	z = zPos;

	std::string param = this->name;

	shader->setParameter( param, x, y, z, radius );
}

Vector3f SphereAberration::getPos()
{
	return Vector3f(x, y, z);
}

#pragma endregion

#pragma region Box

Box::Box(std::string n, float xpos1, float ypos1, float zpos1, float xpos2, float ypos2, float zpos2, bool u, bool cast, bool recieve)
{
	name = n;

	x1 = xpos1;
	y1 = ypos1;
	z1 = zpos1;

	x2 = xpos2;
	y2 = ypos2;
	z2 = zpos2;

	uniform = u;

	type = "box";

	castshadows = cast;
	recieveshadows = recieve;
}

Node* Box::getContext()
{
	Node* context = Object::getContext();

	context->set("min", Vector3Node(x1, y1, z1) );
	context->set("max", Vector3Node(x2, y2, z2) );

	return context;
}

float Box::intersect(Vector3f ro, Vector3f rd)
{
	if (x1 < ro.x && y1 < ro.y && z1 < ro.z)
	{
		if( x2 > ro.x && y2 > ro.y && z2 > ro.z)
		{
			return -1;
		}
	}

	float tmin = ( x1 - ro.x ) / rd.x;
	float tmax = ( x2 - ro.x ) / rd.x;

	if( tmin > tmax )
	{
		float temp = tmin;
		tmin = tmax;
		tmax = temp;
	}

	float tymin = ( y1 - ro.y ) / rd.y;
	float tymax = ( y2 - ro.y ) / rd.y;

	if(tymin > tymax)
	{
		float temp = tymin;
		tymin = tymax;
		tymax = temp;
	}

	if( tmin > tymax || tymin > tmax )
		return -1;

	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	float tzmin = ( z1 - ro.z ) / rd.z;
	float tzmax = ( z2 - ro.z ) / rd.z;

	if(tzmin > tzmax)
	{
		float temp = tzmin;
		tzmin = tzmax;
		tzmax = temp;
	}

	if ( tmin > tzmax || tzmin > tmax )
		return -1;

	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	if ( tmax < 0.0 )
		return -1;

	float t = tmin;

	return t;
}


#pragma endregion

#pragma region BoxPortal

BoxPortal::BoxPortal(const char* n,  Vector3f min_1, Vector3f min_2, Vector3f s, bool u)
{
	name = n;

	min1 = min_1;


	min2 = min_2;

	size = s;

	uniform = u;

	type = "boxportal";

	cameraInside = false;
}

Node* BoxPortal::getContext()
{
	Node* context = Object::getContext();

	context->set("min1", min1 );
	context->set("min2", min2 );

	context->set("size", size );

	return context;
}


#pragma endregion

#pragma region BoxAberration

BoxAberration::BoxAberration(std::string n, Vector3f minimum, Vector3f maximum, Vector3f s, bool u)
{
	name = n;
	
	min = minimum;
	max = maximum;
	scale = s;

	uniform = u;

	type = "boxaberration";

	cameraInside = false;
}

Node* BoxAberration::getContext()
{
	Node* context = Object::getContext();

	context->set("min", min );
	context->set("max", max );

	context->set("scale", scale );

	return context;
}

bool BoxAberration::isInside(Vector3f point)
{
	if( point.x > min.x && point.y > min.y && point.z > min.z )
	{
		if( point.x < max.x && point.y < max.y && point.z < max.z )
		{
			return true;
		}
	}

	return false;
}
void BoxAberration::setScale( float x, float y, float z )
{
	scale.x = x;
	scale.y = y;
	scale.z = z;

	std::string param = this->name + "_scale";

	shader->setParameter( param, scale );
}

void BoxAberration::setMin( float x, float y, float z )
{
	min.x = x;
	min.y = y;
	min.z = z;

	std::string param = this->name + "_min";

	shader->setParameter( param, min );
}

void BoxAberration::setMax( float x, float y, float z )
{
	max.x = x;
	max.y = y;
	max.z = z;

	std::string param = this->name + "_max";

	shader->setParameter( param, max );
}

Vector3f BoxAberration::getMin()
{
	return min;
}

Vector3f BoxAberration::getMax()
{
	return max;
}

#pragma endregion

#pragma region Point

Point::Point(std::string n, float xpos, float ypos, float zpos)
{
	name = n;
	x = xpos;
	y = ypos;
	z = zpos;

	uniform = false;

	type = "point";

	castshadows = false;
	recieveshadows = false;
}

Node* Point::getContext()
{
	Node* context = Object::getContext();

	context->set("pos", Vector3Node(x, y, z) );

	return context;
}

Vector3f Point::getPos()
{
	return Vector3f(x, y, z);
}

#pragma endregion