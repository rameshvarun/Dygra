

#ifdef GL_ES
precision highp float;
#endif

uniform vec2 resolution;

uniform vec3 cameraPos;
uniform vec3 cameraUp;
uniform vec3 cameraLook;


vec4 pl1 = vec4(0.0, 1.0, 0.0, 0.0);

vec4 sp1 = vec4(3.0,1.0,0.0,1.0);
vec4 sp2 = vec4(0.0,1.0,0.0,1.0);

vec3 cor1 = vec3(0.0,0.0,0.0);
vec3 cor2 = vec3(1.0,1.0,1.0);



bool iBox(in vec3 v1, in vec3 v2, in vec3 ro, in vec3 rd, in float tm, out float t)
{
	//return false;
	float xd = rd.x;
	float xo = ro.x;
	
	float xl = v1.x+v2.x;
	float xh = v1.x;
	
	float t1 = (xl - xo)/xd;
	float t2 = (xh - xo)/xd;
	
	if(t1 > t2)
	{
		float temp = t1;
		t1 = t2;
		t2 = temp;
	}
	//if(t2 >)
	
	return (t  > 0.0) && (t < tm);
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

vec3 nSphere(in vec3 pos, in vec4 sp)
{
	return (pos-sp.xyz)/sp.w;
}

bool iPlane(in vec4 pl, in vec3 ro, in vec3 rd, in float tm, out float t)
{
	t = -(dot(pl.xyz,ro)+pl.w)/dot(pl.xyz,rd);
	return (t > 0.0) && (t < tm);
}





//Actual intersection function - check for ray collision with all objects in scene
float intersect(in vec3 ro, in vec3 rd, out vec3 co , in vec3 light, in int depth, out bool isNewRay, out vec3 newRay, out vec3 newPos )
{
	isNewRay = false; //Use this to tell to the main function to iterate again

	float tm = 10000.0;
	float t;
	
	co = vec3(0.7);
	
	
	
	
	if (iSphere(sp1, ro, rd, tm, t))
	{
		vec3 pos = ro + t*rd;
		vec3 nor = nSphere(pos, sp1);

		float dif = clamp(dot(nor, light), 0.0, 1.0);
		float ao = 0.5 + 0.5*nor.y; 
		co = vec3(0.6, 0.8, 0.9)*dif*ao + (0.1, 0.2, 0.3)*ao;
		tm = t;

		//isNewRay = true;
		
		
		//newRay = -rd - 2*nor *(-rd * nor);

		//float c1 = -dot( normalize(nor), normalize(rd) );
		//newRay = rd + (2 * nor * c1);

		//newPos = pos;
	}

	if (iSphere(sp2, ro, rd, tm, t))
	{
		vec3 pos = ro + t*rd;
		vec3 nor = nSphere(pos, sp2);

		float dif = clamp(dot(nor, light), 0.0, 1.0);
		float ao = 0.5 + 0.5*nor.y; 
		co = vec3(0.6, 0.8, 0.9)*dif*ao + (0.1, 0.2, 0.3)*ao;
		tm = t;

		//isNewRay = true;
		
	}

	if (iPlane(pl1, ro, rd, tm, t))
	{
		vec3 pos = ro + t*rd;

		co = vec3(0.7 - 0.02*sin(pos.x*0.2) + 0.02*sin(pos.x*0.6) - 0.02*cos(pos.z*0.2)) ;
		
		
		float rm = 10000.0;
		float r;
		if (iSphere(sp1, pos, light, rm, r) || iSphere(sp2, pos, light, rm, r) )
		{
			float amb = smoothstep(0.0, 1.0, r);
			co = amb*co;
		}
		tm = t;
		
		//Reflect viewing ray across a sphere

		isNewRay = true;
		newRay = rd;
		newRay.y = -rd.y;
		
		newPos = pos;
	}

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

	
	//Set Light direction
	vec3 light = normalize(vec3(1.0, 1.0, 1.0));

	//First intersection
	int depth = 2;
	float t = intersect(E, rayDir, co, light, depth, isNewRay, newRay, newPos);

	while(isNewRay && depth > 0)
	{
		vec3 co2 = vec3(0.0);
		intersect(newPos + vec3(0, 0, 0), newRay, co2, light, 0, isNewRay, newRay, newPos);
		co = mix(co,co2,0.1);
	}
	
	vec3 pos = E + t*rayDir;
	
	gl_FragColor = vec4(sqrt(co), 1.0);

}