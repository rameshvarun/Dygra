#include "level.h"
#include "tinyxml2.h"


#include "util.h"

using namespace tinyxml2;

#include "dependencies.h"

#include "template.h"

#include "graphics.h"

using namespace templating;

#include <boost/foreach.hpp>

Level::Level(std::string rendererType)
{
	renderer = rendererType;

	shader = new sf::Shader();

	pos = sf::Vector3f(0,1.0,4.0);

	script = scripting::newState();
	luabind::globals(script)["level"] = this;

	luabind::globals(script)["done"] = false;

	if( renderer.find("software") == std::string::npos )
	{
		software = false;
	}
	else
	{
		software = true;
	}
}

sf::Vector3f Level::getPos()
{
	return pos;
}

void Level::setPos(Vector3f newPos)
{
	pos = newPos;
}

bool replace(std::string& str, const std::string& from, const std::string& to)
{
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}


void Level::BuildShader()
{

	try
	{
		luabind::call_function<void>(script,"preBuildShader");
	}
	catch(...)
	{
		BOOST_LOG_TRIVIAL(error) << "Error calling lua function preBuildShader.";
	}

	if( !software )
	{

		BOOST_LOG_TRIVIAL(debug) << "Building shader...";

		Node *context = new Node();

		NodeList *objectlist = new NodeList();
		context->properties["objects"] = objectlist;


		for (std::map<const char*, Object*>::iterator it=objects.begin(); it!=objects.end(); ++it)
		{
			objectlist->nodes.push_back( (*it).second->getContext() );
		}

		std::string glsl;
		if(renderer.compare("raymarching") == 0 )
		{
			glsl = render("shaders/raymarching/template.frag", context);
		}
		if(renderer.compare("raytracing") == 0 )
		{
			glsl = render("shaders/raytracing/template.frag", context);
		}

	
	#ifdef _DEBUG
		std::ofstream out;
		out.open("output.frag");
		out << glsl;
		out.close();

		BOOST_LOG_TRIVIAL(debug) << "Output.frag written with resulting shader code.";
	#endif


		shader->loadFromMemory(glsl, sf::Shader::Fragment);
		shader->setParameter("resolution", graphics::getResolution() );

		BOOST_LOG_TRIVIAL(debug) << "Shader has been built...";
	}
}

void Level::SetCamera(sf::Vector3f pos, sf::Vector3f up, sf::Vector3f look)
{
	if( !software )
	{
		shader->setParameter("cameraPos", pos);
		shader->setParameter("cameraUp", up);
		shader->setParameter("cameraLook", look);
	}
}

