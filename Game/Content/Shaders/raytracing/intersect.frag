bool iBox2(in vec3 min, in vec3 max, in vec3 ro, in vec3 rd, in float tm, out float t)
{
	if(!(min.x < ro.x && min.y < ro.y && min.z < ro.z && max.x > ro.x && max.y > ro.y && max.z > ro.z))
	{
		return false;
	}
	
	vec3 center = (min + max)/2;
	
	vec3 newrd = rd;
	
	vec3 newro = ro;
	
	
	float tmin = ( min.x - newro.x ) / newrd.x;
	float tmax = ( max.x - newro.x ) / newrd.x;
	
	if(tmin > tmax)
	{
		float temp = tmin;
		tmin = tmax;
		tmax = temp;
	}
	
	float tymin = ( min.y - newro.y ) / newrd.y;
	float tymax = ( max.y - newro.y ) / newrd.y;
	
	if(tymin > tymax)
	{
		float temp = tymin;
		tymin = tymax;
		tymax = temp;
	}
	
	if ((tmin > tymax) || (tymin > tmax))
		return false;
		
	if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;
		
	float tzmin = ( min.z - newro.z ) / newrd.z;
	float tzmax = ( max.z - newro.z ) / newrd.z;
	
	if(tzmin > tzmax)
	{
		float temp = tzmin;
		tzmin = tzmax;
		tzmax = temp;
	}
	
	if ((tmin > tzmax) || (tzmin > tmax))
        return false;
		
	if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;
		
	if ((tmax > tm) || (tmax < 0.0)) return false;
	
	t = tmax;
	
	return true;
}

bool iBox(in vec3 min, in vec3 max, in vec3 ro, in vec3 rd, in float tm, out float t)
{
	if(min.x < ro.x && min.y < ro.y && min.z < ro.z)
	{
		if(max.x > ro.x && max.y > ro.y && max.z > ro.z)
		{
			return false;
		}
	}
	
	float tmin = ( min.x - ro.x ) / rd.x;
	float tmax = ( max.x - ro.x ) / rd.x;
	
	if(tmin > tmax)
	{
		float temp = tmin;
		tmin = tmax;
		tmax = temp;
	}
	
	float tymin = ( min.y - ro.y ) / rd.y;
	float tymax = ( max.y - ro.y ) / rd.y;
	
	if(tymin > tymax)
	{
		float temp = tymin;
		tymin = tymax;
		tymax = temp;
	}
	
	if ((tmin > tymax) || (tymin > tmax))
		return false;
		
	if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;
		
	float tzmin = ( min.z - ro.z ) / rd.z;
	float tzmax = ( max.z - ro.z ) / rd.z;
	
	if(tzmin > tzmax)
	{
		float temp = tzmin;
		tzmin = tzmax;
		tzmax = temp;
	}
	
	if ((tmin > tzmax) || (tzmin > tmax))
        return false;
		
	if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;
		
	if ((tmin > tm) || (tmax < 0.0)) return false;
	
	t = tmin;
	
	return true;
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

bool iSphere2(in vec4 sp, in vec3 ro, in vec3 rd, in float tm, out float t)
{
	vec3 d = ro - sp.xyz;
	float b = dot(rd, d);
	float c = dot(d, d) - sp.w*sp.w;
	t = b*b - c;
	if (t > 0.0)
	{
		t = - b +sqrt(t);
		return (t  > 0.0) && (t < tm);
	}
	return false;
}

bool iPlane(in vec4 pl, in vec3 ro, in vec3 rd, in float tm, out float t)
{
	t = -(dot(pl.xyz,ro)+pl.w)/dot(pl.xyz,rd);
	return (t > 0.0) && (t < tm);
}