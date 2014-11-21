#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const unsigned int numTrees = 40000;

int main( int argc, char **argv )
{
    float area = sqrt((float)numTrees) * 4.0f;
    for( unsigned int i = 0; i < numTrees; i++ )
    {
        float width  = 5.0f + ((float)(rand()%1000)/1000.0f) * 20.0f;
	    float height = 10.0f + ((float)(rand()%1000)/1000.0f) * 40.0f;
	    unsigned int  paletteIndex = rand() % 5;

        float x = -(area*0.5f) + ((float)(rand() % 10000)/10000.0f) * area;
        float y = -(area*0.5f) + ((float)(rand() % 10000)/10000.0f) * area;
        float z = 0.0f;

        printf("      %G %G %u %G %G %G\n", width, height, paletteIndex, x, y, z );

    }
}
