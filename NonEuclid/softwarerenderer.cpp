#include "level.h"

#include <boost/foreach.hpp>

void Level::softRender()
{
	int width = softwareTexture.getSize().x;
	int height = softwareTexture.getSize().y;

	sf::Uint8* pixels = new sf::Uint8[width * height * 4];

	Vector3f light = normalize( Vector3f(1.0, 1.0, 1.0) );

	for(int x = 0; x < width; ++x)
	{
		for(int y = 0; y < height; ++y)
		{

			//Calculate ray
			Vector3f C = normalize( look - pos);
			Vector3f A = cross(C, up);
			Vector3f B = cross(A, C);
			Vector3f M = (pos + C);

			//Calculate screen coordinages
			Vector2f vPos = Vector2f( Vector2f( (float)x/width, (float)y/height ) * 2.0f - Vector2f(1.0, 1.0) ); 

			float aspect = (float)width/height;

			Vector3f P = M + vPos.x*A*aspect + vPos.y*B;
			Vector3f rayDir=normalize(P-pos);

			sf::Color co(0, 0, 0, 255);

			

			//Expose all level objects to the script
			std::pair<const char*, Object*> kv; 
			BOOST_FOREACH(kv, objects)
			{
				if( kv.second->type == "Plane" )
				{
					co.r = 255;
				}
			}

			int position = (width * y + x)*4;
			pixels[ position ] = co.r;
			pixels[ position + 1 ] = co.g;
			pixels[ position + 2] = co.b;
			pixels[ position + 3] = co.a;
		}
	}

	softwareTexture.update( pixels );
}