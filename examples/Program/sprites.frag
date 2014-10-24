uniform sampler2D tex0;

void main()
{
    vec4 rgba = texture2D( tex0, gl_TexCoord[0].st );
    if( rgba.a <= 0.2 ) //need way to set this tolerance was .58 which resulted in nothing passing
        discard;
    else
        gl_FragColor = rgba;
        
    //gl_FragColor = vec4( 1, 1, 1, 1 );
	
}


