#version 120
#extension GL_EXT_gpu_shader4 : enable

uniform int palette_rows;
uniform int palette_cols;

varying vec2 texcoordOffset;
varying vec2 texcoordScale;

void main()
{
    float sdiv = 1.0/palette_cols;
    float tdiv = 1.0/palette_rows;
	texcoordScale = vec2(sdiv,tdiv);

	vec4 cc = gl_MultiTexCoord0;
	int index =  int(cc[2]);
	float w   = cc[0];
    float h   = cc[1];
        
	float s0 = (index%palette_cols) * sdiv;
	float t0 = (index/palette_cols) * tdiv;
		
	texcoordOffset = vec2(s0, t0);
    gl_Position = ftransform();
	
	//gl_PointSize = w;// / gl_Position.w;
	
	float pointSize = abs(1600.0*w / gl_Position.w);

    //gl_PointSize = max(ceil(pointSize),2);
    gl_PointSize = ceil(pointSize);
}


