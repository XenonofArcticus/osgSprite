#include <osg/Notify>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgViewer/Viewer>
#include <osgSprites/Sprites.h>
#include <osgSprites/TexturePalette.h>
#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>

static const std::string spriteSheet = "./data/trees.png";
static const unsigned int spriteRows = 1;
static const unsigned int spriteCols = 5;

static const unsigned int numTrees = 1000000;
static const float treeHeightMin = 20.0f;
static const float treeHeightMax = 40.0f;
static const float spacePerTree = 30.0f;

float randomRange(float min, float max) {
	return min + (((float)(rand()%10000)/10000.0f) * (max-min));
}

osg::Geode *loadTrees()
{
    // Invent some random tree positions 
    float area = sqrt((double)numTrees) * spacePerTree;

    osgSprites::Sprites::SpriteDataList dataList;
    for( unsigned int i = 0; i < numTrees; i++ )
    {
        osgSprites::Sprites::SpriteData treeData;

        treeData.width  = randomRange(treeHeightMin, treeHeightMax);
	    treeData.height = treeData.width; //make square

	    treeData.paletteIndex = rand() % (spriteCols*spriteRows);

		//if using treessprites shader this sets the sway rate of the tree stored in texcoord0.w
		treeData.userData = randomRange(0.5f, 3.0f);

		//random world space position of the tree in x,y plane
        treeData.position[0] = randomRange(-(area*0.5f), area*0.5f);
        treeData.position[1] = randomRange(-(area*0.5f), area*0.5f);
        treeData.position[2] = 0.0f;

		//set the world up axis for the tree
		treeData.up = osg::Vec3(0,0,1);

        dataList.push_back( treeData );
    }

	//use custom shader with fixed up that applies a sway to the trees
	osgSprites::Sprites * trees = new osgSprites::Sprites( dataList, "data/treesprites", true );

    // Open the pallete texture
    osg::ref_ptr<osg::Image> treePaletteImage = osgDB::readImageFile( osgDB::findDataFile(spriteSheet) );
    if( treePaletteImage.valid() )
    {
        // Make the Palette
        osg::ref_ptr<osgSprites::TexturePalette> texturePalette = new osgSprites::TexturePalette( spriteRows, spriteCols, treePaletteImage.get() );
        trees->setTexturePalette( texturePalette.get() );
    }
    else{
        osg::notify( osg::WARN ) << "Can't open the tree palette image '" << spriteSheet << "'." << std::endl;
		int t = 0;
		std::cin >> t;
	}
    return trees;
}


int main(int, char **)
{
	osg::setNotifyLevel(osg::WARN);
    osgViewer::Viewer viewer;

	viewer.setCameraManipulator(new osgGA::TrackballManipulator());
    viewer.addEventHandler(new osgViewer::StatsHandler);

	viewer.setSceneData( loadTrees() );
	return viewer.run();
}

