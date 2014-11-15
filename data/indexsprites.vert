uniform sampler2D osgSprites_tex0;

varying vec4 dataoutput;

void main()
{   
	dataoutput = texture2D( osgSprites_tex0, gl_MultiTexCoord0.st );
	gl_Position = gl_Vertex;
}


