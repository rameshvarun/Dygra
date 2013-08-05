if (iSphere( {{NAME}} , ro, rd, tm, t))
{
	vec3 pos = ro + t*rd;
	vec3 nor = nSphere(pos, {{NAME}} );

	float dif = clamp(dot(nor, light), 0.0, 1.0);
	float ao = 0.5 + 0.5*nor.y; 
	//co = vec3(0.6, 0.8, 0.9)*dif*ao + (0.1, 0.2, 0.3)*ao;
	tm = t;

	
	isNewRay = true;
	newRay = normalize( rd * {{NAME}}_scale );
	
	newPos = (pos - {{NAME}}.xyz)*0.99 + {{NAME}}.xyz;;
	
	reflect = 1.0;
}


if (iSphere2( {{NAME}} , ro, rd, tm, t))
{
	vec3 pos = ro + t*rd;
	vec3 nor = nSphere(pos, {{NAME}} );

	float dif = clamp(dot(nor, light), 0.0, 1.0);
	float ao = 0.5 + 0.5*nor.y; 
	//co = vec3(0.6, 0.8, 0.9)*dif*ao + (0.1, 0.2, 0.3)*ao;
	tm = t;

	
	isNewRay = true;
	newRay = normalize( rd / {{NAME}}_scale );
	
	newPos = (pos - {{NAME}}.xyz)*1.01 + {{NAME}}.xyz;;
	
	reflect = 1.0;
}