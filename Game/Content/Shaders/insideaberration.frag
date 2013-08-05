{% for object in objects %}

	{% if object.type == "sphereaberration" %}
		if( length(E - {{object.name}}.xyz) < {{object.name}}.w)
		{
			rayDir = normalize( rayDir * {{object.name}}_scale );
		}
	{% endif %}
	
	{% if object.type == "boxaberration" %}
		if( E.x > {{object.name}}_min.x && E.y > {{object.name}}_min.y && E.z > {{object.name}}_min.z)
		{
			if( E.x < {{object.name}}_max.x && E.y < {{object.name}}_max.y && E.z < {{object.name}}_max.z)
			{
				rayDir = normalize( rayDir * {{object.name}}_scale );
			}
		}
	{% endif %}
	
{% endfor %}