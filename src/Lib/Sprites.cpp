#include <osgDB/FileUtils>
#include <osg/StateSet>
#include <osgSprites/Sprites.h>

namespace osgSprites {


static const char *vertexShaderSource = 
    "void main()\n"
    "{\n"
    "    gl_FrontColor = gl_Color;\n"
    "    gl_Position = ftransform();\n"
    "}\n";


static const char *geometryShaderSource = 
    "#version 120 \n"
    "#extension GL_EXT_gpu_shader4 : enable\n"
    "#extension GL_EXT_geometry_shader4 : enable\n"
    "\n"
    "uniform int palette_rows;\n"
    "uniform int palette_cols;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "    if( palette_cols == 0 || palette_rows == 0 )\n"
    "        return;\n"
    "\n"
    "    float sdiv = 1.0/palette_cols;\n"
    "    float tdiv = 1.0/palette_rows;\n"
    "    for( int i=0; i< gl_VerticesIn; i++)\n"
    "    {\n"
    "        vec4 cc = gl_FrontColorIn[i];\n"
    "        float w   = cc[0];\n"
    "        float h   = cc[1];\n"
    "        int index =  int(cc[2]);\n"
    "\n"
    "        float s0 = (index%palette_cols) * sdiv;\n"
    "        float s1 = (index%palette_cols) * sdiv + sdiv;\n"
    "        float t0 = (index/palette_cols) * tdiv;\n"
    "        float t1 = (index/palette_cols) * tdiv + tdiv;\n"
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
    "uniform sampler2D tex0;\n"
    "void main()\n"
    "{\n"
    "    vec4 rgba = texture2D( tex0, gl_TexCoord[0].st );\n"
    "    if( rgba.a <= 0.58 )\n"
    "        discard;\n"
    "    else\n"
    "        gl_FragColor = rgba;\n"
    "}\n";


Sprites::Sprites()
{
    _init();
}

Sprites::Sprites(const Sprites&, const osg::CopyOp&)
{
}

Sprites::Sprites( const SpriteDataList &l )
{
    setSpriteList( l );
    _init();
}


void Sprites::setTexturePalette( TexturePalette *texturePalette )
{
    _texturePalette = texturePalette;

    osg::StateSet *sset = getOrCreateStateSet();

    sset->setTextureAttribute( 0, _texturePalette.get() );
    sset->addUniform( new osg::Uniform( "tex0", 0 ) );
    sset->addUniform( new osg::Uniform( "palette_rows", (int)_texturePalette->getNumRows() ) );
    sset->addUniform( new osg::Uniform( "palette_cols", (int)_texturePalette->getNumCols() ) );
}

TexturePalette *Sprites::getTexturePalette() const
{
    return _texturePalette.get();
}


void Sprites::setSpriteList( const SpriteDataList &l )
{
    osg::Vec3Array *coords = new osg::Vec3Array;
    osg::Vec3Array *data = new osg::Vec3Array;

    for( osgSprites::Sprites::SpriteDataList::const_iterator p = l.begin(); p != l.end(); p++ )
    {
        coords->push_back(  p->position );
        data->push_back( osg::Vec3f( p->width, p->height, float(p->paletteIndex) ) );
    }

    _spriteDataList = l;
    _numSprites = _spriteDataList.size();

    osg::Geometry *geom = new osg::Geometry;
    geom->setVertexArray( coords );
    geom->setColorArray( data );
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

void Sprites::_init()
{
    osg::StateSet *sset = getOrCreateStateSet();
    osg::Program* program = new osg::Program;
    sset->setAttribute(program);

    std::string vertShaderFile = osgDB::findDataFile( "sprites.vert" );
    if( !vertShaderFile.empty() )
        program->addShader(osg::Shader::readShaderFile(osg::Shader::VERTEX, vertShaderFile ) );
    else
        program->addShader(new osg::Shader(osg::Shader::VERTEX, vertexShaderSource));

    std::string fragShaderFile = osgDB::findDataFile( "sprites.frag" );
    if( !fragShaderFile.empty() )
        program->addShader(osg::Shader::readShaderFile(osg::Shader::FRAGMENT, fragShaderFile ) );
    else
        program->addShader(new osg::Shader(osg::Shader::FRAGMENT, fragmentShaderSource));

    std::string geomShaderFile = osgDB::findDataFile("sprites.geom");
    if( !geomShaderFile.empty() )
        program->addShader(osg::Shader::readShaderFile(osg::Shader::GEOMETRY, geomShaderFile ));
    else
        program->addShader(new osg::Shader(osg::Shader::GEOMETRY, geometryShaderSource));

    program->setParameter( GL_GEOMETRY_INPUT_TYPE_EXT, GL_POINTS );
    program->setParameter( GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_QUADS );
    program->setParameter( GL_GEOMETRY_VERTICES_OUT_EXT, 4 );
}

}
