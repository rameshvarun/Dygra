if (iBox( {{NAME}}_min , {{NAME}}_max, ro, rd, tm, t))
{
	vec3 pos = ro + t*rd;
	vec3 nor = nBox(pos, {{NAME}}_min, {{NAME}}_max);
	
	float dif = clamp(dot(nor, light), 0.0, 1.0);
	float ao = 0.5 + 0.5*nor.y; 
	
	co = vec3(0.6, 0.8, 0.9)*dif*ao + (0.1, 0.2, 0.3)*ao;
	

	
	tm = t;
	
	isNewRay = true;
	newRay = normalize( rd * {{NAME}}_scale );
	

	vec3 center = ( {{NAME}}_min + {{NAME}}_max )/2;
	newPos = (pos - center)*0.99 + center;
	
	reflect = 1.0;
}


if (iBox2( {{NAME}}_min , {{NAME}}_max, ro, rd, tm, t))
{
	vec3 pos = ro + t*rd;
	vec3 nor = nBox(pos, {{NAME}}_min, {{NAME}}_max);
	
	float dif = clamp(dot(nor, light), 0.0, 1.0);
	float ao = 0.5 + 0.5*nor.y; 
	
	co = vec3(0.6, 0.8, 0.9)*dif*ao + (0.1, 0.2, 0.3)*ao;
	

	
	tm = t;
	
	isNewRay = true;
	newRay = normalize( rd / {{NAME}}_scale );
	
	vec3 center = ( {{NAME}}_min + {{NAME}}_max )/2;
	newPos = (pos - center)*1.01 + center;
	
	reflect = 1.0;
}