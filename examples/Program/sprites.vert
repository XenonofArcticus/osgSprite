void main()
{   
    //vec3 nv = gl_NormalMatrix * vec3( 0, 1, 0 );

    gl_FrontColor = gl_Color;
    //gl_FrontColor[3] = nv[1];
    gl_Position = ftransform();
}


