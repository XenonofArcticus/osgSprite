#include <osgDB/Registry>
#include <iostream>
#include <string>
#include <osgDB/Registry>
#include <osgDB/Input>
#include <osgDB/Output>
#include <osgDB/DynamicLibrary>
#include <osgDB/FileUtils>


#include <osgSprites/Sprites.h>

bool Sprites_readLocalData(osg::Object& obj, osgDB::Input& fr);
bool Sprites_writeLocalData(const osg::Object& obj, osgDB::Output& fw);

osgDB::RegisterDotOsgWrapperProxy g_SpritesProxy (
     new osgSprites::Sprites,
         "Sprites",
         "Sprites",
          &Sprites_readLocalData,
          &Sprites_writeLocalData,
          osgDB::DotOsgWrapper::READ_AND_WRITE

);

bool Sprites_readLocalData(osg::Object& obj, osgDB::Input& fr)
{
    bool itAdvanced = false;
    osgSprites::Sprites &st = static_cast<osgSprites::Sprites&>(obj);

    //printf("Sprites_readLocalData called fr[0] = %s\n", fr[0].getStr() );
    if( fr.matchSequence( "TexturePalette {" ) )
    {
        osg::ref_ptr<osg::Image> image;
        int numRows = 0;
        int numCols = 0;

        int entry = fr[0].getNoNestedBrackets();
        fr += 2;
        while (!fr.eof() && fr[0].getNoNestedBrackets()>entry)
        {
            if( fr[0].matchWord( "fileName" ) && fr[1].isString() )
            {
                image = fr.readImage( fr[1].getStr() );
                fr += 2;
            }

            if( fr[0].matchWord( "numRows" ) )
            {
                fr[1].getInt( numRows );
                fr += 2;
            }

            if( fr[0].matchWord( "numCols" ) )
            {
                fr[1].getInt( numCols );
                fr += 2;
            }
        }

        if( image.valid() )
        {
            st.setTexturePalette( new osgSprites::TexturePalette( numRows, numCols, image.get() ) );
        }
        itAdvanced = true;
    }


    if( fr.matchSequence( "SpriteData {" ) )
    {
        int entry = fr[0].getNoNestedBrackets();
        fr += 2;
        osgSprites::Sprites::SpriteDataList dl;
        while( !fr.eof() && fr[0].getNoNestedBrackets() > entry)
        {
            float width, height;
            unsigned int paletteIndex;
            float x, y, z;

            fr[0].getFloat( width ); 
            fr[1].getFloat( height ); 
            fr[2].getUInt( paletteIndex ); 
            fr[3].getFloat( x ); 
            fr[4].getFloat( y ); 
            fr[5].getFloat( z ); 
            fr += 6;

            //printf("%f %f %d %f %f %f\n", width, height, paletteIndex, x, y , z );
            dl.push_back( osgSprites::Sprites::SpriteData( width, height, paletteIndex, osg::Vec3( x, y, z )));
        }

        if( dl.size() > 0 )
            st.setSpriteList( dl );
    }

    return itAdvanced;
}

bool Sprites_writeLocalData(const osg::Object& obj, osgDB::Output& fw)
{
    const osgSprites::Sprites &st = static_cast<const osgSprites::Sprites&>(obj);

    const osgSprites::TexturePalette *tp = st.getTexturePalette();
    std::string imageFileName = std::string("IMAGE_NOT_FOUND");
    if( tp && tp->getImage() != NULL )
        std::string imageFileName = tp->getImage()->getName();

    fw.indent() << "TexturePalette {" << std::endl;
    fw.moveIn();
    fw.indent() << "fileName \"" << imageFileName << "\"" << std::endl;
    fw.indent() << "numRows  " << tp->getNumRows() << std::endl;
    fw.indent() << "numCols  " << tp->getNumCols() << std::endl;
    fw.moveOut();

    fw.indent() << "}" << std::endl;

    fw.indent() << "Comment { " << std::endl;
    fw.moveIn();
    fw.indent() << "SpriteData records: " << std::endl;
    fw.indent() << "width height paletteIndex X Y Z" << std::endl;
    fw.moveOut();
    fw.indent() << "}" << std::endl;

    fw.indent() << "SpriteData { " << std::endl;
    fw.moveIn();

    const osgSprites::Sprites::SpriteDataList &dl = st.getSpriteList();
    for( osgSprites::Sprites::SpriteDataList::const_iterator p = dl.begin(); p != dl.end(); p++ )
    {
        fw.indent() << p->width << "  " << p->height << "  "  << p->paletteIndex << "  " << 
            p->position[0] << " " << p->position[1] << " " <<  p->position[2] <<  std::endl;
    }

    fw.moveOut();
    fw.indent() << "}" << std::endl;


    return true;
}
