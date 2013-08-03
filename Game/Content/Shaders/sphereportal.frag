if (iSphere( {{NAME}}_1 , ro, rd, tm, t))
{
	vec3 pos = ro + t*rd;
	
	co = vec3(1.0);
	
	vec3 nor = nSphere(pos, {{NAME}}_1 );

	float dif = clamp(dot(nor, light), 0.0, 1.0);
	float ao = 0.5 + 0.5*nor.y; 
	co = vec3(0.6, 0.8, 0.9)*dif*ao + (0.1, 0.2, 0.3)*ao;
	tm = t;

	isNewRay = true;
	
	newRay = rd;
	newPos = (pos - {{NAME}}_1.xyz)*0.99 + {{NAME}}_2.xyz;
	
	reflect  = 1.0;
}

if (iSphere( {{NAME}}_2 , ro, rd, tm, t))
{
	vec3 pos = ro + t*rd;
	vec3 nor = nSphere(pos, {{NAME}}_1 );

	float dif = clamp(dot(nor, light), 0.0, 1.0);
	float ao = 0.5 + 0.5*nor.y; 
	co = vec3(0.6, 0.8, 0.9)*dif*ao + (0.1, 0.2, 0.3)*ao;
	tm = t;

	isNewRay = true;
	
	newRay = rd;
	newPos = (pos - {{NAME}}_2.xyz)*0.99 + {{NAME}}_1.xyz;
	
	reflect  = 1.0;

}