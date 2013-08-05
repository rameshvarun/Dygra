{% for object in objects %}
	{% if object.type == "sphere" %}
		if( length( {object.name}.xyz - currentPos ) < {object.name}.w )
		{
			co = vec3(1.0);
			break;
		}
	{% endif %}
	{% if object.type == "box" %}
		if(
		all( greaterThan( currentPos, {{object.name}}_min ) ) &&
		all( lessThan( currentPos, {{object.name}}_max ) )
		)
		{
			co = vec3(1.0);
			break;
		}
	{% endif %}
{% endfor %}