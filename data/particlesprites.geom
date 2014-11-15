#version 120 
#extension GL_EXT_gpu_shader4 : enable
#extension GL_EXT_geometry_shader4 : enable


uniform int osgSprites_palette_rows;
uniform int osgSprites_palette_cols;

varying float shade;

void main(void)
{
    if( osgSprites_palette_cols == 0 || osgSprites_palette_rows == 0 )
        return;
        
    float sdiv = 1.0/osgSprites_palette_cols;
    float tdiv = 1.0/osgSprites_palette_rows;

    for( int i=0; i< gl_VerticesIn; i++)
    {
        vec4 cc = gl_TexCoordIn[i][0];
        float w   = cc[0];
        float h   = cc[1];
        int index =  int(cc[2]);
		float userData = cc[3];
        
        float s0 = (index%osgSprites_palette_cols) * sdiv;
        float s1 = (index%osgSprites_palette_cols) * sdiv + sdiv;
        float t0 = (index/osgSprites_palette_cols) * tdiv;
        float t1 = (index/osgSprites_palette_cols) * tdiv + tdiv;

        vec4 p0 = gl_PositionIn[i];
        p0.x -= w*0.5;
        vec4 p1 = vec4( p0.x + w, p0.yzw );
        vec4 p2 = vec4( p0.x, p0.y + h, p0.zw );
        vec4 p3 = vec4( p1.x, p2.y, p0.zw );

        gl_TexCoord[0] =  vec4( s0, t0, 0.0, 1.0 );
        gl_Position = p0;
		shade = userData;
        EmitVertex();

        gl_TexCoord[0] =  vec4( s1, t0, 0.0, 1.0 );
        gl_Position = p1;
		shade = userData;
        EmitVertex();

        gl_TexCoord[0] =  vec4( s0, t1, 0.0, 1.0 );
        gl_Position = p2;
		shade = userData;
        EmitVertex();

        gl_TexCoord[0] =  vec4( s1, t1, 0.0, 1.0 );
        gl_Position = p3;
		shade = userData;
        EmitVertex();
    }
    EndPrimitive();
}
