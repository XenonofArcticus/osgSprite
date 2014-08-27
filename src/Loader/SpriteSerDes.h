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
#ifndef OSG_SPRITES_SER_DES_DEF
#define OSG_SPRITES_SER_DES_DEF 1

#include <Chaskii/XML/ElementSerDes.h>
#include <osgSprites/TexturePalette.h>

class TexturePaletteElement: public iiXML::Element
{
    public:
        TexturePaletteElement( TexturePalette *tp ):
            _tp(tp) {}

        virtual const char *elementName() { return "TexturePalette"; }

        TexturePalette * getTexturePalette() { return _tp.get(); }

    private:
        osg::ref_ptr<TexturePalette> _tp;
};

class TexturePaletteSerDes: public iiXML::ElementSerDes
{
    public:
        TexturePaletteSerDes():
            iiXML::ElementSerDes( "TexturePalette" )
        {
            addProperty( iiXML::Property( "imageFile", iiXML::Type_string, &_fileName ) );
            addProperty( iiXML::Property( "numRows", iiXML::Type_int, &_numRows ) );
            addProperty( iiXML::Property( "numCols", iiXML::Type_int, &_numCols ) );
        }

        virtual void serialize( Element *, ElementList &elements, std::string  &text )
        {
        }

        virtual Element *deserialize( ElementList &elements, const std::string &text )
        {
            osg::ref_ptr<osg::Image> image = osgDB::readImageFile( _fileName );
            return new TexturePaletteElement( new TexturePalette( _numRows, _numCols, image.get() ));
        }


    private:
        std::string _fileName;
        unsigned int _numRows;
        unsigned int _numCols;
};

class osgSpritesXMLSerDes: public iiXML::ElementSerDes
{
    public:
        osgSpritesXMLSerDes():
            ElementSerDes( "Sprites" ) {}


};


#endif
