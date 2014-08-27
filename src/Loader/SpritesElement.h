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
#ifndef SPRITE_TREES_ELEMENT_DEF
#define SPRITE_TREES_ELEMENT_DEF 1

class SpritesElement: public iiXML::Element
{
    public:
        SpritesElement( osgSprites::Sprites *osgSprites ):
            _osgSprites(osgSprites) {}

        virtual const char *elementName() { return "Sprites"; }
        osgSprites::Sprites * getSprites() { return _osgSprites.get(); }

    private:
        osg::ref_ptr<osgSprites::Sprites> _osgSprites;
};

#endif
