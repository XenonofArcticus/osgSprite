#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const unsigned int numTrees = 40000;

int main( int argc, char **argv )
{
    float area = sqrt((double)numTrees) * 4.0;
    for( unsigned int i = 0; i < numTrees; i++ )
    {
        float width  = 5.0f + ((float)(rand()%1000)/1000.0f) * 20.0f;
	    float height = 10.0f + ((float)(rand()%1000)/1000.0f) * 40.0f;
	    unsigned int  paletteIndex = rand() % 5;

        float x = -(area*0.5) + ((float)(rand() % 10000)/10000.0f) * area;
        float y = -(area*0.5) + ((float)(rand() % 10000)/10000.0f) * area;
        float z = 0.0f;

        /*
         * output example:
    <SpriteData  width="0.7704"  height="1.232"  paletteIndex="1">
        <Position  x="-2.9255"  y="-2.6621"  z="0"        />
    </SpriteData>
        */

        printf(
               "\n"
               "      <SpriteData width=\"%G\" height=\"%G\" paletteIndex=\"%u\">\n" 
               "        <Position x=\"%G\" y=\"%G\" z=\"%G\" />\n"
               "      </SpriteData>\n",
                width, height, paletteIndex, x, y, z );

    }
}
