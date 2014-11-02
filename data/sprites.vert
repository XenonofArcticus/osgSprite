
//attribute vec3 _spriteData;

void main()
{   
    //vec3 nv = gl_NormalMatrix * vec3( 0, 1, 0 );

    //gl_FrontColor = gl_MultiTexCoord0; //gl_Color;//vec4(_spriteData,0.0);//
	gl_TexCoord[0] = gl_MultiTexCoord0; //gl_Color;//vec4(_spriteData,0.0);//
    //gl_FrontColor[3] = nv[1];
    gl_Position = ftransform();
}


