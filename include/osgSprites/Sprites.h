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

		enum RenderMode {
			GEOMETRY_SHADER_SPRITES,
			POINT_SPRITES
		};

        struct SpriteData 
        {
            float width;
            float height;
            unsigned int paletteIndex;
            osg::Vec3f position;
			osg::Vec3f up;

            SpriteData( float w, float h, unsigned int pi, const osg::Vec3f &p ):
                width(w),
                height(h),
				paletteIndex(pi),
                position(p),
				up(osg::Vec3f(0,1,0))
            {}

				SpriteData( float w, float h, unsigned int pi, const osg::Vec3f &p, const osg::Vec3f &u ):
                width(w),
                height(h),
				paletteIndex(pi),
                position(p),
				up(u)
            {}

            SpriteData() :
                width(1.0),
                height(2.0),
                paletteIndex(0),
                position(osg::Vec3f(0,0,0)),
				up(osg::Vec3f(0,1,0))
            {}

            SpriteData( const SpriteData &d ):
                width(d.width),
                height(d.height),
                paletteIndex(d.paletteIndex),
                position(d.position),
				up(d.up)
            {
            }

        };

        typedef std::vector<SpriteData> SpriteDataList;

		//
		// shaderFile will be used to find shaderFile + .vert, shaderFile + .geom and shaderFile + .frag
		// e.g if shaders are data/myspriteshader.vert, data/myspriteshader.geom and data/myspriteshader.frag
		// then pass data/myspriteshader, any that is not found will use default shader source in code
        Sprites(const std::string &shaderFile = "", const RenderMode &renderMode = RenderMode::GEOMETRY_SHADER_SPRITES);
	    Sprites(const Sprites&, const osg::CopyOp&);
        Sprites(const SpriteDataList &, const std::string &shaderFile = "", const bool& useUpVector = false, const RenderMode &renderMode = RenderMode::GEOMETRY_SHADER_SPRITES);

        void setTexturePalette( TexturePalette * );
        TexturePalette *getTexturePalette() const;

        void setSpriteList( const SpriteDataList &, const bool& useUpVector = false);
        const SpriteDataList &getSpriteList() const;

        unsigned int getNumSprites() { return _numSprites; }

		bool getUseUpVector() { return _useUpVector; }

    private:
        SpriteDataList _spriteDataList;
        void _init(const std::string &shaderFile);
        osg::ref_ptr<TexturePalette> _texturePalette;
        osg::ref_ptr<osg::Uniform> _palette_rows;
        osg::ref_ptr<osg::Uniform> _palette_cols;

        unsigned int _numSprites;

		bool _useUpVector;
		RenderMode _renderMode;
};

}
#endif

