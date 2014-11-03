#include <osgSprites/Sprites.h>

#include <osg/StateSet>
#include <osg/PointSprite>
#include <osg/Point>
#include <osg/BlendFunc>
#include <osgDB/FileUtils>


namespace osgSprites {


static const char *vertexShaderSource = 
    "void main()\n"
    "{\n"
    "    gl_FrontColor = gl_MultiTexCoord0;;\n"
    "    gl_Position = ftransform();\n"
    "}\n";


static const char *geometryShaderSource = 
    "#version 120 \n"
    "#extension GL_EXT_gpu_shader4 : enable\n"
    "#extension GL_EXT_geometry_shader4 : enable\n"
    "\n"
    "uniform int osgSprites_palette_rows;\n"
    "uniform int osgSprites_palette_cols;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "    if( osgSprites_palette_cols == 0 || osgSprites_palette_rows == 0 )\n"
    "        return;\n"
    "\n"
    "    float sdiv = 1.0/osgSprites_palette_cols;\n"
    "    float tdiv = 1.0/osgSprites_palette_rows;\n"
    "    for( int i=0; i< gl_VerticesIn; i++)\n"
    "    {\n"
    "        vec4 cc = gl_FrontColorIn[i];\n"
    "        float w   = cc[0];\n"
    "        float h   = cc[1];\n"
    "        int index =  int(cc[2]);\n"
    "\n"
    "        float s0 = (index%osgSprites_palette_cols) * sdiv;\n"
    "        float s1 = (index%osgSprites_palette_cols) * sdiv + sdiv;\n"
    "        float t0 = (index/osgSprites_palette_cols) * tdiv;\n"
    "        float t1 = (index/osgSprites_palette_cols) * tdiv + tdiv;\n"
    "\n"
    "        vec4 p0 = gl_PositionIn[i];\n"
    "        p0.x -= w*0.5;\n"
    "        vec4 p1 = vec4( p0.x + w, p0.yzw );\n"
    "        vec4 p2 = vec4( p0.x, p0.y + h, p0.zw );\n"
    "        vec4 p3 = vec4( p1.x, p2.y, p0.zw );\n"
    "\n"
    "        gl_TexCoord[0] =  vec4( s0, t0, 0.0, 1.0 );\n"
    "        gl_Position = p0;\n"
    "        EmitVertex();\n"
    "\n"
    "        gl_TexCoord[0] =  vec4( s1, t0, 0.0, 1.0 );\n"
    "        gl_Position = p1;\n"
    "        EmitVertex();\n"
    "\n"
    "        gl_TexCoord[0] =  vec4( s0, t1, 0.0, 1.0 );\n"
    "        gl_Position = p2;\n"
    "        EmitVertex();\n"
    "\n"
    "        gl_TexCoord[0] =  vec4( s1, t1, 0.0, 1.0 );\n"
    "        gl_Position = p3;\n"
    "        EmitVertex();\n"
    "    }\n"
    "    EndPrimitive();\n"
    "}\n"
    ;


static const char *fragmentShaderSource = 
    "uniform sampler2D osgSprites_tex0;\n"
	"uniform float osgSprites_clipTolerance;\n"
    "void main()\n"
    "{\n"
    "    vec4 rgba = texture2D( osgSprites_tex0, gl_TexCoord[0].st );\n"
    "    if( rgba.a <= osgSprites_clipTolerance )\n"
    "        discard;\n"
    "    else\n"
    "        gl_FragColor = rgba;\n"
    "}\n";


Sprites::Sprites(const std::string &shaderFile, const RenderMode &renderMode)
	: osg::Geode(),
	_renderMode(renderMode)
{
    _init(shaderFile);
}

Sprites::Sprites(const Sprites& sprite, const osg::CopyOp& op)
	: osg::Geode(sprite, op),
	_renderMode(sprite._renderMode)
{
}

Sprites::Sprites( const SpriteDataList &l, const std::string &shaderFile, const bool& useUpVector, const RenderMode &renderMode)
	: osg::Geode(),
	_renderMode(renderMode)
{
    setSpriteList( l, useUpVector );
    _init(shaderFile);
}


void Sprites::setTexturePalette( TexturePalette *texturePalette )
{
    _texturePalette = texturePalette;

    osg::StateSet *sset = getOrCreateStateSet();

    sset->setTextureAttribute( 0, _texturePalette.get() );
    sset->addUniform( new osg::Uniform( "osgSprites_tex0", 0 ) );
    sset->addUniform( new osg::Uniform( "osgSprites_palette_rows", (int)_texturePalette->getNumRows() ) );
    sset->addUniform( new osg::Uniform( "osgSprites_palette_cols", (int)_texturePalette->getNumCols() ) );
}

TexturePalette *Sprites::getTexturePalette() const
{
    return _texturePalette.get();
}


void Sprites::setSpriteList( const SpriteDataList &l, const bool &useUpVector )
{
	_useUpVector = useUpVector;

    osg::Vec3Array *coords = new osg::Vec3Array;
	osg::Vec3Array *ups = new osg::Vec3Array;
    osg::Vec4Array *data = new osg::Vec4Array;

    for( osgSprites::Sprites::SpriteDataList::const_iterator p = l.begin(); p != l.end(); p++ )
    {
        coords->push_back(  p->position );
		data->push_back( osg::Vec4f( p->width, p->height, float(p->paletteIndex), p->userData ) );
		if(_useUpVector && _renderMode == GEOMETRY_SHADER_SPRITES)
			ups->push_back(p->up);
    }

    _spriteDataList = l;
    _numSprites = _spriteDataList.size();

	OSG_INFO << "Sprites: Num Sprites: " << _numSprites << std::endl;

    osg::Geometry *geom = new osg::Geometry;
    geom->setVertexArray( coords );
	geom->setTexCoordArray(0, data);
	if(_useUpVector && _renderMode == GEOMETRY_SHADER_SPRITES)
		geom->setTexCoordArray(1, ups);
    geom->addPrimitiveSet( new osg::DrawArrays( GL_POINTS, 0, _numSprites ));

    if( getNumDrawables() > 0 )
        replaceDrawable( getDrawable(0), geom );
    else
        addDrawable( geom );

}

const Sprites::SpriteDataList &Sprites::getSpriteList() const
{
    return _spriteDataList;
}

void Sprites::setClipTolerance(float clipTolerance)
{
	_clipTolerance->set(clipTolerance);
}

float Sprites::getClipTolerance()
{
	float tolerance = 0.0f;
	_clipTolerance->get(tolerance);
	return tolerance;
}

void Sprites::setTransparencyActive(bool isTransparent)
{
	osg::StateSet *sset = getOrCreateStateSet();
	osg::BlendFunc *func = new osg::BlendFunc(); 
	func->setFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	sset->setAttributeAndModes(func);

	sset->setMode(GL_BLEND, isTransparent ? osg::StateAttribute::ON : osg::StateAttribute::OFF); 
}

void Sprites::_init(const std::string &shaderFile)
{
	std::string defaultShader = _renderMode == GEOMETRY_SHADER_SPRITES ? "data/sprites" : "data/pointsprites";
	std::string shaderBase = shaderFile.empty() ? defaultShader : shaderFile;
    
	osg::StateSet *sset = getOrCreateStateSet();
    osg::Program* program = new osg::Program;
    sset->setAttribute(program);

	//create clip tolerance uniform
	_clipTolerance = new osg::Uniform("osgSprites_clipTolerance", 0.2f);
	sset->addUniform(_clipTolerance);

	//load vert shader
    std::string vertShaderFile = osgDB::findDataFile( shaderBase + ".vert" );
    if( !vertShaderFile.empty() )
        program->addShader(osg::Shader::readShaderFile(osg::Shader::VERTEX, vertShaderFile ) );
    else {
        program->addShader(new osg::Shader(osg::Shader::VERTEX, vertexShaderSource));
		OSG_WARN << "osgSprites::_init: Failed to find .vert shader '" << shaderBase << ".vert' using default" << std::endl;
	}

	//load frag shader
    std::string fragShaderFile = osgDB::findDataFile( shaderBase + ".frag" );
    if( !fragShaderFile.empty() )
        program->addShader(osg::Shader::readShaderFile(osg::Shader::FRAGMENT, fragShaderFile ) );
    else {
        program->addShader(new osg::Shader(osg::Shader::FRAGMENT, fragmentShaderSource));
		OSG_WARN << "osgSprites::_init: Failed to find .frag shader '" << shaderBase << ".frag' using default" << std::endl;

	}

	//load geom shader if using geometry shader rendering mode
	if(_renderMode == GEOMETRY_SHADER_SPRITES)
	{
		std::string geomShaderFile = osgDB::findDataFile(shaderBase + ".geom");
		if( !geomShaderFile.empty() )
			program->addShader(osg::Shader::readShaderFile(osg::Shader::GEOMETRY, geomShaderFile ));
		else {
			program->addShader(new osg::Shader(osg::Shader::GEOMETRY, geometryShaderSource));
			OSG_WARN << "osgSprites::_init: Failed to find .geom shader '" << shaderBase << ".geom' using default" << std::endl;
		}
		//program->addBindAttribLocation("_spriteData", 5);
		program->setParameter( GL_GEOMETRY_INPUT_TYPE_EXT, GL_POINTS );
		program->setParameter( GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_QUADS );
		program->setParameter( GL_GEOMETRY_VERTICES_OUT_EXT, 4 );

	}else if(_renderMode == POINT_SPRITES) {
		/// Setup the point sprites
		osg::PointSprite *sprite = new osg::PointSprite();
		sset->setTextureAttributeAndModes(0, sprite, osg::StateAttribute::ON);
		sset->setMode(GL_VERTEX_PROGRAM_POINT_SIZE, osg::StateAttribute::ON);
	}
}

}
