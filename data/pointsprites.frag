uniform sampler2D tex0;

varying vec2 texcoordOffset;
varying vec2 texcoordScale;

void main()
{
	vec2 scaledCoords = vec2(gl_TexCoord[0].s*texcoordScale.s, 1.0-(gl_TexCoord[0].t*texcoordScale.t));
    vec4 rgba = texture2D( tex0, (scaledCoords + texcoordOffset ));
    if( rgba.a <= 0.2 ) //need way to set this tolerance was .58 which resulted in nothing passing
        discard;
    else
        gl_FragColor = rgba;
}


