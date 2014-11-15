uniform sampler2D dataChannel0;

void main()
{   
	//use the particle position texture to position our sprite
	vec4 position = vec4(texture2D( dataChannel0, gl_Vertex.st ).xyz, 1.0);
    gl_Position = gl_ModelViewProjectionMatrix * position;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}


