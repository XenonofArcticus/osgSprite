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
#ifndef OSG_SPRITES_DEF
#define OSG_SPRITES_DEF 1

#include <vector>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/BoundingSphere>
#include <osgSprites/Export.h>
#include "TexturePalette.h"

namespace osgSprites {

class OSGSPRITES_EXPORT Sprites: public osg::Geode
{
    public:
        META_Object( osgSprites, Sprites);

        struct SpriteData 
        {
            float width;
            float height;
            unsigned int paletteIndex;
            osg::Vec3f position;

            SpriteData( float w, float h, unsigned int pi, const osg::Vec3f &p ):
                width(w),
                height(h),
               paletteIndex(pi),
                position(p ) 
            {}

            SpriteData() :
                width(1.0),
                height(2.0),
                paletteIndex(0),
                position(osg::Vec3f(0,0,0))
            {}

            SpriteData( const SpriteData &d ):
                width(d.width),
                height(d.height),
                paletteIndex(d.paletteIndex),
                position(d.position)
            {
            }

        };

        typedef std::vector<SpriteData> SpriteDataList;

        Sprites();
	    Sprites(const Sprites&, const osg::CopyOp&);
        Sprites( const SpriteDataList & );

        void setTexturePalette( TexturePalette * );
        TexturePalette *getTexturePalette() const;

        void setSpriteList( const SpriteDataList & );
        const SpriteDataList &getSpriteList() const;

        unsigned int getNumSprites() { return _numSprites; }

    private:
        SpriteDataList _spriteDataList;
        void _init();
        osg::ref_ptr<TexturePalette> _texturePalette;
        osg::ref_ptr<osg::Uniform> _palette_rows;
        osg::ref_ptr<osg::Uniform> _palette_cols;

        unsigned int _numSprites;
};

}
#endif

