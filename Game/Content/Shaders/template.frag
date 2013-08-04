

#ifdef GL_ES
precision highp float;
#endif

uniform vec2 resolution;

//Camera information
uniform vec3 cameraPos;
uniform vec3 cameraUp;
uniform vec3 cameraLook;

//Object Definitions
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


//Functions that test for intersections with a ray
{% include intersect.frag %}

//Functions that can give the normal of a shape
{% include normals.frag %}


//Actual intersection function - check for ray collision with all objects in scene
float intersect(in vec3 ro, in vec3 rd, out vec3 co , in vec3 light, in int depth, out bool isNewRay, out vec3 newRay, out vec3 newPos, out float reflect )
{
	isNewRay = false; //Use this to tell to the main function to iterate again

	float tm = 10000.0;
	float t = tm;
	
	co = vec3(0.7, 0.7, 0.7);
	
	
	{{INTERSECTCODE}}
	
	

	return tm;
	
	co = sqrt(co);
}



void main( void ) {

	//Camera information
	vec3  viewDest = cameraLook;
	vec3  E = cameraPos;
	
	vec3 U= cameraUp;
	vec3 C=normalize(viewDest-E);
	vec3 A=cross(C, U);
	vec3 B=cross(A, C);
	vec3 M=(E+C);

	//Calculate screen coordinages
	vec2 vPos=2.0*gl_FragCoord.xy/resolution.xy - 1.0; 
	vec3 P=M + vPos.x*A*resolution.x/resolution.y + vPos.y*B;
	vec3 rayDir=normalize(P-E);
	
	//Empty color
	vec3 co = vec3(0.0);

	bool isNewRay = false; //Empty value, returned by intersect function to see if a new ray should be created
	vec3 newRay  = vec3(0.0);
	vec3 newPos  = vec3(0.0);

	float reflect = 0.0;
	
	//Set Light direction
	vec3 light = normalize(vec3(1.0, 1.0, 1.0));
	
	
	{{ABERRATION}}
	

	//First intersection
	int depth = 5;
	float mixvalue = 1.0;
	float t = intersect(E, rayDir, co, light, depth, isNewRay, newRay, newPos, reflect);

	while(isNewRay && depth > 0)
	{
		depth -= 1;
		
		mixvalue *= reflect;
	
		vec3 co2 = vec3(0.0);
		intersect(newPos + vec3(0, 0, 0), newRay, co2, light, depth, isNewRay, newRay, newPos, reflect);
		co = mix(co,co2,mixvalue);
		
	}
	
	vec3 pos = E + t*rayDir;
	
	gl_FragColor = vec4(sqrt(co), 1.0);

}