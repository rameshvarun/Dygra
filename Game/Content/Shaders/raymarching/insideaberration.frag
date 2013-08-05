{% for object in objects %}

	{% if object.type == "sphereaberration" %}
		if( length( {object.name}.xyz - currentPos ) < {object.name}.w )
		{
			stepDir *= {object.name}_scale;
		}
	{% endif %}
	
	{% if object.type == "boxaberration" %}
		if(
		all( greaterThan( currentPos, {{object.name}}_min ) ) &&
		all( lessThan( currentPos, {{object.name}}_max ) )
		)
		{
			stepDir *= {object.name}_scale;
		}
	{% endif %}
	
{% endfor %}