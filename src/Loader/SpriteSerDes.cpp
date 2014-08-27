#ifndef OSG_SPRITES_SER_DES_DEF
#define OSG_SPRITES_SER_DES_DEF 1

#include <string>
#include <sstream>

#include <Chaskii/XML/ElementSerDes.h>
#include <osgSprites/Sprites.h>
#include <osgSprites/TexturePalette.h>
#include <osgDB/ReadFile>
#include <osg/Image>

#include "SpritesElement.h"
#include "TexturePaletteElement.h"
#include "SpriteDataElement.h"
#include "PositionElement.h"

class PositionSerDes: public iiXML::ElementSerDes
{
    public:
        PositionSerDes():
            iiXML::ElementSerDes( "Position" )
        {
            addProperty( iiXML::Property( "x", iiXML::Type_float, &_x ));
            addProperty( iiXML::Property( "y", iiXML::Type_float, &_y ));
            addProperty( iiXML::Property( "z", iiXML::Type_float, &_z ));
        }

        virtual void serialize( iiXML::Element *e, iiXML::ElementList &/*elements*/, std::string  &/*text*/ )
        {
            PositionElement *pe = dynamic_cast<PositionElement *>(e);
            if( pe != NULL )
            {
                osg::Vec3f p = pe->getPosition();
                _x = p[0];
                _y = p[1];
                _z = p[2];
            }
        }

        virtual iiXML::Element *deserialize( iiXML::ElementList &/*elements*/, const std::string &/*text*/ )
        {
            return new PositionElement( osg::Vec3f(_x, _y, _z ) );
        }
    private:
        float _x, _y, _z;
};

static iiMem::ref_ptr<PositionSerDes> _c = new PositionSerDes;

class SpriteDataSerDes: public iiXML::ElementSerDes
{
    public:
        SpriteDataSerDes() :
            iiXML::ElementSerDes( "SpriteData" ) 
        {
            addProperty( iiXML::Property( "width", iiXML::Type_float, &_width ));
            addProperty( iiXML::Property( "height", iiXML::Type_float, &_height ));
            addProperty( iiXML::Property( "paletteIndex", iiXML::Type_int, &_paletteIndex ));
        }

        virtual void serialize( iiXML::Element *e, iiXML::ElementList &elements, std::string  &/*`text  */)
        {
            SpriteDataElement *tde = dynamic_cast<SpriteDataElement *>(e);
            if( tde != NULL )
            {
                osgSprites::Sprites::SpriteData d = tde->getSpriteData();

                elements.push_back( new PositionElement( d.position ));
            }
        }

        virtual iiXML::Element *deserialize( iiXML::ElementList &elements, const std::string &/*text*/ )
        {
            for( iiXML::ElementList::iterator p = elements.begin(); p != elements.end(); p++ )
            {
                if( std::string( (*p)->elementName() ) == "Position" )
                {
                    PositionElement *pe = dynamic_cast<PositionElement *>(p->get());
                    if( pe != NULL )
                    {
                        _position = pe->getPosition();
                    }
                }
            }

            return new SpriteDataElement( osgSprites::Sprites::SpriteData( _width, _height, _paletteIndex, _position ));
        }

    private:
        float _width;
        float _height;
        int _paletteIndex;
        osg::Vec3f _position;
};

static iiMem::ref_ptr<SpriteDataSerDes> _d = new SpriteDataSerDes;

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

        virtual void serialize( iiXML::Element *e, iiXML::ElementList &/*elements*/, std::string  &/*text*/ )
        {
            TexturePaletteElement *tpe = dynamic_cast<TexturePaletteElement *>(e);
            if( tpe != NULL )
            {
                osgSprites::TexturePalette *tp = tpe->getTexturePalette();
                if( tp && tp->getImage() != NULL )
                {
                    _fileName = tp->getImage()->getName();
                    _numRows = tp->getNumRows();
                    _numCols = tp->getNumCols();
                }
                else
                {
                    _fileName = std::string("FILE_NAME_WAS_NOT_STORED_IN_IMAGE" );
                    _numRows = _numCols = 0;
                }
                        
            }

        }

        virtual iiXML::Element *deserialize( iiXML::ElementList &/*elements*/, const std::string &/*text*/ )
        {
            osg::ref_ptr<osg::Image> image = osgDB::readImageFile( _fileName );
            if( image.valid( ) )
                image->setName( _fileName );
            return new TexturePaletteElement( new osgSprites::TexturePalette( _numRows, _numCols, image.get() ));
        }


    private:
        std::string _fileName;
        unsigned int _numRows;
        unsigned int _numCols;
};

static iiMem::ref_ptr<TexturePaletteSerDes> _a = new TexturePaletteSerDes;

class SpritesXMLSerDes: public iiXML::ElementSerDes
{
    public:
        SpritesXMLSerDes():
            ElementSerDes( "Sprites" ) {}

        virtual void serialize( iiXML::Element *e, iiXML::ElementList &elements, std::string  &/*text*/ )
        {
            if( std::string(e->elementName()) == "Sprites" )
            {
                SpritesElement *ste = dynamic_cast<SpritesElement *>(e);
                osgSprites::Sprites *t = ste->getSprites();
                osgSprites::TexturePalette *tp = t->getTexturePalette();
                elements.push_back( new TexturePaletteElement( tp ) );


                const osgSprites::Sprites::SpriteDataList &l = t->getSpriteList();
                for( osgSprites::Sprites::SpriteDataList::const_iterator p = l.begin(); p != l.end(); p++ )
                {
                    osgSprites::Sprites::SpriteData d = *p;

                    elements.push_back( new SpriteDataElement( d ) );
                }
            }
        }

        virtual iiXML::Element *deserialize( iiXML::ElementList &elements, const std::string &/*text*/ )
        {
            osg::ref_ptr<osgSprites::Sprites> s = new osgSprites::Sprites;
            osgSprites::Sprites::SpriteDataList dl;

            for( iiXML::ElementList::iterator p = elements.begin(); p != elements.end(); p++ )
            {
                if( std::string( (*p)->elementName() ) == "TexturePalette" )
                {
                    TexturePaletteElement *e = dynamic_cast<TexturePaletteElement *>(p->get());

                    if( e != NULL )
                    {
                        s->setTexturePalette( e->getTexturePalette() );
                    }
                }
                else if( std::string( (*p)->elementName() ) == "SpriteData" )
                {
                    SpriteDataElement *e = dynamic_cast<SpriteDataElement *>(p->get());

                    if( e != NULL )
                    {
                        dl.push_back( e->getSpriteData() );
                    }
                }
            }

            if( dl.size() > 0 )
	            s->setSpriteList( dl );


            return new SpritesElement( s.get() );
        }

};

static iiMem::ref_ptr<SpritesXMLSerDes> _b = new SpritesXMLSerDes;

#endif
