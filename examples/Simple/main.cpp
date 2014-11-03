#include <osg/Notify>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgViewer/Viewer>
#include <osgSprites/Sprites.h>
#include <osgSprites/TexturePalette.h>
#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>

static const std::string spriteSheet = "./data/glowsprites.png";
static const unsigned int spriteRows = 1;
static const unsigned int spriteCols = 4;

static const unsigned int numSprites = 10000000;
static const float sphereInnerRadius = 900.0f;
static const float sphereOutterRadius = 1000.0f;


float randomRange(float min, float max) {
	return min + (((float)(rand()%10000)/10000.0f) * (max-min));
}

float randomAngle() {
	return randomRange(0.0f,osg::PI*2.0f);
}

osg::Geode *loadSprites()
{
    osgSprites::Sprites::SpriteDataList dataList;
    for( unsigned int i = 0; i < numSprites; i++ )
    {
        osgSprites::Sprites::SpriteData spriteData;

		//world space size of the sprite
        spriteData.width  = randomRange(10.0f, 20.0f);
	    spriteData.height = randomRange(10.0f, 20.0f);

		//the index of the cell to use in the palette texture
	    spriteData.paletteIndex = rand() % (spriteCols*spriteRows);

		//random world space position of the sprite in a sphere
		float theta = randomAngle();
		float phi = randomAngle();
		float radius = randomRange(sphereInnerRadius, sphereOutterRadius);

		spriteData.position[0] = cos(theta) * cos(phi) * radius;
		spriteData.position[1] = sin(theta) * cos(phi) * radius;
		spriteData.position[2] = sin(phi) * radius;

        dataList.push_back( spriteData );
    }

    // Create the Sprites passing our list of data
	osgSprites::Sprites * sprites = new osgSprites::Sprites( dataList ); //default geometry shader sprites with screen aligned up
	//osgSprites::Sprites * sprites = new osgSprites::Sprites( dataList, "", false, osgSprites::Sprites::RenderMode::POINT_SPRITES ); //gl point sprites
	sprites->setTransparencyActive(true);
	sprites->setClipTolerance(0.0f);

    // Open the pallete texture
    osg::ref_ptr<osg::Image> spritePaletteImage = osgDB::readImageFile( osgDB::findDataFile(spriteSheet) );
    if( spritePaletteImage.valid() )
    {
        // Make the Palette
        osg::ref_ptr<osgSprites::TexturePalette> texturePalette = new osgSprites::TexturePalette( spriteRows, spriteCols, spritePaletteImage.get() );
        sprites->setTexturePalette( texturePalette.get() );
    }
    else{
        osg::notify( osg::WARN ) << "Can't open the sprite palette image '" << spriteSheet << "'." << std::endl;
		int t = 0;
		std::cin >> t;
	}
    return sprites;
}


int main(int, char **)
{
	osg::setNotifyLevel(osg::WARN);
    osgViewer::Viewer viewer;

	viewer.setCameraManipulator(new osgGA::TrackballManipulator());
    viewer.addEventHandler(new osgViewer::StatsHandler);

	viewer.setSceneData( loadSprites() );
	return viewer.run();
}

