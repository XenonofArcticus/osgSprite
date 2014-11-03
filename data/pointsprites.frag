uniform sampler2D osgSprites_tex0;
uniform float osgSprites_clipTolerance;

varying vec2 texcoordOffset;
varying vec2 texcoordScale;

void main()
{
	vec2 scaledCoords = vec2(gl_TexCoord[0].s*texcoordScale.s, 1.0-(gl_TexCoord[0].t*texcoordScale.t));
    vec4 rgba = texture2D( osgSprites_tex0, (scaledCoords + texcoordOffset ));
    if( rgba.a <= osgSprites_clipTolerance)
        discard;
    else
        gl_FragColor = rgba;
}


