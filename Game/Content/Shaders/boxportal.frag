
if (iBox( {{NAME}}_min1 , {{NAME}}_min1 + {{NAME}}_size, ro, rd, tm, t))
{
	vec3 pos = ro + t*rd;
	
	//co = vec3(0.6, 0.8, 0.9);
	
	tm = t;
	
	
	isNewRay = true;
	
	
	
	newRay = rd;
	newPos = (pos + vec3(0.01, 0.01, 0.01) - {{NAME}}_min1)*0.98 + {{NAME}}_min2.xyz;
	
	reflect  = 1.0;
}


if (iBox( {{NAME}}_min2 , {{NAME}}_min2 + {{NAME}}_size, ro, rd, tm, t))
{
	vec3 pos = ro + t*rd;
	
	//co = vec3(0.6, 0.8, 0.9);
	
	tm = t;
	
	
	isNewRay = true;
	
	newRay = rd;
	newPos = (pos + vec3(0.01, 0.01, 0.01) - {{NAME}}_min2)*0.98 + {{NAME}}_min1.xyz;
	
	reflect  = 1.0;
}