void Level::AddObject(Object* newobject)
{
	objects[newobject->name.c_str()] = newobject;
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

void Level::LoadXML(const char* filename)
{
	BOOST_LOG_TRIVIAL(debug) << "Loading map " << filename;

	XMLDocument doc;
	doc.Parse( file_to_string(filename).c_str() );

	XMLElement* scene = doc.FirstChildElement("scene");

	for(const XMLElement* plane = scene->FirstChildElement("Plane"); plane; plane = plane->NextSiblingElement("Plane"))
	{
		std::string name = plane->Attribute("name");

		Plane *obj = new Plane(
			name,
			plane->FloatAttribute("x"),
			plane->FloatAttribute("y"),
			plane->FloatAttribute("z"),
			plane->FloatAttribute("c"),
			plane->FloatAttribute("reflectivity"),
			plane->BoolAttribute("uniform"),
			plane->BoolAttribute("recieve")
			);

		BOOST_LOG_TRIVIAL(debug) << "Lodaded plane " << obj->name;
		
		this->AddObject(obj);
	}
	
	for(const XMLElement* sphere = scene->FirstChildElement("Sphere"); sphere; sphere = sphere->NextSiblingElement("Sphere"))
	{
		std::string name = sphere->Attribute("name");

		Sphere *obj = new Sphere(
			name,
			sphere->FloatAttribute("x"),
			sphere->FloatAttribute("y"),
			sphere->FloatAttribute("z"),
			sphere->FloatAttribute("r"),
			sphere->FloatAttribute("reflectivity"),
			sphere->BoolAttribute("uniform"),
			sphere->BoolAttribute("cast"),
			sphere->BoolAttribute("recieve")
			);

		BOOST_LOG_TRIVIAL(debug) << "Lodaded sphere " << obj->name;
		
		this->AddObject(obj);
	}

	for(const XMLElement* box = scene->FirstChildElement("Box"); box; box = box->NextSiblingElement("Box"))
	{
		std::string name = box->Attribute("name");

		std::vector<std::string> min = split( box->Attribute("min"), ',');
		std::vector<std::string> max = split( box->Attribute("max"), ',');

		Box *obj = new Box(
			name,
			atof(min[0].c_str()),
			atof(min[1].c_str()),
			atof(min[2].c_str()),
			atof(max[0].c_str()),
			atof(max[1].c_str()),
			atof(max[2].c_str()),
			box->BoolAttribute("uniform"),
			box->BoolAttribute("cast"),
			box->BoolAttribute("recieve")
			);

		BOOST_LOG_TRIVIAL(debug) << "Lodaded box " << obj->name;

		this->AddObject(obj);
	}

	for(const XMLElement* boxab = scene->FirstChildElement("BoxAberration"); boxab; boxab = boxab->NextSiblingElement("BoxAberration"))
	{
		std::string name = boxab->Attribute("name");

		std::vector<std::string> min = split( boxab->Attribute("min"), ',');
		std::vector<std::string> max = split( boxab->Attribute("max"), ',');

		std::vector<std::string> scale = split( boxab->Attribute("scale"), ',');

		Vector3f minf = Vector3f( atof(min[0].c_str()) , atof(min[1].c_str()), atof(min[2].c_str()));
		Vector3f maxf = Vector3f( atof(max[0].c_str()) , atof(max[1].c_str()), atof(max[2].c_str()));
		Vector3f scalef = Vector3f( atof(scale[0].c_str()) , atof(scale[1].c_str()), atof(scale[2].c_str()));

		bool uniform = std::string( boxab->Attribute("uniform") ).compare("True") == 0;

		BoxAberration *obj = new BoxAberration(name, minf, maxf, scalef, uniform);

		BOOST_LOG_TRIVIAL(debug) << "Lodaded Box Aberration " << obj->name;

		this->AddObject(obj);
	}

	for(const XMLElement* sphereab = scene->FirstChildElement("SphereAberration"); sphereab; sphereab = sphereab->NextSiblingElement("SphereAberration"))
	{
		std::string name = sphereab->Attribute("name");

		std::vector<std::string> scale = split( sphereab->Attribute("scale"), ',');
		//Vector3f scalef = Vector3f(  , , );

		SphereAberration *obj = new SphereAberration(
			name,
			sphereab->FloatAttribute("r"),
			sphereab->FloatAttribute("x"),
			sphereab->FloatAttribute("y"),
			sphereab->FloatAttribute("z"),

			atof(scale[0].c_str()),
			atof(scale[1].c_str()),
			atof(scale[2].c_str()),

			sphereab->BoolAttribute("uniform")
			);
		
		BOOST_LOG_TRIVIAL(debug) << "Lodaded Sphere Aberration " << obj->name;
		
		this->AddObject(obj);
	}

	for(const XMLElement* sphereportal = scene->FirstChildElement("SpherePortal"); sphereportal; sphereportal = sphereportal->NextSiblingElement("SpherePortal"))
	{
		std::string name = sphereportal->Attribute("name");

		SpherePortal *obj = new SpherePortal(
			name,
			sphereportal->FloatAttribute("r"),

			sphereportal->FloatAttribute("x"),
			sphereportal->FloatAttribute("y"),
			sphereportal->FloatAttribute("z"),

			sphereportal->FloatAttribute("x2"),
			sphereportal->FloatAttribute("y2"),
			sphereportal->FloatAttribute("z2"),
			
			sphereportal->BoolAttribute("uniform")
			);
		
		BOOST_LOG_TRIVIAL(debug) << "Lodaded Sphere Portal " << obj->name;
		
		this->AddObject(obj);
	}

#pragma region Script

	for(const XMLElement* element = scene->FirstChildElement("script"); element; element = element->NextSiblingElement("script"))
	{
		const char* scriptString = element->GetText();
		
		if(scriptString != NULL)
		{
			BOOST_LOG_TRIVIAL(debug) << "Lodaded lua script.";

			luaL_dostring(script, scriptString);
		}
		else
		{
			BOOST_LOG_TRIVIAL(debug) << "This map does not have a lua script.";
		}
	}

#pragma endregion

#pragma region Point

	for(const XMLElement* point = scene->FirstChildElement("Point"); point; point = point->NextSiblingElement("Point"))
	{
		std::string name = point->Attribute("name");

		Point *obj = new Point(
			name,
			point->FloatAttribute("x"),
			point->FloatAttribute("y"),
			point->FloatAttribute("z")
			);

		BOOST_LOG_TRIVIAL(debug) << "Lodaded point " << obj->name;
		
		this->AddObject(obj);
	}

#pragma endregion


	BOOST_LOG_TRIVIAL(debug) << "Finished loading map " << filename;

	
}

float Level::intersect(Vector3f ro, Vector3f rd)
{
	float t = -1;

	std::pair<const char*, Object*> kv; 
	BOOST_FOREACH(kv, objects)
	{
		float result = kv.second->intersect(ro, rd);

		if(result > 0)
		{
			if(t < 0 || result < t )
			{
				t = result;
			}
		}
	}

	return t;
}

std::string Level::run()
{
	sf::RenderWindow* window = graphics::getWindow();
	sf::RectangleShape* shape = graphics::getShape();

	sf::Vector3f up = sf::Vector3f(0,1,0);
	sf::Vector3f look = sf::Vector3f(0.0,0.0,0.0);

	float theta = 1.24f;
	float phi = 3.14f;

	Mouse::setPosition(sf::Vector2i( window->getSize().x/2 , window->getSize().y/2 ), *window);

	sf::Clock clock;

	//Expose all level objects to the script
	std::pair<const char*, Object*> kv; 
	BOOST_FOREACH(kv, objects)
	{
		luabind::globals(script)[kv.first] = kv.second;

		kv.second->shader = shader;
	}

	//Call load script
	try
	{
		luabind::call_function<void>(script,"load");
	}
	catch(...)
	{
		BOOST_LOG_TRIVIAL(error) << "Error calling lua load function.";
	}

	BOOST_LOG_TRIVIAL(debug) << "Starting game loop.";

	float velY = 0;

    while (window->isOpen())
	{
		sf::Event event;
        while (window->pollEvent(event))
        {
			if (event.type == sf::Event::Closed)
                window->close();

			if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                    // Escape key: exit
                    case sf::Keyboard::Escape:
                        window->close();
                        break;

					case sf::Keyboard::F:
						graphics::toggleFullscreen();

                    default:
                        break;
                }
            }
		}

		if( luabind::globals(script)["done"] == true )
		{
			return "finish";
		}

		//Draw
		window->clear(sf::Color(0,0,255));

		if(!software)
		{
			window->draw(*shape, this->shader);
		}

		window->display();

		//Update
		float dt = clock.getElapsedTime().asSeconds();
		clock.restart();



		float sensitivity = 0.1f;
		sf::Vector2i mrel = Mouse::getPosition(*window) - sf::Vector2i(window->getSize().x/2, window->getSize().y/2);
		phi -= sensitivity*mrel.x*dt;
		theta += sensitivity*mrel.y*dt;

		if(theta < 0.6)
		{
			theta = 0.6;
		}
		if(theta > 2.6)
		{
			theta = 2.6;
		}


		Mouse::setPosition(sf::Vector2i(window->getSize().x/2,window->getSize().y/2), *window);

		float z = sin(theta)*cos(phi);
		float y = cos(theta);
		float x = sin(theta)*sin(phi);


		look = pos + Vector3f(x,y,z);

		
		float scalex = 1.0f;
		float scaley = 1.0f;
		float scalez = 1.0f;

		for (std::map<const char*, Object*>::iterator it=this->objects.begin(); it!=this->objects.end(); ++it)
		{
			Object* obj = (*it).second;

			if(obj->type == "sphereportal")
			{
				SpherePortal* portal = ((SpherePortal*)obj);

				double dist1 = sqrt(pow(pos.x - portal->x1, 2) + pow(pos.y - portal->y1, 2) + pow(pos.z - portal->z1, 2));

				double dist2 = sqrt(pow(pos.x - portal->x2, 2) + pow(pos.y - portal->y2, 2) + pow(pos.z - portal->z2, 2));

				if(dist1 < portal->radius && portal->cameraInside == false)
				{
					pos.x = (pos.x - portal->x1) + portal->x2;
					pos.y = (pos.y - portal->y1) + portal->y2;
					pos.z = (pos.z - portal->z1) + portal->z2;
				}
				if(dist2 < portal->radius && portal->cameraInside == false)
				{
					pos.x = (pos.x - portal->x2) + portal->x1;
					pos.y = (pos.y - portal->y2) + portal->y1;
					pos.z = (pos.z - portal->z2) + portal->z1;
				}

				if(dist1 < portal->radius || dist2 < portal->radius)
				{
					portal->cameraInside = true;
				}
				else
				{
					portal->cameraInside = false;
				}
				
			}

			if(obj->type == "boxportal")
			{
				BoxPortal* portal = ((BoxPortal*)obj);

				Vector3f diff1 = pos - portal->min1;
				Vector3f diff2 = pos - portal->min2;

				bool in1 = false;
				if(diff1.x > 0 && diff1.y > 0 && diff1.z > 0 && diff1.x < portal->size.x && diff1.y < portal->size.y  && diff1.z < portal->size.z)
				{
					in1 = true;
				}

				bool in2 = false;
				if(diff2.x > 0 && diff2.y > 0 && diff2.z > 0 && diff2.x < portal->size.x && diff2.y < portal->size.y  && diff2.z < portal->size.z)
				{
					in2 = true;
				}

				if(in1 && portal->cameraInside == false)
				{
					pos = (pos - portal->min1) + portal->min2;
				}
				if(in2 && portal->cameraInside == false)
				{
					pos = (pos - portal->min2) + portal->min1;
				}

				if(in1 || in2)
				{
					portal->cameraInside = true;
				}
				else
				{
					portal->cameraInside = false;
				}
				
			}

			if(obj->type == "sphereaberration")
			{
				SphereAberration* ab = ((SphereAberration*)obj);

				double dist = sqrt(pow(pos.x - ab->x, 2) + pow(pos.y - ab->y, 2) + pow(pos.z - ab->z, 2));

				if(dist < ab->radius)
				{
					scalex *= ab->scalex;
					scaley *= ab->scaley;
					scalez *= ab->scalez;
				}
			}

			if(obj->type == "boxaberration")
			{
				BoxAberration* ab = ((BoxAberration*)obj);

				if(pos.x > ab->min.x && pos.y > ab->min.y && pos.z > ab->min.z)
				{
					if(pos.x < ab->max.x && pos.y < ab->max.y && pos.z < ab->max.z)
					{
						scalex *= ab->scale.x;
						scaley *= ab->scale.y;
						scalez *= ab->scale.z;
					}
				}
			}
		}

		this->SetCamera(pos, up, look);

		float speed = 3.0f;
		if(Keyboard::isKeyPressed(Keyboard::W))
		{
			pos.x += scalex*speed*dt*sin(phi);
			pos.z += scalez*speed*dt*cos(phi);
		}
		if(Keyboard::isKeyPressed(Keyboard::S))
		{
			pos.x -= scalex*speed*dt*sin(phi);
			pos.z -= scalez*speed*dt*cos(phi);
		}
		if(Keyboard::isKeyPressed(Keyboard::D))
		{
			pos.x -= scalex*speed*dt*sin(phi + 3.14/2);
			pos.z -= scalez*speed*dt*cos(phi + 3.14/2);
		}
		if(Keyboard::isKeyPressed(Keyboard::A))
		{
			pos.x += scalex*speed*dt*sin(phi + 3.14/2);
			pos.z += scalez*speed*dt*cos(phi + 3.14/2);
		}


		//Collision detection
		float PLAYER_WIDTH = 0.5;
		float STEP_HEIGHT = 0.5;
		float PLAYER_HEIGHT = 1;

		Vector3f stepPos = pos - Vector3f(0, PLAYER_HEIGHT - STEP_HEIGHT, 0);

		//X-Axis
		float result = this->intersect(stepPos, Vector3f(1, 0, 0) );
		if(result > 0 && result < PLAYER_WIDTH*scalex)
		{
			pos.x -= (PLAYER_WIDTH*scalex - result);
		}
		result = this->intersect(stepPos, Vector3f(-1, 0, 0) );
		if(result > 0 && result < PLAYER_WIDTH*scalex)
		{
			pos.x += (PLAYER_WIDTH*scalex - result);
		}

		//Z-Axis
		result = this->intersect(stepPos, Vector3f(0, 0, 1) );
		if(result > 0 && result < PLAYER_WIDTH*scalez)
		{
			pos.z -= (PLAYER_WIDTH*scalez - result);
		}
		result = this->intersect(stepPos, Vector3f(0, 0, -1) );
		if(result > 0 && result < PLAYER_WIDTH*scalez)
		{
			pos.z += (PLAYER_WIDTH*scalez - result);
		}

		float GRAVITY = 3.0;

		//Y-Axis
		result = this->intersect(pos, Vector3f(0, -1, 0) );
		if(result > 0 && result < PLAYER_HEIGHT)
		{
			pos.y += (PLAYER_HEIGHT - result);
			velY = 0;
		}
		else
		{
			velY -= GRAVITY*dt;
			pos.y += velY*dt;
		}

		if(result > 0 && result < PLAYER_HEIGHT*1.01)
		{
			if( Keyboard::isKeyPressed(Keyboard::Space) )
			{
				velY = 2;
			}
		}

		//Call update script
		try
		{
			luabind::call_function<void>(script,"update", dt);
		}
		catch(...)
		{
			BOOST_LOG_TRIVIAL(error) << "Error calling lua update function.";
		}
		
	}

	return "exit";
}