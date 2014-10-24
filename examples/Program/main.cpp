#include <osg/Notify>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgViewer/Viewer>
#include <osgSprites/Sprites.h>
#include <osgSprites/TexturePalette.h>
#include <osgGA/TrackballManipulator>

static const unsigned int numTrees = 4000;
/*
osg::Geode *loadTrees()
{

    // Invent some random tree positions 
    float area = sqrt((double)numTrees) * 4.0;
    osgSprites::Sprites::SpriteDataList dataList;
    for( unsigned int i = 0; i < numTrees; i++ )
    {
        osgSprites::Sprites::SpriteData treeData;

        treeData.width  = 5.0f + ((float)(rand()%1000)/1000.0f) * 20.0f;
	    treeData.height = 10.0f + ((float)(rand()%1000)/1000.0f) * 40.0f;
	    treeData.paletteIndex = rand() % 5;

        treeData.position[0] = -(area*0.5) + ((float)(rand() % 10000)/10000.0f) * area;
        treeData.position[1] = -(area*0.5) + ((float)(rand() % 10000)/10000.0f) * area;
        treeData.position[2] = 0.0f;

        dataList.push_back( treeData );
    }


    // Create the Sprite Trees
    osgSprites::Sprites * trees = new osgSprites::Sprites( dataList );

    // Open the pallete texture
    osg::ref_ptr<osg::Image> treePaletteImage = osgDB::readImageFile( osgDB::findDataFile("./data/trees.png") );
    if( treePaletteImage.valid() )
    {
        // Make the Palette
        osg::ref_ptr<osgSprites::TexturePalette> texturePalette = new osgSprites::TexturePalette( 1, 5, treePaletteImage.get() );
        trees->setTexturePalette( texturePalette.get() );
    }
    else{
        osg::notify( osg::WARN ) << "Can't open the tree palette image \"trees.png\"" << std::endl;
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

	viewer.setSceneData( loadTrees() );
	return viewer.run();
}
*/


#include <osg/Notify>
#include <osgViewer/Viewer>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/ExampleResources>
#include <osgEarthFeatures/Filter>

#define LC "[viewer] "

using namespace osgEarth;
using namespace osgEarth::Features;
using namespace osgEarth::Util;

int
main(int argc, char** argv)
{    
    //Run this example with the the feature_custom_filters.earth file in the tests directory for a simple example
    osg::ArgumentParser arguments(&argc,argv);
    if ( arguments.read("--stencil") )
        osg::DisplaySettings::instance()->setMinimumNumStencilBits( 8 );

    // create a viewer:
    osgViewer::Viewer viewer(arguments);

    //Tell the database pager to not modify the unref settings
    viewer.getDatabasePager()->setUnrefImageDataAfterApplyPolicy( false, false );

    // install our default manipulator (do this before calling load)
    viewer.setCameraManipulator( new EarthManipulator() );

	osgEarth::setNotifyLevel(osg::FATAL);

    // load an earth file, and support all or our example command-line options
    // and earth file <external> tags    
    osg::Node* node = MapNodeHelper().load( arguments, &viewer );
    if ( node )
    {
        viewer.setSceneData( node );

        // configure the near/far so we don't clip things that are up close
        viewer.getCamera()->setNearFarRatio(0.00002);

        viewer.run();
    }
    else
    {
        OE_NOTICE 
            << "\nUsage: " << argv[0] << " file.earth" << std::endl
            << MapNodeHelper().usage() << std::endl;
    }
    return 0;
}
