#pragma once

#include <osg/Switch>
#include <osg/Texture2D>
#include <osg/Depth>
#include <osgSprites/Sprites.h>
#include "DataTexture.h"

class ComputeNode : public osg::Switch
{
public:

	class DoubleBufferCameras : public osg::NodeCallback 
    {
    public:
       virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
       {
          osg::ref_ptr<ComputeNode> computeNode = dynamic_cast<ComputeNode*>(node);
          if(computeNode)
			  computeNode->swap();

          traverse(node, nv); 
       }
    };

	ComputeNode(const unsigned int& arraySize, const std::string& program, const unsigned int& numChannels = 1, bool isFloat=true, std::vector<float>* userData = NULL)
		: osg::Switch(),
		_numChannels(numChannels),
		_flipFlop(true)
	{
		init(arraySize,program, numChannels, isFloat, userData);
	}

	void init(const unsigned int& arraySize, const std::string& program, const unsigned int& numChannels = 1, bool isFloat=true, std::vector<float>* userData = NULL)
	{
		for(unsigned int i=0; i<numChannels; i++) {
			_input.push_back(new DataTexture(arraySize,isFloat));
			_output.push_back(new DataTexture(arraySize,isFloat));
		}

		int w = _input[0]->getTextureWidth();
		int h = _input[0]->getTextureHeight();

		//osg::Geode* inputGeode = _input[0]->createOperationSprites(program, userData);
		osg::Geode* inputGeode = this->createOperationQuad(program);

		for(unsigned int i=0; i<numChannels; i++) {
			std::ostringstream ss;
			ss << "dataChannel" << i;
			inputGeode->getOrCreateStateSet()->addUniform( new osg::Uniform( ss.str().c_str(), (int)i ) ); //positions
		}

		//create our camera, first uses reads from input and renders to output
		//second reads from output and renders input, we toggle which is rendered after each frame
		//to double buffer our data textures
		for(unsigned int i=0; i<2; i++)
		{
			osg::Camera* camera = new osg::Camera();
			camera->setViewport(0, 0, w, h);
			camera->setRenderOrder(osg::Camera::PRE_RENDER);
			camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
			camera->setClearColor(osg::Vec4(0.0f,0.0f,0.0f,0.0f));
			camera->setClearMask(0);
			camera->setImplicitBufferAttachmentMask(osg::Camera::IMPLICIT_COLOR_BUFFER_ATTACHMENT);
			camera->setProjectionMatrix(osg::Matrix::ortho2D(0, w, 0, h));
			camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
			camera->setViewMatrix(osg::Matrix::identity());
			camera->setComputeNearFarMode(osg::Camera::DO_NOT_COMPUTE_NEAR_FAR);
			
			camera->addChild(inputGeode);
			
			//which textures to use for input and output
			std::vector< DataTextureRef > inputs = i==0 ? _input : _output;
			std::vector< DataTextureRef > outputs = i!=0 ? _input : _output;
			
			//attach inputs textures to our stateset so the sprites use them
			for(unsigned int i=0; i<inputs.size(); i++){
				camera->getOrCreateStateSet()->setTextureAttribute( i, inputs[i], osg::StateAttribute::ON );

				osg::Depth* depth = new osg::Depth();
				depth->setWriteMask( false );
				camera->getOrCreateStateSet()->setAttributeAndModes( depth, osg::StateAttribute::ON );
				camera->getOrCreateStateSet()->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
			}
			//attach outpus to the camera to render to
			for(unsigned int i=0; i<outputs.size(); i++)
				camera->attach(osg::Camera::BufferComponent(osg::Camera::COLOR_BUFFER0+i), outputs[i]); 

			this->addChild(camera);
		}

		this->setUpdateCallback(new DoubleBufferCameras());
		this->setSingleChildOn(0);
	}

	void swap()
	{
		_flipFlop = !_flipFlop;
		this->setSingleChildOn(_flipFlop ? 0 : 1);
	}

	osgSprites::Sprites* createOperationSprites(const std::string programPath, const std::vector<float>* userData = NULL) 
	{
		//must have an input texture
		if(_input.size() == 0) return NULL;

		osgSprites::Sprites::SpriteDataList dataList = _input[0]->createIndexDataList();
		if(userData != NULL && userData->size() == dataList.size())
		{
			for(unsigned int i=0; i<userData->size(); i++)
				dataList[i].userData = (*userData)[i];
		}
		return new osgSprites::Sprites( dataList, programPath, false, osgSprites::Sprites::POINTS);
	}

	osg::Geode* createOperationQuad(const std::string programPath) 
	{
		//must have an input texture
		if(_input.size() == 0) return NULL;

		osg::Vec4Array *verts = new osg::Vec4Array;
		osg::Vec2 bottomLeft = osg::Vec2(-1.0f,-1.0f);
		osg::Vec2 topRight = osg::Vec2(1.0f,1.0f);

		verts->push_back(osg::Vec4(bottomLeft.x(), bottomLeft.y(), 0.0f, 1.0f));
		verts->push_back(osg::Vec4(topRight.x(), bottomLeft.y(), 0.0f, 1.0f));
		verts->push_back(osg::Vec4(topRight.x(), topRight.y(), 0.0f, 1.0f));
		verts->push_back(osg::Vec4(bottomLeft.x(), topRight.y(), 0.0f, 1.0f));

		//
		osg::Vec4Array *coords = new osg::Vec4Array;
		osg::Vec2 bottomLeftUV = osg::Vec2(0.0f,0.0f);
		osg::Vec2 topRightUV = osg::Vec2(1.0f,1.0f);

		coords->push_back(osg::Vec4(bottomLeftUV.x(), bottomLeftUV.y(), 0, 0));
		coords->push_back(osg::Vec4(topRightUV.x(), bottomLeftUV.y(), 0, 0));
		coords->push_back(osg::Vec4(topRightUV.x(), topRightUV.y(), 0, 0));
		coords->push_back(osg::Vec4(bottomLeftUV.x(), topRightUV.y(), 0, 0));

		osg::Geometry *geom = new osg::Geometry;
		geom->setUseVertexBufferObjects(true);
		geom->setVertexArray( verts );
		geom->setTexCoordArray(0, coords);

	#if defined(OSG_GLES1_AVAILABLE) || !defined(OSG_GLES2_AVAILABLE)
		osg::DrawElementsUByte* elems = new osg::DrawElementsUByte(osg::PrimitiveSet::TRIANGLES);
		elems->push_back(0);
		elems->push_back(1);
		elems->push_back(2);

		elems->push_back(2);
		elems->push_back(3);
		elems->push_back(0);
		geom->addPrimitiveSet(elems);
	#else
		geom->addPrimitiveSet(new DrawArrays(PrimitiveSet::QUADS,0,4));
	#endif

		osg::Geode* geode = new osg::Geode();
		geode->addDrawable(geom);
		
		osg::StateSet *sset = geode->getOrCreateStateSet();
		osg::Program* program = new osg::Program();
		sset->setAttribute(program);
		program->addShader(osg::Shader::readShaderFile(osg::Shader::VERTEX, programPath + ".vert" ) );
		program->addShader(osg::Shader::readShaderFile(osg::Shader::FRAGMENT, programPath + ".frag" ) );

		return geode;
	}


	unsigned int _numChannels;
	std::vector< DataTextureRef > _input;
	std::vector< DataTextureRef > _output;
	bool _flipFlop;
};
