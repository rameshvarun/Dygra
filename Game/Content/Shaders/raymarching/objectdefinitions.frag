
{% for object in objects %}

{% if object.type == "sphere" %}
	{{object.uniform}} vec4 {{object.name}} = vec4( {{object.pos.x}}, {{object.pos.y}}, {{object.pos.z}}, {{object.radius}} );
	float {{object.name}}reflect = {{object.reflectivity}};
{% endif %}

{% if object.type == "plane" %}
	{{object.uniform}} vec4 {{object.name}} = vec4( {{object.x}}, {{object.y}}, {{object.z}}, {{object.w}} );
	float {{object.name}}reflect = {{object.reflectivity}};
{% endif %}

{% if object.type == "sphereportal" %}
	{{object.uniform}} vec4 {{object.name}}_1 = vec4( {{object.1.x}}, {{object.1.y}}, {{object.1.z}}, {{object.radius}} );
	{{object.uniform}} vec4 {{object.name}}_2 = vec4( {{object.2.x}}, {{object.2.y}}, {{object.2.z}}, {{object.radius}} );
{% endif %}

{% if object.type == "sphereaberration" %}
	{{object.uniform}} vec4 {{object.name}} = vec4( {{object.pos.x}}, {{object.pos.y}}, {{object.pos.z}}, {{object.radius}} );
	{{object.uniform}} vec3 {{object.name}}_scale = vec3( {{object.scale.x}}, {{object.scale.y}}, {{object.scale.z}} );
{% endif %}

{% if object.type == "box" %}
	{{object.uniform}} vec3 {{object.name}}_min = vec3( {{object.min.x}}, {{object.min.y}}, {{object.min.z}} );
	{{object.uniform}} vec3 {{object.name}}_max = vec3( {{object.max.x}}, {{object.max.y}}, {{object.max.z}} );
{% endif %}

{% if object.type == "boxportal" %}
	{{object.uniform}} vec3 {{object.name}}_min1 = vec3( {{object.min1.x}}, {{object.min1.y}}, {{object.min1.z}} );
	{{object.uniform}} vec3 {{object.name}}_min2 = vec3( {{object.min2.x}}, {{object.min2.y}}, {{object.min2.z}} );
	
	{{object.uniform}} vec3 {{object.name}}_size = vec3( {{object.size.x}}, {{object.size.y}}, {{object.size.z}} );
{% endif %}

{% if object.type == "boxaberration" %}
	{{object.uniform}} vec3 {{object.name}}_min = vec3( {{object.min.x}}, {{object.min.y}}, {{object.min.z}} );
	{{object.uniform}} vec3 {{object.name}}_max = vec3( {{object.max.x}}, {{object.max.y}}, {{object.max.z}} );
	
	{{object.uniform}} vec3 {{object.name}}_scale = vec3( {{object.scale.x}}, {{object.scale.y}}, {{object.scale.z}} );
{% endif %}

{% endfor %}