{% for object in objects %}
	
	//Sphere Shape
	{% if object.type == "sphere" %}
		if (iSphere( {{object.name}} , ro, rd, tm, t))
		{
			vec3 pos = ro + t*rd;
			vec3 nor = nSphere(pos, {{object.name}} );

			float dif = clamp(dot(nor, light), 0.0, 1.0);
			float ao = 0.5 + 0.5*nor.y; 
			co = vec3(0.6, 0.8, 0.9)*dif*ao + (0.1, 0.2, 0.3)*ao;
			
			//Shadows
			float rm = 10000.0;
			float r;
			
			{{SHADOWCODE}} //Does not work, due to lack of nested if's
			
			tm = t;
			
			if( {{object.name}}reflect > 0.01 )
			{
				isNewRay = true;
				newRay = rd - nor * 2.0 * dot(nor, rd);
				newPos = pos;
				reflect  = {{object.name}}reflect;
			}
			else
			{
				isNewRay = false;
			}
		}
	{% endif %}
	
	//Plane
	{% if object.type == "plane" %}
		if (iPlane( {{object.name}} , ro, rd, tm, t))
		{
			vec3 pos = ro + t*rd;

			co = vec3(0.7 - 0.02*sin(pos.x*0.2) + 0.02*sin(pos.x*0.6) - 0.02*cos(pos.z*0.2)) ;

			//Shadows
			float rm = 10000.0;
			float r;
			
			//Does not work, due to lack of nested if's
			{{object.shadowcode}}
			
			tm = t;
			
			//Reflect viewing ray across a sphere
			
			if( {{object.name}}reflect > 0.01)
			{
				isNewRay = true;
				newRay = rd;
				newRay.y = -rd.y;
				
				newPos = pos;
				
				reflect  = {{object.name}}reflect;
			}
			else
			{
				isNewRay = false;
			}
		}
	{% endif %}
	
	{% if object.type == "sphereportal" %}
		if (iSphere2( {{object.name}}_1 , ro, rd, tm, t))
		{
			vec3 pos = ro + t*rd;
			
			co = vec3(1.0);
			
			vec3 nor = nSphere(pos, {{object.name}}_1 );

			float dif = clamp(dot(nor, light), 0.0, 1.0);
			float ao = 0.5 + 0.5*nor.y; 
			co = vec3(0.6, 0.8, 0.9)*dif*ao + (0.1, 0.2, 0.3)*ao;
			tm = t;

			isNewRay = true;
			
			newRay = rd;
			newPos = (pos - {{object.name}}_1.xyz)*1.001 + {{object.name}}_2.xyz;
			
			reflect  = 1.0;
		}

		if (iSphere2( {{object.name}}_2 , ro, rd, tm, t))
		{
			vec3 pos = ro + t*rd;
			vec3 nor = nSphere(pos, {{object.name}}_1 );

			float dif = clamp(dot(nor, light), 0.0, 1.0);
			float ao = 0.5 + 0.5*nor.y; 
			co = vec3(0.6, 0.8, 0.9)*dif*ao + (0.1, 0.2, 0.3)*ao;
			tm = t;

			isNewRay = true;
			
			newRay = rd;
			newPos = (pos - {{object.name}}_2.xyz)*1.001 + {{object.name}}_1.xyz;
			
			reflect  = 1.0;
		}
	{% endif %}

	{% if object.type == "sphereaberration" %}
		if (iSphere( {{object.name}} , ro, rd, tm, t))
		{
			vec3 pos = ro + t*rd;
			vec3 nor = nSphere(pos, {{object.name}} );

			float dif = clamp(dot(nor, light), 0.0, 1.0);
			float ao = 0.5 + 0.5*nor.y; 
			//co = vec3(0.6, 0.8, 0.9)*dif*ao + (0.1, 0.2, 0.3)*ao;
			tm = t;
			
			isNewRay = true;
			newRay = normalize( rd * {{object.name}}_scale );
			
			newPos = (pos - {{object.name}}.xyz)*0.99 + {{object.name}}.xyz;
			
			reflect = 1.0;
		}
		
		if (iSphere2( {{object.name}} , ro, rd, tm, t))
		{
			vec3 pos = ro + t*rd;
			vec3 nor = nSphere(pos, {{object.name}} );

			float dif = clamp(dot(nor, light), 0.0, 1.0);
			float ao = 0.5 + 0.5*nor.y; 
			//co = vec3(0.6, 0.8, 0.9)*dif*ao + (0.1, 0.2, 0.3)*ao;
			tm = t;

			isNewRay = true;
			newRay = normalize( rd / {{object.name}}_scale );
			
			newPos = (pos - {{object.name}}.xyz)*1.01 + {{object.name}}.xyz;;
			
			reflect = 1.0;
		}
	{% endif %}

	{% if object.type == "box" %}
		if (iBox( {{object.name}}_min , {{object.name}}_max, ro, rd, tm, t))
		{
			vec3 pos = ro + t*rd;
			vec3 nor = nBox(pos, {{object.name}}_min, {{object.name}}_max);
			
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
	{% endif %}

	{% if object.type == "boxportal" %}
		if (iBox( {{object.name}}_min1 , {{object.name}}_min1 + {{object.name}}_size, ro, rd, tm, t))
		{
			vec3 pos = ro + t*rd;

			tm = t;
			
			isNewRay = true;

			newRay = rd;
			newPos = (pos + vec3(0.01, 0.01, 0.01) - {{object.name}}_min1)*0.98 + {{object.name}}_min2.xyz;
			
			reflect  = 1.0;
		}

		if (iBox( {{object.name}}_min2 , {{object.name}}_min2 + {{object.name}}_size, ro, rd, tm, t))
		{
			vec3 pos = ro + t*rd;

			tm = t;

			isNewRay = true;
			
			newRay = rd;
			newPos = (pos + vec3(0.01, 0.01, 0.01) - {{object.name}}_min2)*0.98 + {{object.name}}_min1.xyz;
			
			reflect  = 1.0;
		}
	{% endif %}

	{% if object.type == "boxaberration" %}
		if (iBox( {{object.name}}_min , {{object.name}}_max, ro, rd, tm, t))
		{
			vec3 pos = ro + t*rd;
			vec3 nor = nBox(pos, {{object.name}}_min, {{object.name}}_max);
			
			float dif = clamp(dot(nor, light), 0.0, 1.0);
			float ao = 0.5 + 0.5*nor.y; 
			
			co = vec3(0.6, 0.8, 0.9)*dif*ao + (0.1, 0.2, 0.3)*ao;
			
			tm = t;
			
			isNewRay = true;
			newRay = normalize( rd * {{object.name}}_scale );
			
			vec3 center = ( {{object.name}}_min + {{object.name}}_max )/2;
			newPos = (pos - center)*0.99 + center;
			
			reflect = 1.0;
		}

		if (iBox2( {{object.name}}_min , {{object.name}}_max, ro, rd, tm, t))
		{
			vec3 pos = ro + t*rd;
			vec3 nor = nBox(pos, {{object.name}}_min, {{object.name}}_max);
			
			float dif = clamp(dot(nor, light), 0.0, 1.0);
			float ao = 0.5 + 0.5*nor.y; 
			
			co = vec3(0.6, 0.8, 0.9)*dif*ao + (0.1, 0.2, 0.3)*ao;
			
			tm = t;
			
			isNewRay = true;
			newRay = normalize( rd / {{object.name}}_scale );
			
			vec3 center = ( {{object.name}}_min + {{object.name}}_max )/2;
			newPos = (pos - center)*1.01 + center;
			
			reflect = 1.0;
		}
	{% endif %}
	
{% endfor %}