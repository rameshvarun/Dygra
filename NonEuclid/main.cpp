#include <SFML/Graphics.hpp>
#include <vector>

#include<iostream>
#include<math.h>

using namespace sf;


#include "level.h"

#include "physfs.h"


int main()
{
	PHYSFS_init(NULL);
	PHYSFS_mount( "../content/", "", 1 );

	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML Shader");

	window.setVerticalSyncEnabled(true);

	sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(window.getSize().x,window.getSize().y));

	Level *level = new Level();

	//level->AddObject(new Sphere("sp3", -4.0,1.0,0.0,1.0, 0.2, false, true, false));
	//level->AddObject(new Sphere("sp1", 0.0,1.0,4.0,1.0, 0.2, false, true, false));
	//level->AddObject(new Sphere("sp2", 4.0,1.0,0.0,1.0, 0.2, false, true, false));
	
	//level->AddObject(new Plane("pl1", 0.0, 1.0, 0.0, 0.0, 0.0, false, true));
	
	level->LoadXML("test.xml");
	
	//level->AddObject(new SpherePortal("portal1", 2, 10.0, 2.0, 2.0, -10.0, 2.0, 2.0, false) );

	//level->AddObject(new SphereAberration("aberration1", 2.0, 0.0, 0.0, 0.0, 0.5, 1.0, 1.0, false));


	//level->AddObject(new Box("box1", 0, 0, 7, 1, 2, 10, false, true, false));
	//level->AddObject(new Box("box2", 2, 0, 7, 3, 2, 10, false, true, false));

	//level->AddObject(new Box("box3", 1, 1.5, 7, 2, 2, 10, false, true, false));

	//level->AddObject(new BoxAberration("aberration2", Vector3f(0.9,-1,7.1), Vector3f(2.1,2.0,9.9), Vector3f(1.0, 1.0, 0.2), false ));




	//level->AddObject(new Box("box4", 4, 0, 7, 5, 2, 15, false, true, false));
	//level->AddObject(new Box("box5", 6, 0, 7, 7, 2, 15, false, true, false));

	//level->AddObject(new Box("box6", 5, 1.5, 7, 6, 2, 15, false, true, false));

	//level->AddObject(new BoxAberration("aberration3", Vector3f(4.9,-1,7.1), Vector3f(6.1,2.0,14.9), Vector3f(1.0, 1.0, 5), false ));

	//level->AddObject(new BoxPortal("portal2", Vector3f(-10,0,-10), Vector3f(-10,0,15), Vector3f(20,2,0.5), false ) );


	//level->AddObject(new BoxPortal("portal3", Vector3f(-10,0,-10), Vector3f(10,0,-10), Vector3f(0.5,2,26), false ) );

	//level->AddObject(new BoxPortal("portal4", Vector3f( -0.21, 0, 8), Vector3f(7.01, 0, 10), Vector3f(0.2,1.5,1), false ) );
	
	
	//level->AddObject(new SphereAberration("aberration1", 1.5, 1.5, 0.0, 8.5, 1.0, 1.0, 0.3, false));

	

	level->BuildShader(sf::Vector2f(window.getSize().x,window.getSize().y) );


	



	sf::Vector3f pos = sf::Vector3f(0,1.0,4.0);
	sf::Vector3f up = sf::Vector3f(0,1,0);
	sf::Vector3f look = sf::Vector3f(0.0,0.0,0.0);

	float theta = 1.24f;
	float phi = 3.14f;

	Mouse::setPosition(sf::Vector2i(window.getSize().x/2,window.getSize().y/2), window);

	sf::Clock clock;
    while (window.isOpen())
	{
		sf::Event event;
        while (window.pollEvent(event))
        {
			if (event.type == sf::Event::Closed)
                window.close();

			if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                    // Escape key: exit
                    case sf::Keyboard::Escape:
                        window.close();
                        break;

                    default:
                        break;
                }
            }
		}

		//Draw
		window.clear(sf::Color(0,0,255));

		window.draw(shape, level->shader);

		window.display();

		//Update
		float dt = clock.getElapsedTime().asSeconds();
		clock.restart();



		float sensitivity = 0.1f;
		sf::Vector2i mrel = Mouse::getPosition(window) - sf::Vector2i(window.getSize().x/2,window.getSize().y/2);
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


		Mouse::setPosition(sf::Vector2i(window.getSize().x/2,window.getSize().y/2), window);

		float z = sin(theta)*cos(phi);
		float y = cos(theta);
		float x = sin(theta)*sin(phi);


		look = pos + Vector3f(x,y,z);

		
		float scalex = 1.0f;
		float scaley = 1.0f;
		float scalez = 1.0f;

		for (std::map<const char*, Object*>::iterator it=level->objects.begin(); it!=level->objects.end(); ++it)
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

		level->SetCamera(pos, up, look);

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
		
	}
}