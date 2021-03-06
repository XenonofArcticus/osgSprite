#version 120 
#extension GL_EXT_gpu_shader4 : enable
#extension GL_EXT_geometry_shader4 : enable

uniform float osg_SimulationTime;

uniform int osgSprites_palette_rows;
uniform int osgSprites_palette_cols;

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
		float swayRate = cc[3];
        
        float s0 = (index%osgSprites_palette_cols) * sdiv;
        float s1 = (index%osgSprites_palette_cols) * sdiv + sdiv;
        float t0 = (index/osgSprites_palette_cols) * tdiv;
        float t1 = (index/osgSprites_palette_cols) * tdiv + tdiv;

		
		vec4 p = gl_PositionIn[i];
		vec4 u = gl_TexCoordIn[i][1]; //up
		vec4 l = vec4(cross(u.xyz, vec3(0.0,0.0,1.0)).xyz, 0.0);
		vec4 d = l * (sin(osg_SimulationTime*swayRate) - 0.5) * 1.0;
		
		vec4 p0 = p + (l * (w*0.5)); //bottom left
		vec4 p1 = p + (l * -(w*0.5)); //bottom right
		vec4 p2 = p + (l * (w*0.5)) + (u * h) + d; //top left
		vec4 p3 = p + (l * -(w*0.5)) + (u * h) + d; //top right
		
        gl_TexCoord[0] =  vec4( s0, t0, 0.0, 1.0 );
        gl_Position = gl_ProjectionMatrix * p0;
        EmitVertex();

        gl_TexCoord[0] =  vec4( s1, t0, 0.0, 1.0 );
        gl_Position = gl_ProjectionMatrix * p1;
        EmitVertex();

        gl_TexCoord[0] =  vec4( s0, t1, 0.0, 1.0 );
        gl_Position = gl_ProjectionMatrix * p2;
        EmitVertex();

        gl_TexCoord[0] =  vec4( s1, t1, 0.0, 1.0 );
        gl_Position = gl_ProjectionMatrix * p3;
		
        EmitVertex();
    }
    EndPrimitive();
}
