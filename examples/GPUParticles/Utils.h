
#include <osg/Texture2D>
#include <osg/Depth>
#include <osgSprites/Sprites.h>


struct Rangei
{
	Rangei(int mi, int ma)
		: min(mi),
		max(ma)
	{}

	int min;
	int max;
};
struct Rangef
{
	Rangef(float mi, float ma)
		: min(mi),
		max(ma)
	{}
	float min;
	float max;
};


inline float randomRange(float min, float max) {
	return min + (((float)(rand()%RAND_MAX)/(float)RAND_MAX) * (max-min));
}
inline float randomRange(Rangef range) {
	return randomRange(range.min, range.max);
}

inline float randomAngle() {
	return randomRange(0.0f,osg::PI*2.0f);
}

inline std::vector<float> randomRangeVector(const unsigned int& size, Rangef range){
	std::vector<float> vec;
	for(unsigned int i=0; i<size; i++)
		vec.push_back(randomRange(range));
	return vec;
}

class DataTexture : public osg::Texture2D
{
public:

	DataTexture(unsigned int arraySize, bool isFloat = true)
		: osg::Texture2D(),
		_arraySize(arraySize),
		_isFloat(isFloat)
	{
		int squared = ceil(sqrt((float)arraySize));
		int w = squared;
		init(w,w);
	}

	void fillTextureRandomRange(Rangef range) 
	{
		fillTextureRandomRange(range, range, range, range);
	}
	void fillTextureRandomRange(Rangef rangeR, Rangef rangeG, Rangef rangeB) 
	{
		fillTextureRandomRange(rangeR, rangeG, rangeB, Rangef(1.0,1.0));
	}
	void fillTextureRandomRange(Rangef rangeR, Rangef rangeG, Rangef rangeB, Rangef rangeA) 
	{
		osg::Image* image = new osg::Image();
		image->allocateImage(this->getTextureWidth(), this->getTextureHeight(), 1, GL_RGBA, _isFloat ? GL_FLOAT : GL_UNSIGNED_BYTE);
		this->setImage(0, image);

		for(int r=0; r<image->s(); ++r)
		{
			if(_isFloat)
			{
				float* data = (float*)image->data(0, r);
				for(int c=0; c<image->t(); ++c)
				{
					(*data) = randomRange(rangeR); ++data;
					(*data) = randomRange(rangeG); ++data;
					(*data) = randomRange(rangeB); ++data;
					(*data) = randomRange(rangeA); ++data;
				}
			}else{
				unsigned char* data = (unsigned char*)image->data(0, r);
				for(int c=0; c<image->t(); ++c)
				{
					(*data) = floor(randomRange(rangeR)*255.0f); ++data;
					(*data) = floor(randomRange(rangeG)*255.0f); ++data;
					(*data) = floor(randomRange(rangeB)*255.0f); ++data;
					(*data) = floor(randomRange(rangeA)*255.0f); ++data;
				}
			}
		}

		image->dirty();
	}

	osg::Vec2 computeColRowForIndex(const unsigned int& index) 
	{
		float x = (float)(index%this->getTextureWidth());
		float y = ceil((float)index/(float)this->getTextureWidth());
		return osg::Vec2(x,y);
	}

	osg::Vec2 computeUVForColRow(osg::Vec2 colRow)
	{
		float w = (float)this->getTextureWidth();
		float h = (float)this->getTextureHeight();
		osg::Vec2 texelSize = osg::Vec2(1.0f/w, 1.0f/h);
		float x = (colRow.x() / w) + (texelSize.x()*0.5f);
		float y = (colRow.y() / h) + (texelSize.y()*0.5f);
		return osg::Vec2(x,y);
	}

	osg::Vec2 computeClipSpaceForColRow(osg::Vec2 colRow)
	{
		osg::Vec2 uv = computeUVForColRow(colRow);
		float hw = floor((float)this->getTextureWidth()*0.5f);
		float hh = floor((float)this->getTextureHeight()*0.5f);
		float x = (uv.x() * 2.0f) - 1.0f;// (colRow.x() / hw) - 1.0f;
		float y = (uv.y() * 2.0f) - 1.0f;// (colRow.y() / hh) - 1.0f;
		return osg::Vec2(x,y);
	}

	//
	// Create a SpriteDataList array which maps individual sprites to individual pixels in our data texture 
	// the position (gl_Vertex.xy) is used to store the clip space pixel position we want to render to for output
	// the width/height (gl_MultiTexcoord0.xy) is to store the uv coord of the same pixel for input
	//
	osgSprites::Sprites::SpriteDataList createIndexDataList()
	{
		int cols = this->getTextureWidth();
		int rows = this->getTextureHeight();
		osgSprites::Sprites::SpriteDataList dataList;
		for(unsigned int i = 0; i < _arraySize; i++ )
		{
			osgSprites::Sprites::SpriteData spriteData;
			osg::Vec2 colRow = computeColRowForIndex(i);
			//clip space position for ensuring we render to the correct pixel for output
			spriteData.position = osg::Vec3(computeClipSpaceForColRow(colRow), 0.0f);
			//unit/tex coord location for ensuring we read from the correct pixel for input
			osg::Vec2 uvCoord = computeUVForColRow(colRow);
			spriteData.width = uvCoord.x();
			spriteData.height = uvCoord.y();
			dataList.push_back( spriteData );
		}
		return dataList;
	}

