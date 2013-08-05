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
{% endfor %}