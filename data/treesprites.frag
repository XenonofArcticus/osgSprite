uniform sampler2D osgSprites_tex0;
uniform float osgSprites_clipTolerance;

void main()
{
    vec4 rgba = texture2D( osgSprites_tex0, gl_TexCoord[0].st );
    if( rgba.a <= osgSprites_clipTolerance )
        discard;
    else
        gl_FragColor = rgba;	
}


