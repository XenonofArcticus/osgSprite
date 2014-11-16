
uniform sampler2D dataChannel0;
uniform sampler2D dataChannel1;

//additional input mass, lifetime of particle
uniform sampler2D inputChannel0;

uniform float osg_SimulationTime;

varying vec2 uv;

void main()
{
	vec4 position = texture2D( dataChannel0, uv );
	vec4 velocity = texture2D( dataChannel1, uv );
	vec4 data = texture2D( inputChannel0, uv );
	
	float timeStep = (osg_SimulationTime-position.a);
	vec3 up = vec3(0,0,1);
	float gravity = 9.8; // 9.8m/s/s on earth
	float mass = data.r;
	
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
			velocity.xyz = reflect(velocity.xyz, up)*(0.4*(1.0-mass));
		}
		
		//check if lifetime has passed
		if(velocity.a > data.g){
			position.z = 50.0;
			velocity = vec4(0,0,0,0);
			velocity.a = 0.0;
		}
	
	}
	
	position.a = osg_SimulationTime; //stash last frame time in position alpha
	velocity.a = velocity.a + timeStep; //stash life time of the particle in velocity alpha
	
	//write out new position and velocity
    gl_FragData[0] = position;
	gl_FragData[1] = velocity;
}


