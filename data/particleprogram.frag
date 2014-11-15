
varying vec4 dataoutput0;
varying vec4 dataoutput1;

void main()
{
    gl_FragData[0] = dataoutput0;
	gl_FragData[1] = dataoutput1;
}


