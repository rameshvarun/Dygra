

#ifdef GL_ES
precision highp float;
#endif

uniform vec2 resolution;

uniform vec3 cameraPos;
uniform vec3 cameraUp;
uniform vec3 cameraLook;


//vec4 pl1 = vec4(0.0, 1.0, 0.0, 0.0);

{{OBJECTS}}

bool iBox2(in vec3 min, in vec3 max, in vec3 ro, in vec3 rd, in float tm, out float t)
{
	if(!(min.x < ro.x && min.y < ro.y && min.z < ro.z && max.x > ro.x && max.y > ro.y && max.z > ro.z))
	{
		return false;
	}
	
	vec3 center = (min + max)/2;
	
	vec3 newrd = rd;
	
	vec3 newro = ro;
	
	
	float tmin = ( min.x - newro.x ) / newrd.x;
	float tmax = ( max.x - newro.x ) / newrd.x;
	
	if(tmin > tmax)
	{
		float temp = tmin;
		tmin = tmax;
		tmax = temp;
	}
	
	float tymin = ( min.y - newro.y ) / newrd.y;
	float tymax = ( max.y - newro.y ) / newrd.y;
	
	if(tymin > tymax)
	{
		float temp = tymin;
		tymin = tymax;
		tymax = temp;
	}
	
	if ((tmin > tymax) || (tymin > tmax))
		return false;
		
	if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;
		
	float tzmin = ( min.z - newro.z ) / newrd.z;
	float tzmax = ( max.z - newro.z ) / newrd.z;
	
	if(tzmin > tzmax)
	{
		float temp = tzmin;
		tzmin = tzmax;
		tzmax = temp;
	}
	
	if ((tmin > tzmax) || (tzmin > tmax))
        return false;
		
	if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;
		
	if ((tmax > tm) || (tmax < 0.0)) return false;
	
	t = tmax;
	
	return true;
}

bool iBox(in vec3 min, in vec3 max, in vec3 ro, in vec3 rd, in float tm, out float t)
{
	if(min.x < ro.x && min.y < ro.y && min.z < ro.z)
	{
		if(max.x > ro.x && max.y > ro.y && max.z > ro.z)
		{
			return false;
		}
	}
	
	float tmin = ( min.x - ro.x ) / rd.x;
	float tmax = ( max.x - ro.x ) / rd.x;
	
	if(tmin > tmax)
	{
		float temp = tmin;
		tmin = tmax;
		tmax = temp;
	}
	
	float tymin = ( min.y - ro.y ) / rd.y;
	float tymax = ( max.y - ro.y ) / rd.y;
	
	if(tymin > tymax)
	{
		float temp = tymin;
		tymin = tymax;
		tymax = temp;
	}
	
	if ((tmin > tymax) || (tymin > tmax))
		return false;
		
	if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;
		
	float tzmin = ( min.z - ro.z ) / rd.z;
	float tzmax = ( max.z - ro.z ) / rd.z;
	
	if(tzmin > tzmax)
	{
		float temp = tzmin;
		tzmin = tzmax;
		tzmax = temp;
	}
	
	if ((tmin > tzmax) || (tzmin > tmax))
        return false;
		
	if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;
		
	if ((tmin > tm) || (tmax < 0.0)) return false;
	
	t = tmin;
	
	return true;
}

bool iSphere(in vec4 sp, in vec3 ro, in vec3 rd, in float tm, out float t)
{
	vec3 d = ro - sp.xyz;
	float b = dot(rd, d);
	float c = dot(d, d) - sp.w*sp.w;
	t = b*b - c;
	if (t > 0.0)
	{
		t = - b -sqrt(t);
		return (t  > 0.0) && (t < tm);
	}
	return false;
}

bool iSphere2(in vec4 sp, in vec3 ro, in vec3 rd, in float tm, out float t)
{
	vec3 d = ro - sp.xyz;
	float b = dot(rd, d);
	float c = dot(d, d) - sp.w*sp.w;
	t = b*b - c;
	if (t > 0.0)
	{
		t = - b +sqrt(t);
		return (t  > 0.0) && (t < tm);
	}
	return false;
}

vec3 nSphere(in vec3 pos, in vec4 sp)
{
	return (pos-sp.xyz)/sp.w;
}

vec3 nBox(in vec3 pos, in vec3 min, in vec3 max)
{
	float edge = 0.001;
	
	if(pos.x > max.x - edge)
	{
		return vec3(1, 0, 0);
	}
	
	if(pos.x < min.x + edge)
	{
		return vec3(-1, 0, 0);
	}
	
	if(pos.y > max.y - edge)
	{
		return vec3(0, 1, 0);
	}
	
	if(pos.y < min.y + edge)
	{
		return vec3(0, -1, 0);
	}
	
	if(pos.z > max.z - edge)
	{
		return vec3(0, 0, 1);
	}
	
	if(pos.z < min.z + edge)
	{
		return vec3(0, 0, -1);
	}
	
	return vec3(0,0,0);
}

bool iPlane(in vec4 pl, in vec3 ro, in vec3 rd, in float tm, out float t)
{
	t = -(dot(pl.xyz,ro)+pl.w)/dot(pl.xyz,rd);
	return (t > 0.0) && (t < tm);
}





//Actual intersection function - check for ray collision with all objects in scene
float intersect(in vec3 ro, in vec3 rd, out vec3 co , in vec3 light, in int depth, out bool isNewRay, out vec3 newRay, out vec3 newPos, out float reflect )
{
	isNewRay = false; //Use this to tell to the main function to iterate again

	float tm = 10000.0;
	float t = tm;
	
	co = vec3(0.7, 0.7, 0.7);
	
	
	{{INTERSECTCODE}}
	
	

	return tm;
	
	co = sqrt(co);
}



void main( void ) {

	//Cmaera information
	vec3  viewDest = cameraLook;
	vec3  E = cameraPos;
	
	vec3 U= cameraUp;
	vec3 C=normalize(viewDest-E);
	vec3 A=cross(C, U);
	vec3 B=cross(A, C);
	vec3 M=(E+C);

	//Calculate screen coordinages
	vec2 vPos=2.0*gl_FragCoord.xy/resolution.xy - 1.0; 
	vec3 P=M + vPos.x*A*resolution.x/resolution.y + vPos.y*B;
	vec3 rayDir=normalize(P-E);
	
	//Empty color
	vec3 co = vec3(0.0);

	bool isNewRay = false; //Empty value, returned by intersect function to see if a new ray should be created
	vec3 newRay  = vec3(0.0);
	vec3 newPos  = vec3(0.0);

	float reflect = 0.0;
	
	//Set Light direction
	vec3 light = normalize(vec3(1.0, 1.0, 1.0));
	
	
	{{ABERRATION}}
	

	//First intersection
	int depth = 5;
	float mixvalue = 1.0;
	float t = intersect(E, rayDir, co, light, depth, isNewRay, newRay, newPos, reflect);

	while(isNewRay && depth > 0)
	{
		depth -= 1;
		
		mixvalue *= reflect;
	
		vec3 co2 = vec3(0.0);
		intersect(newPos + vec3(0, 0, 0), newRay, co2, light, depth, isNewRay, newRay, newPos, reflect);
		co = mix(co,co2,mixvalue);
		
		//co = co2;
		
		
	}
	
	vec3 pos = E + t*rayDir;
	
	gl_FragColor = vec4(sqrt(co), 1.0);

}