/* @License 
 -------------------------------------------------------------------------------
 | Chaskii - Copyright (C) 2010  Don Burns, Andes Computer Engineering, LLC    |
 |                                                                             |
 | This library is free software; you can redistribute it and/or modify        |
 | it under the terms of the GNU Lesser General Public License as published    |
 | by the Free Software Foundation; either version 3 of the License, or        |
 | (at your option) any later version.                                         |
 |                                                                             |
 | This library is distributed in the hope that it will be useful, but         |
 | WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY  |
 | or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public     |
 | License for more details.                                                   |
 |                                                                             |
 | You should have received a copy of the GNU Lesser General Public License    |
 | along with this software; if not, write to the Free Software Foundation,    |
 | Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.               |
 ---------------------------------------------------------------------------- */
#ifndef POSITION_ELEMENT_DEF
#define POSITION_ELEMENT_DEF 1

#include <Chaskii/XML/Element.h>
#include <osg/Vec3f>

class PositionElement: public iiXML::Element
{
    public:
        PositionElement( const osg::Vec3f &p ):
            _p(p)  {}

        virtual const char *elementName() { return "Position"; }
        osg::Vec3f getPosition() { return _p; }
    private:
        osg::Vec3f _p;
};
#endif
