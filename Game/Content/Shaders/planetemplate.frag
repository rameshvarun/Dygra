if (iPlane( {{NAME}} , ro, rd, tm, t))
{
	vec3 pos = ro + t*rd;

	co = vec3(0.7 - 0.02*sin(pos.x*0.2) + 0.02*sin(pos.x*0.6) - 0.02*cos(pos.z*0.2)) ;
	

	//Shadows
	float rm = 10000.0;
	float r;
	
	{{SHADOWCODE}}
	
	tm = t;
	
	//Reflect viewing ray across a sphere
	
	if({{NAME}}reflect > 0.01)
	{
		isNewRay = true;
		newRay = rd;
		newRay.y = -rd.y;
		
		newPos = pos;
		
		reflect  = {{NAME}}reflect;
	}
	else
	{
		isNewRay = false;
	}
}