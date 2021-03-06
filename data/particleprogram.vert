uniform sampler2D dataChannel0;
uniform sampler2D dataChannel1;

uniform float osg_SimulationTime;

varying vec4 dataoutput0;
varying vec4 dataoutput1;

void main()
{   
	vec4 position = texture2D( dataChannel0, gl_MultiTexCoord0.st );
	vec4 velocity = texture2D( dataChannel1, gl_MultiTexCoord0.st );
    
	float timeStep = (osg_SimulationTime-position.a);
	vec3 up = vec3(0,0,1);
	float gravity = 9.8; // 9.8m/s/s on earth
	float mass = 1.0;
	
	vec3 forces = vec3(0,0,0);
	forces = forces + (-up * gravity);
	
	vec3 acceleration = forces / mass; 
	if(velocity.a >= 0.0)
	{
		velocity.xyz = (velocity.xyz + (acceleration * timeStep))*0.999;
		position.xyz = position.xyz + (velocity.xyz * timeStep);
	
		//collision
		if(position.z < 0.0){
			position.z = 0.0;
			velocity.xyz = reflect(velocity.xyz, up)*0.8;
		}
		
		//check if lifetime has passed
		if(velocity.a > gl_MultiTexCoord0.w){
			position.z = 40.0;
			velocity = vec4(0,0,0,0);
			velocity.a = 0.0;
		}
	
	}
	
	dataoutput0 = position;
	dataoutput0.a = osg_SimulationTime; //stash last frame time in position alpha
	dataoutput1 = velocity;
	dataoutput1.a = velocity.a + timeStep; //stash life time of the particle in velocity alpha
	
	gl_Position = gl_Vertex;
}


