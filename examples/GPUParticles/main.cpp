#include <osg/Notify>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgViewer/Viewer>
#include <osgSprites/Sprites.h>
#include <osgSprites/TexturePalette.h>
#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>

#include "Utils.h"
#include "GPUParticles.h"

static const std::string spriteSheet = "./data/glowsprites.png";
static const unsigned int spriteRows = 1;
static const unsigned int spriteCols = 4;

static const unsigned int defaultParticleCount = 1000000;


osg::Node *createPreviewRenderGraph(DataTexture* texture)
{
	int w = 500;//texture->getTextureWidth();
	int h = 500;//texture->getTextureHeight();

	osg::Camera* camera = new osg::Camera();
	camera->setViewport(0, 0, w, h);
	camera->setRenderOrder(osg::Camera::POST_RENDER);
	camera->setClearColor(osg::Vec4(0.0f,0.0f,0.0f,0.0f));
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	camera->setProjectionMatrix(osg::Matrix::ortho2D(0, w, 0, h));
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setViewMatrix(osg::Matrix::identity());
	camera->setComputeNearFarMode(osg::Camera::DO_NOT_COMPUTE_NEAR_FAR);

	osg::Geode* geode = new osg::Geode();
	geode->addDrawable(osg::createTexturedQuadGeometry(osg::Vec3(0,0,0), osg::Vec3(w,0,0), osg::Vec3(0,h,0)));
	camera->addChild(geode);

	geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
	osg::Depth* depth = new osg::Depth();
	depth->setWriteMask( false );
	geode->getOrCreateStateSet()->setAttributeAndModes( depth, osg::StateAttribute::ON );
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	return camera;
}


osg::Node* createParticles(const unsigned int& numParticles)
{
	osg::Group* group = new osg::Group();

	float vel = 10.0f;
	Rangef startDelayRange = Rangef(-2.0f, -1.0f);
	Rangef massRange = Rangef(0.5f,1.0f);
	Rangef lifeRange = Rangef(1.0f, 5.0f);

	//create our node to run the particle program
	GPUParticles* particleNode = new GPUParticles(numParticles, "data/particleprogram-perfrag", massRange, lifeRange);
	
	OSG_ALWAYS << "Simulating " << numParticles << " particles in data texture of size w: " << particleNode->_input[0]->getTextureWidth() << " h: " << particleNode->_input[0]->getTextureHeight() << std::endl;

	//fill initial positions
	particleNode->_input[0]->fillTextureRandomRange(Rangef(-10.0f,10.0f), Rangef(-10.0f,10.0f), Rangef(0.0f,20.0f));
	//fill initial velocities
	particleNode->_input[1]->fillTextureRandomRange(Rangef(-vel,vel), Rangef(-vel,vel), Rangef(vel*2,vel*5.0f), startDelayRange);

	//why do I need this? somethings wrong
	for(unsigned int i=0; i<particleNode->_input.size(); i++)
		particleNode->_output[i]->setImage(particleNode->_input[i]->getImage());

	group->addChild(particleNode);

	//
	//render the particles

	//create the particle sprites that use the positions computed by our compute node
	osgSprites::Sprites* particleSprites = particleNode->createParticleRenderSprites(numParticles, Rangef(0.1f,1.0f), "data/particlesprites");
	particleSprites->setAlphaEnabled(true);
	//bind the positions texture
	particleSprites->getOrCreateStateSet()->setTextureAttributeAndModes(1, particleNode->_output[0], osg::StateAttribute::ON);
	particleSprites->getOrCreateStateSet()->addUniform(new osg::Uniform("dataChannel0", 1));
	
	osg::Depth* depth = new osg::Depth();
	depth->setWriteMask( false );
	particleSprites->getOrCreateStateSet()->setAttributeAndModes( depth, osg::StateAttribute::ON );
	
	osg::ref_ptr<osg::Image> spritePaletteImage = osgDB::readImageFile( osgDB::findDataFile(spriteSheet) );
    if( spritePaletteImage.valid() )
        particleSprites->setTexturePalette( new osgSprites::TexturePalette( spriteRows, spriteCols, spritePaletteImage.get() ) );

	group->addChild(particleSprites);

	//group->addChild(createPreviewRenderGraph(particleNode->_input[0]));

	return group;
}

void setupViewerOnSingleScreen(osgViewer::Viewer* viewer, unsigned int screenID)
{
	unsigned int screenWidth, screenHeight;
    osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();
    wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(screenID), screenWidth, screenHeight);

	OSG_ALWAYS << "Screen width " << screenWidth << " height " << screenHeight << std::endl;

	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    
	// Setup the traits parameters
	traits->x = 0;
	traits->y = 0;
	traits->width = screenWidth;
	traits->height = screenHeight;
	traits->depth = 24;
	traits->alpha = 8;
    traits->samples = 0;
    traits->sampleBuffers = 0;
	traits->stencil = 0;
	traits->windowDecoration = false;
	traits->doubleBuffer = true;
	traits->vsync = false;
	traits->sharedContext = 0;
    
	// Create the Graphics Context
	osg::ref_ptr<osg::GraphicsContext> graphicsContext = osg::GraphicsContext::createGraphicsContext(traits.get());
	
	if(graphicsContext)
        viewer->getCamera()->setGraphicsContext(graphicsContext);

	viewer->getCamera()->setViewport(new osg::Viewport(0, 0, screenWidth, screenHeight));
    viewer->getCamera()->setProjectionMatrixAsPerspective(45.0f,(float)screenWidth/screenHeight,
                                                           0.1, 1000.0);
}

int main(int argc, char **argv)
{
	osg::setNotifyLevel(osg::WARN);
	osg::ArgumentParser args = osg::ArgumentParser(&argc, argv);
	args.getApplicationUsage()->addCommandLineOption("--numParticles <count>","Number of particles to be simulated.");
	
	int numParticles = defaultParticleCount;
	int readint = 0;
    if (args.read("--numParticles",readint))
		numParticles = readint;

    osgViewer::Viewer viewer;
	setupViewerOnSingleScreen(&viewer, 0);
	viewer.getCamera()->setComputeNearFarMode(osg::Camera::DO_NOT_COMPUTE_NEAR_FAR);
	viewer.setCameraManipulator(new osgGA::TrackballManipulator());
    viewer.addEventHandler(new osgViewer::StatsHandler);
	
	viewer.setSceneData(createParticles(numParticles));
	
	return viewer.run();
}

