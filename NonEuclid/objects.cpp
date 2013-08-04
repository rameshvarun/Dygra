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

		if(obj->name.compare( name ) == 0)
		{
			if(obj->type == "sphere" && obj->castshadows)
			{
				std::string shad = file_to_string( "shaders/sphereshadow.frag" );

				replaceAll(shad, "{{NAME}}", obj->name);

				shadow << shad;
			}

			if(obj->type == "box" && obj->castshadows)
			{
				std::string shad = file_to_string( "shaders/boxshadow.frag" );

				replaceAll(shad, "{{NAME}}", obj->name);

				shadow << shad;
			}
		}
	}

	return shadow.str();
}


#pragma region Object

std::string Object::GetDefinitionCode()
{
	return "";
}

std::string Object::GetIntersectCode()
{
	return "";
}

Node* Object::getContext()
{
	Node* context = new Node();

	context->set("name", name.c_str() );
	context->set("type", type );

	context->set("uniform", uniform );
	context->set("castshadows", castshadows );
	context->set("recieveshadows", recieveshadows );

	return context;
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

	Node *position = new Node();
	position->set("x", x);
	position->set("y", y);
	position->set("z", z);

	context->set("pos", position);

	return context;
}

std::string Sphere::GetDefinitionCode()
{
	std::stringstream code;

	if(uniform)
	{
		code << "uniform ";
	}

	code << "vec4 " << name << " = vec4(" <<
		x << "," << y << "," << z << "," << radius
		<< ");";

	code << "\n";

	if(uniform)
	{
		code << "uniform ";
	}

	code << "float " << name << "reflect = " << reflectivity << ";";

	code << "\n\n";

	return code.str();

}

std::string Sphere::GetIntersectCode()
{
	//Read template glsl into string
	std::string glsl  = file_to_string( "shaders/spheretemplate.frag" );

	replaceAll(glsl, "{{NAME}}", name);

	if(recieveshadows)
	{
		replaceAll(glsl, "{{SHADOWCODE}}",  ShadowCode(objects, name) );
	}
	else
	{
		replaceAll(glsl, "{{SHADOWCODE}}",  "" );
	}

	return glsl;
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

std::string Plane::GetDefinitionCode()
{
	std::stringstream code;

	if(uniform)
	{
		code << "uniform ";
	}

	code << "vec4 " << name << " = vec4(" <<
		x << "," << y << "," << z << "," << w
		<< ");";

	code << "\n";

	if(uniform)
	{
		code << "uniform ";
	}

	code << "float " << name << "reflect = " << reflectivity << ";";

	code << "\n\n";

	return code.str();

}



std::string Plane::GetIntersectCode()
{
	//Read template glsl into string
	std::string glsl = file_to_string( "shaders/planetemplate.frag" );

	replaceAll(glsl, "{{NAME}}", name);

	//Shadow code

	if(recieveshadows)
	{
		replaceAll(glsl, "{{SHADOWCODE}}",  ShadowCode(objects, name) );
	}
	else
	{
		replaceAll(glsl, "{{SHADOWCODE}}",  "" );
	}


	return glsl;
}

#pragma endregion

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

	cameraInside = false;
}

std::string SpherePortal::GetDefinitionCode()
{
	std::stringstream code;

	if(uniform)
	{
		code << "uniform ";
	}

	code << "vec4 " << name << "_1 = vec4(" <<
		x1 << "," << y1 << "," << z1 << "," << radius
		<< ");";

	code << "\n";

	if(uniform)
	{
		code << "uniform ";
	}

	code << "vec4 " << name << "_2 = vec4(" <<
		x2 << "," << y2 << "," << z2 << "," << radius
		<< ");";

	code << "\n\n";

	return code.str();

}
std::string SpherePortal::GetIntersectCode()
{
	//Read template glsl into string
	std::string glsl = file_to_string( "shaders/sphereportal.frag" );

	replaceAll(glsl, "{{NAME}}", name);

	return glsl;
}

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

std::string SphereAberration::GetDefinitionCode()
{
	std::stringstream code;

	if(uniform)
	{
		code << "uniform ";
	}

	code << "vec4 " << name << " = vec4(" <<
		x << "," << y << "," << z << "," << radius
		<< ");";

	code << "\n";

	if(uniform)
	{
		code << "uniform ";
	}

	code << "vec3 " << name << "_scale = vec3(" <<
		scalex << "," << scaley << "," << scalez << ");";

	code << "\n\n";

	return code.str();

}
std::string SphereAberration::GetIntersectCode()
{
	//Read template glsl into string
	std::string glsl = file_to_string( "shaders/sphereaberration.frag" );

	replaceAll(glsl, "{{NAME}}", name);

	return glsl;
}

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

std::string Box::GetDefinitionCode()
{
	std::stringstream code;

	if(uniform)
	{
		code << "uniform ";
	}

	code << "vec3 " << name << "_min = vec3(" <<
		x1 << "," << y1 << "," << z1 << ");";

	code << "\n";

	if(uniform)
	{
		code << "uniform ";
	}

	code << "vec3 " << name << "_max = vec3(" <<
		x2 << "," << y2 << "," << z2 << ");";

	code << "\n\n";

	return code.str();

}

std::string Box::GetIntersectCode()
{
	//Read template glsl into string
	std::string glsl = file_to_string( "shaders/box.frag" );

	replaceAll(glsl, "{{NAME}}", name);

	if(recieveshadows)
	{
		replaceAll(glsl, "{{SHADOWCODE}}",  ShadowCode(objects, name) );
	}
	else
	{
		replaceAll(glsl, "{{SHADOWCODE}}",  "" );
	}

	return glsl;
}

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


std::string BoxPortal::GetDefinitionCode()
{
	std::stringstream code;

	if(uniform)
	{
		code << "uniform ";
	}

	code << "vec3 " << name << "_min1 = vec3(" <<
		min1.x << "," << min1.y << "," << min1.z << ");";

	code << "\n";


	if(uniform)
	{
		code << "uniform ";
	}

	code << "vec3 " << name << "_min2 = vec3(" <<
		min2.x << "," << min2.y << "," << min2.z << ");";

	code << "\n";

	if(uniform)
	{
		code << "uniform ";
	}

	code << "vec3 " << name << "_size = vec3(" <<
		size.x << "," << size.y << "," << size.z << ");";


	code << "\n\n";

	return code.str();

}

std::string BoxPortal::GetIntersectCode()
{
	//Read template glsl into string
	std::string glsl = file_to_string( "shaders/boxportal.frag" );

	replaceAll(glsl, "{{NAME}}", name);

	return glsl;
}


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

std::string BoxAberration::GetDefinitionCode()
{
	std::stringstream code;

	if(uniform)
	{
		code << "uniform ";
	}

	code << "vec3 " << name << "_min = vec3(" <<
		min.x << "," << min.y << "," << min.z << ");";

	code << "\n";

	if(uniform)
	{
		code << "uniform ";
	}

	code << "vec3 " << name << "_max = vec3(" <<
		max.x << "," << max.y << "," << max.z << ");";

	code << "\n";


	if(uniform)
	{
		code << "uniform ";
	}

	code << "vec3 " << name << "_scale = vec3(" <<
		scale.x << "," << scale.y << "," << scale.z << ");";


	code << "\n\n";

	return code.str();

}

std::string BoxAberration::GetIntersectCode()
{
	//Read template glsl into string
	std::string glsl  = file_to_string( "shaders/boxaberration.frag" );

	replaceAll(glsl, "{{NAME}}", name);

	return glsl;
}