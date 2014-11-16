#pragma once

#include <osg/Texture2D>
#include "Utils.h"

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


	void fillTexture(std::vector<osg::Vec4> fillData) 
	{
		osg::Image* image = new osg::Image();
		image->allocateImage(this->getTextureWidth(), this->getTextureHeight(), 1, GL_RGBA, _isFloat ? GL_FLOAT : GL_UNSIGNED_BYTE);
		this->setImage(0, image);
		int i=0;
		for(int r=0; r<image->s() && i<fillData.size()-1; ++r)
		{
			if(_isFloat)
			{
				float* data = (float*)image->data(0, r);
				for(int c=0; c<image->t() && i<fillData.size()-1; ++c)
				{
					(*data) = fillData[i].x(); ++data;
					(*data) = fillData[i].y(); ++data;
					(*data) = fillData[i].z(); ++data;
					(*data) = fillData[i].w(); ++data;
					i++;
				}
			}else{
				unsigned char* data = (unsigned char*)image->data(0, r);
				for(int c=0; c<image->t() && i<fillData.size()-1; ++c)
				{
					(*data) = floor(fillData[i].x()*255.0f); ++data;
					(*data) = floor(fillData[i].y()*255.0f); ++data;
					(*data) = floor(fillData[i].z()*255.0f); ++data;
					(*data) = floor(fillData[i].w()*255.0f); ++data;
					i++;
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