	//
	// Create an osgSprites array that will render verts to pixels allowing use to run our gpu program
	//
	osgSprites::Sprites* createOperationSprites(const std::string programPath, const std::vector<float>* userData = NULL) 
	{
		osgSprites::Sprites::SpriteDataList dataList = this->createIndexDataList();
		if(userData != NULL && userData->size() == _arraySize)
		{
			for(unsigned int i=0; i<userData->size(); i++)
				dataList[i].userData = (*userData)[i];
		}
		return new osgSprites::Sprites( dataList, programPath, false, osgSprites::Sprites::POINTS);
	}

	osg::Geode* createOperationQuad(const std::string programPath) 
	{
		int cols = this->getTextureWidth();
		int rows = this->getTextureHeight();

		osg::Vec4Array *verts = new osg::Vec4Array;
		osg::Vec2 bottomLeft = osg::Vec2(-1.0f,-1.0f);//this->computeClipSpaceForColRow(this->computeColRowForIndex(0));
		osg::Vec2 topRight = osg::Vec2(1.0f,1.0f);//this->computeClipSpaceForColRow(osg::Vec2(cols-1, rows-1));

		verts->push_back(osg::Vec4(bottomLeft.x(), bottomLeft.y(), 0.0f, 1.0f));
		verts->push_back(osg::Vec4(topRight.x(), bottomLeft.y(), 0.0f, 1.0f));
		verts->push_back(osg::Vec4(topRight.x(), topRight.y(), 0.0f, 1.0f));
		verts->push_back(osg::Vec4(bottomLeft.x(), topRight.y(), 0.0f, 1.0f));

		//
		osg::Vec4Array *coords = new osg::Vec4Array;
		osg::Vec2 bottomLeftUV = osg::Vec2(0.0f,0.0f);//this->computeUVForColRow(this->computeColRowForIndex(0));
		osg::Vec2 topRightUV = osg::Vec2(1.0f,1.0f);//this->computeUVForColRow(osg::Vec2(cols-1, rows-1));

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


	osgSprites::Sprites* createParticleRenderSprites(unsigned int spriteCount, Rangef sizeRange, const std::string& programPath) 
	{
		int cols = this->getTextureWidth();
		int rows = this->getTextureHeight();
		osgSprites::Sprites::SpriteDataList dataList;
		for(unsigned int i = 0; i < spriteCount; i++ )
		{
			osgSprites::Sprites::SpriteData spriteData;
		
			osg::Vec2 colRow = computeColRowForIndex(i);

			//stash uv coord for our positions data in the gl_vertex as texcoord0 is used for width/height data
			spriteData.position = osg::Vec3(computeUVForColRow(colRow), 0.0f);

			//unit/tex coord location
			spriteData.width = randomRange(sizeRange);
			spriteData.height = spriteData.width;

			spriteData.paletteIndex = rand()%4;
			spriteData.userData = randomRange(0.01f,1.0f);

			dataList.push_back( spriteData );
		}
		return new osgSprites::Sprites( dataList, programPath, false, osgSprites::Sprites::GEOMETRY_SHADER_SPRITES);
	}

protected:

	void init(const unsigned int& w, const unsigned int& h)
	{
		this->setTextureSize(w, h);
		this->setInternalFormat(_isFloat ? GL_RGBA32F_ARB : GL_RGBA);
		this->setSourceType(_isFloat ? GL_FLOAT : GL_UNSIGNED_BYTE);
		this->setFilter(osg::Texture2D::MIN_FILTER,osg::Texture2D::NEAREST);
		this->setFilter(osg::Texture2D::MAG_FILTER,osg::Texture2D::NEAREST);
		this->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE); 
		this->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE); 
		this->setResizeNonPowerOfTwoHint(false);
		this->setUnRefImageDataAfterApply(true);
	}

	unsigned int _arraySize;
	bool _isFloat;

};
typedef osg::ref_ptr<DataTexture> DataTextureRef;
typedef osg::ref_ptr<osg::Texture> TextureRef;


class ComputeNode : public osg::Switch
{
public:

	class FlipFlopCameras : public osg::NodeCallback 
    {
    public:
       virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
       {
          osg::ref_ptr<ComputeNode> computeNode = dynamic_cast<ComputeNode*>(node);
          if(computeNode)
          {
			  computeNode->swap();
          }
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
		osg::Geode* inputGeode = _input[0]->createOperationQuad(program+"-perfrag");

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

		this->setUpdateCallback(new FlipFlopCameras());
		this->setSingleChildOn(0);
	}

	void swap()
	{
		_flipFlop = !_flipFlop;
		this->setSingleChildOn(_flipFlop ? 0 : 1);
	}

	unsigned int _numChannels;
	std::vector< DataTextureRef > _input;
	std::vector< DataTextureRef > _output;
	bool _flipFlop;
};
