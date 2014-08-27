#include <stdlib.h>

#include <Chaskii/XML/SerDes.h>

#include <osgDB/Registry>
#include <osgDB/ReaderWriter>
#include <osgDB/ReadFile>
#include <osgDB/FileNameUtils>
#include <osg/Geode>
#include <osgSprites/Sprites.h>
#include "SpritesElement.h"


class osgSpritesLoader : public osgDB::ReaderWriter
{
    public:
        osgSpritesLoader() {  }

        virtual const char* className() { return "osgSpritesLoader"; }

        virtual bool acceptsExtension(const std::string& extension) const { if( extension == "sprites" ) return true; else return false; }

	    virtual ReadResult readObject(const std::string& fileName, const Options* opt) const
        {
            return readNode( fileName, opt );
        }

        virtual ReadResult readNode(const std::string& fileName, const Options* /*opt*/) const
        {
            if( !acceptsExtension( osgDB::getFileExtension(fileName) ) )
                return ReadResult::FILE_NOT_HANDLED;

            osgSprites::Sprites *sprites = NULL;

            iiXML::SerDes sd;
            iiXML::RootElement *root = sd.deserializeFile( fileName.c_str() );
            if( root == NULL )
                return ReadResult::FILE_NOT_HANDLED;

            if( std::string(root->elementName()) == "Sprites" )
            {
                SpritesElement *ste = dynamic_cast<SpritesElement *>(root);
                if( ste != NULL )
                {
                    sprites = ste->getSprites();
                }
            }

            if( sprites != NULL )
            {
                return sprites;
            }

            return ReadResult::FILE_NOT_HANDLED;
           
        }

        virtual WriteResult writeNode(const osg::Node& node,
                const std::string& fileName, const Options* =NULL) const 
        { 
            if( !acceptsExtension( osgDB::getFileExtension(fileName) ) )
                return WriteResult::FILE_NOT_HANDLED;
            const osg::Geode *g = dynamic_cast<const osg::Geode *>(&node);

            const osgSprites::Sprites *const_sprites = dynamic_cast<const osgSprites::Sprites *>(g->getDrawable(0));

            if( const_sprites != NULL )
            {
                osgSprites::Sprites *sprites = const_cast<osgSprites::Sprites *>(const_sprites);
                iiMem::ref_ptr<SpritesElement> e = new SpritesElement(sprites);

                iiXML::SerDes sd;
                sd.serialize( fileName.c_str(), e );

            }

            return WriteResult(WriteResult::FILE_SAVED); 
        }
};


osgDB::RegisterReaderWriterProxy< osgSpritesLoader > g_ost_Proxy;

