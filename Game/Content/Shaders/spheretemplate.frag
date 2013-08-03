if (iSphere( {{NAME}} , ro, rd, tm, t))
{
	vec3 pos = ro + t*rd;
	vec3 nor = nSphere(pos, {{NAME}} );

	float dif = clamp(dot(nor, light), 0.0, 1.0);
	float ao = 0.5 + 0.5*nor.y; 
	co = vec3(0.6, 0.8, 0.9)*dif*ao + (0.1, 0.2, 0.3)*ao;
	
	//Shadows
	float rm = 10000.0;
	float r;
	{{SHADOWCODE}}
	
	tm = t;

	
	if({{NAME}}reflect > 0.01)
	{
		isNewRay = true;
		
		newRay = rd - nor * 2.0 * dot(nor, rd);
		
		newPos = pos;
		
		reflect  = {{NAME}}reflect;
	}
	else
	{
		isNewRay = false;
	}

}