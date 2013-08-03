if (iBox( {{NAME}}_min , {{NAME}}_max, ro, rd, tm, t))
{
	vec3 pos = ro + t*rd;
	vec3 nor = nBox(pos, {{NAME}}_min, {{NAME}}_max);
	
	float dif = clamp(dot(nor, light), 0.0, 1.0);
	float ao = 0.5 + 0.5*nor.y; 
	
	co = vec3(0.6, 0.8, 0.9)*dif*ao + (0.1, 0.2, 0.3)*ao;
	
	//Shadows
	
	float rm = 10000.0;
	float r;
	
	{{SHADOWCODE}}
	
	tm = t;
	
	isNewRay = false;
}