#include <osgSprites/TexturePalette.h>

namespace osgSprites {

TexturePalette::TexturePalette( ) 
{
	this->setResizeNonPowerOfTwoHint(false);
}

TexturePalette::TexturePalette( 
    unsigned int rows,
    unsigned int cols,
    osg::Image *image ):
    osg::Texture2D( image ),
    _numRows( rows ),
    _numCols( cols )
{
	this->setResizeNonPowerOfTwoHint(false);
}

void TexturePalette::setNumRows( unsigned int nr )
{
    _numRows = nr;
}

unsigned int TexturePalette::getNumRows() const
{
    return _numRows;
}

void TexturePalette::setNumCols( unsigned int nc )
{
    _numCols = nc;
}

unsigned int TexturePalette::getNumCols() const
{
    return _numCols;
}

}
