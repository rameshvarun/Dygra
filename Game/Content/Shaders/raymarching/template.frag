

#ifdef GL_ES
precision highp float;
#endif

uniform vec2 resolution;

//Camera information
uniform vec3 cameraPos;
uniform vec3 cameraUp;
uniform vec3 cameraLook;

//Define all objects
{% include objectdefinitions.frag %}

//Functions that can give the normal of a shape
{% include normals.frag %}

void main( void )
{

	//Camera information
	vec3  viewDest = cameraLook;
	vec3  E = cameraPos;
	
	vec3 U= cameraUp;
	vec3 C=normalize(viewDest-E);
	vec3 A=cross(C, U);
	vec3 B=cross(A, C);
	vec3 M=(E+C);

	//Calculate screen coordinates
	vec2 vPos=2.0*gl_FragCoord.xy/resolution.xy - 1.0; 
	vec3 P=M + vPos.x*A*resolution.x/resolution.y + vPos.y*B;
	vec3 rayDir=normalize(P-E);
	
	//Empty color
	vec3 co = vec3(0.0);
	
	//Set Light direction
	vec3 light = normalize( vec3(1.0, 1.0, 1.0) );
	
	int depth = 0;
	
	float STEP_SIZE = 0.2;
	int MAX_DEPTH = 200;
	
	vec3 currentPos = E;
	
	while( depth < MAX_DEPTH)
	{
		//Step forward
		vec3 oldPos = currentPos;
		
		vec3 stepDir = rayDir;
		
		{% include insideaberration.frag %}
		
		currentPos += stepDir*STEP_SIZE;
		
		{% include intersections.frag %}
		
		depth += 1;
	}
	
	co *= 1 - (  float(depth)/MAX_DEPTH )*(  float(depth)/MAX_DEPTH );
	
	gl_FragColor = vec4(co, 1.0);

}