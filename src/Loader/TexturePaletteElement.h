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
#ifndef TEXTURE_PALETTE_ELEMENT_DEF
#define TEXTURE_PALETTE_ELEMENT_DEF 1

class TexturePaletteElement: public iiXML::Element
{
    public:
        TexturePaletteElement( osgSprites::TexturePalette *tp ):
            _tp(tp) {}

        virtual const char *elementName() { return "TexturePalette"; }

        osgSprites::TexturePalette * getTexturePalette() { return _tp.get(); }

    private:
        osg::ref_ptr<osgSprites::TexturePalette> _tp;
};

#endif
