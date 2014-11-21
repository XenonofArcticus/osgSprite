#pragma once

#include "ComputeNode.h"

class GPUParticles : public ComputeNode
{
public:

	struct GPUParticleBoundCallback : public osg::Node::ComputeBoundingSphereCallback {
        GPUParticles* _particles;
        GPUParticleBoundCallback(GPUParticles* particles) : _particles(particles) { }
        osg::BoundingSphere computeBound(const osg::Node&) const { return osg::BoundingSphere(_particles->_center, _particles->_radius); }
	};

	GPUParticles(const unsigned int& arraySize, const std::string& program, Rangef massRange, Rangef lifeRange)
		: ComputeNode(arraySize, program, 2, true, NULL),
		_center(osg::Vec3(0.0f,0.0f,40.0f)),
		_radius(30.0f)
	{
		//static data texture mass, lifetime, 0, 0
		DataTexture* particleDataTexture = new DataTexture(arraySize);
		std::vector<osg::Vec4> fillData;
		for(unsigned int i=0; i<arraySize; i++)
			fillData.push_back(osg::Vec4(randomRange(massRange), randomRange(lifeRange), 0.0f, 0.0f));
		particleDataTexture->fillTexture(fillData);

		//attach the particle data texture to our compute node to use as input in the shader (use channel 2 as 0 and 1 our used for the double buffer)
		this->getOrCreateStateSet()->setTextureAttributeAndModes(2, particleDataTexture, osg::StateAttribute::ON);
		this->getOrCreateStateSet()->addUniform(new osg::Uniform("inputChannel0", 2));

		this->setComputeBoundingSphereCallback(new GPUParticleBoundCallback(this));

		//why do I need this? somethings wrong
		for(unsigned int i=0; i<this->_input.size(); i++)
			this->_output[i]->setImage(this->_input[i]->getImage());
	}

	osgSprites::Sprites* createParticleRenderSprites(unsigned int spriteCount, Rangef sizeRange, const std::string& programPath) 
	{
		if(_input.size() == 0) return NULL;

		osgSprites::Sprites::SpriteDataList dataList;
		for(unsigned int i = 0; i < spriteCount; i++ )
		{
			osgSprites::Sprites::SpriteData spriteData;
		
			osg::Vec2 colRow = _input[0]->computeColRowForIndex(i);

			//stash uv coord for our positions data in the gl_vertex as texcoord0 is used for width/height data
			spriteData.position = osg::Vec3(_input[0]->computeUVForColRow(colRow), 0.0f);

			//unit/tex coord location
			spriteData.width = randomRange(sizeRange);
			spriteData.height = spriteData.width;

			spriteData.paletteIndex = rand()%4;
			spriteData.userData = randomRange(0.01f,1.0f);

			dataList.push_back( spriteData );
		}
		return new osgSprites::Sprites( dataList, programPath, false, osgSprites::Sprites::GEOMETRY_SHADER_SPRITES);
	}

	osg::Vec3 _center;
	float _radius;
};