#include <osg/Notify>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgViewer/Viewer>
#include <osgSprites/Sprites.h>
#include <osgSprites/TexturePalette.h>
#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>

#include "Utils.h"
#include <Windows.h>

static const std::string spriteSheet = "./data/glowsprites.png";
static const unsigned int spriteRows = 1;
static const unsigned int spriteCols = 4;

static const unsigned int defaultParticleCount = 4000000;


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
	//camera->addChild(geode);

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

	//create some random lifes for the particles, we will pass them as userData on the sprites stored in gl_MultiTexCoord0.w
	std::vector<float> lifeTimes = randomRangeVector(numParticles, Rangef(10.0f,20.0f));

	//create our node to run the particle program, has 2 channels for position and velocity
	ComputeNode* computeNode = new ComputeNode(numParticles, "data/particleprogram", 2, true, &lifeTimes);

	OSG_ALWAYS << "Simulating " << numParticles << " particles in data texture of size w: " << computeNode->_input[0]->getTextureWidth() << " h: " << computeNode->_input[0]->getTextureHeight() << std::endl;

	//fill initial positions
	computeNode->_input[0]->fillTextureRandomRange(Rangef(-10.0f,10.0f), Rangef(-10.0f,10.0f), Rangef(0.0f,20.0f));
	//fill initial velocities
	float vel = 10.0f;
	Rangef startDelayRange = Rangef(-2.0f, -1.0f);
	computeNode->_input[1]->fillTextureRandomRange(Rangef(-vel,vel), Rangef(-vel,vel), Rangef(vel*2,vel*5.0f), startDelayRange);

	//why do I need this? somethings wrong
	for(unsigned int i=0; i<computeNode->_input.size(); i++)
		computeNode->_output[i]->setImage(computeNode->_input[i]->getImage());

	group->addChild(computeNode);

	//create the particle sprites that use the positions computed by our compute node
	osgSprites::Sprites* particleSprites = computeNode->_output[0]->createParticleRenderSprites(numParticles, Rangef(0.1f,1.0f), "data/particlesprites");
	particleSprites->setAlphaEnabled(true);
	//bind the positions texture
	particleSprites->getOrCreateStateSet()->setTextureAttributeAndModes(1, computeNode->_output[0], osg::StateAttribute::ON);
	particleSprites->getOrCreateStateSet()->addUniform(new osg::Uniform("dataChannel0", 1));
	
	osg::Depth* depth = new osg::Depth();
	depth->setWriteMask( false );
	particleSprites->getOrCreateStateSet()->setAttributeAndModes( depth, osg::StateAttribute::ON );
	
	osg::ref_ptr<osg::Image> spritePaletteImage = osgDB::readImageFile( osgDB::findDataFile(spriteSheet) );
    if( spritePaletteImage.valid() )
        particleSprites->setTexturePalette( new osgSprites::TexturePalette( spriteRows, spriteCols, spritePaletteImage.get() ) );

	group->addChild(particleSprites);

	osg::Node* preview = createPreviewRenderGraph(computeNode->_output[0]);
	group->addChild(preview);

	return group;
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
	viewer.getCamera()->setComputeNearFarMode(osg::Camera::DO_NOT_COMPUTE_NEAR_FAR);
	viewer.setCameraManipulator(new osgGA::TrackballManipulator());
    viewer.addEventHandler(new osgViewer::StatsHandler);
	
	viewer.setSceneData(createParticles(numParticles));
	
	return viewer.run();
}

