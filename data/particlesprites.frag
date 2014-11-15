uniform sampler2D osgSprites_tex0;
uniform float osgSprites_clipTolerance;

varying float shade;

void main()
{
	vec4 rgba = texture2D( osgSprites_tex0, gl_TexCoord[0].st );
    //if( rgba.a <= osgSprites_clipTolerance ) //need way to set this tolerance was .58 which resulted in nothing passing
    //    discard;
    //else
        gl_FragColor = vec4(rgba.rgb, rgba.a*shade);
}



