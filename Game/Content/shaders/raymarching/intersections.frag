{% for object in objects %}
	{% if object.type == "sphere" %}
		if( length( {object.name}.xyz - currentPos ) < {object.name}.w )
		{
			vec3 nor = nSphere( currentPos, {object.name} );

			float dif = clamp(dot(nor, light), 0.0, 1.0);
			float ao = 0.5 + 0.5*nor.y; 
			co = vec3(0.6, 0.8, 0.9)*dif*ao + (0.1, 0.2, 0.3)*ao;
			
			break;
		}
	{% endif %}
	{% if object.type == "plane" %}
		if( dot( {{object.name}}.xyz, currentPos - vec3(0, 0, {{object.name}}.w / {{object.name}}.z )  ) < 0)
		{
			vec3 nor = {{object.name}}.xyz;

			float dif = clamp(dot(nor, light), 0.0, 1.0);
			float ao = 0.5 + 0.5*nor.y;
			co = vec3(0.6, 0.8, 0.9)*dif*ao + (0.1, 0.2, 0.3)*ao;
			
			break;
		}
	{% endif %}
	{% if object.type == "box" %}
		if(
		all( greaterThan( currentPos, {{object.name}}_min ) ) &&
		all( lessThan( currentPos, {{object.name}}_max ) )
		)
		{
			vec3 nor = nBox( currentPos, oldPos, {{object.name}}_min, {{object.name}}_max );

			float dif = clamp(dot(nor, light), 0.0, 1.0);
			float ao = 0.5 + 0.5*nor.y;
			co = vec3(0.6, 0.8, 0.9)*dif*ao + (0.1, 0.2, 0.3)*ao;
			
			break;
		}
	{% endif %}
	{% if object.type == "sphereportal" %}
		if(
		length( {object.name}_1.xyz - currentPos ) > {object.name}_1.w &&
		length( {object.name}_1.xyz - oldPos ) < {object.name}_1.w
		)
		{
			currentPos = (currentPos - {object.name}_1.xyz)*1.01 + {object.name}_2.xyz;
		}
		
		if(
		length( {object.name}_2.xyz - currentPos ) > {object.name}_2.w &&
		length( {object.name}_2.xyz - oldPos ) < {object.name}_2.w
		)
		{
			currentPos = (currentPos - {object.name}_2.xyz)*1.01 + {object.name}_1.xyz;
		}
	{% endif %}
{% endfor %}