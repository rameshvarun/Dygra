
vec3 nSphere(in vec3 pos, in vec4 sp)
{
	return (pos-sp.xyz)/sp.w;
}

vec3 nBox(in vec3 pos, in vec3 oldpos, in vec3 min, in vec3 max)
{
	float edge = 1;
	
	if(pos.x < max.x && oldpos.x > max.x)
	{
		return vec3(1, 0, 0);
	}
	
	if(pos.x > min.x && oldpos.x < min.x)
	{
		return vec3(-1, 0, 0);
	}
	
	if(pos.y < max.y && oldpos.y > max.y)
	{
		return vec3(0, 1, 0);
	}
	
	if(pos.y > min.y && oldpos.y < min.y)
	{
		return vec3(0, -1, 0);
	}
	
	if(pos.z < max.z && oldpos.z > max.z)
	{
		return vec3(0, 0, 1);
	}
	
	if(pos.z > min.z && oldpos.z < min.z)
	{
		return vec3(0, 0, -1);
	}
	
	return vec3(0,0,0);
}