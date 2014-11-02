#include <osg/Notify>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgViewer/Viewer>
#include <osgSprites/Sprites.h>
#include <osgSprites/TexturePalette.h>
#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>

static const unsigned int numTrees = 1000000;

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

		treeData.up = osg::Vec3(0,0,1);

        dataList.push_back( treeData );
    }


    // Create the Sprite Trees
	//osgSprites::Sprites * trees = new osgSprites::Sprites( dataList );//, "", false, osgSprites::Sprites::RenderMode::POINT_SPRITES );
    osgSprites::Sprites * trees = new osgSprites::Sprites( dataList, "data/upsprites", true ); //sprites have fixed up axis

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
    viewer.addEventHandler(new osgViewer::StatsHandler);

	viewer.setSceneData( loadTrees() );
	return viewer.run();
}

