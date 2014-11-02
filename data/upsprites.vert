
void main()
{   
    gl_TexCoord[0] = gl_MultiTexCoord0;//data
	gl_TexCoord[1] = vec4(gl_NormalMatrix * gl_MultiTexCoord1.xyz, 0.0);//up vector in eye space
    gl_Position = gl_ModelViewMatrix * gl_Vertex; //vertex in eye space
}


