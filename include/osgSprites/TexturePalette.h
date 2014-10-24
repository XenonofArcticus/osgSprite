/* @License 
 -------------------------------------------------------------------------------
 | Chaskii - Copyright (C) 2010  Don Burns, Andes Computer Engineering, LLC    |
 |                                                                             |
 | This library is free software; you can redistribute it and/or modify        |
 | it under the terms of the GNU Lesser General Public License as published    |
 | by the Free Software Foundation; either version 3 of the License, or        |
 | (at your option) any later version.                                         |
 |                                                                             |
 | This library is distributed in the hope that it will be useful, but         |
 | WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY  |
 | or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public     |
 | License for more details.                                                   |
 |                                                                             |
 | You should have received a copy of the GNU Lesser General Public License    |
 | along with this software; if not, write to the Free Software Foundation,    |
 | Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.               |
 ---------------------------------------------------------------------------- */
#ifndef OSG_SPRITES_TEXTURE_PALETTE_DEF
#define OSG_SPRITES_TEXTURE_PALETTE_DEF 1

#include <osg/Texture2D>
#include <osgSprites/Export.h>

// This class should be expanded out to 
// build from a list of textures.  For now "hard code" 
// the rows, columns and name of the image file (made offline)

namespace osgSprites {

class OSGSPRITES_EXPORT TexturePalette: public osg::Texture2D
{
    public:
        TexturePalette();
        TexturePalette(
                unsigned int rows,
                unsigned int cols,
                osg::Image *image );

        void setNumRows( unsigned int );
        unsigned int getNumRows() const;

        void setNumCols( unsigned int);
        unsigned int getNumCols() const;

    private:
        unsigned int _numRows;
        unsigned int _numCols;

};

}

#endif


