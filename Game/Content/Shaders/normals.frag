
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