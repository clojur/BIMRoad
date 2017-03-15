#include "stdafx.h"
#include "GroundLine.h"


CGroundLine::CGroundLine()
{
	root = new osg::Group;
	points = new osg::Vec3Array;
	CalcVertices();
	_callback = new CGL_Callback(this);
}


CGroundLine::~CGroundLine()
{
}

float CGroundLine::GetZ(float x)
{
	return sinf(x) + 3.0f;
}

void CGroundLine::CalcVertices()
{
	for (float i = -8 * osg::PI; i <= 8 * osg::PI; i += osg::PI_4)
	{
		float x = i;
		float z = GetZ(x);
		GetPoints()->push_back(osg::Vec3(x, 0.0f, z));
	}
}


CGL_Callback::CGL_Callback(CGroundLine *GL) 
	:CComponentCallback(GL->GetRoot()),
	m_GL(GL)
{

}

CGL_Callback::~CGL_Callback()
{

}

void CGL_Callback::DoUpdate()
{
	osg::ref_ptr<osg::Geometry> gm = new osg::Geometry;
	osg::ref_ptr<osg::Geode> gd = new osg::Geode;
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	gm->setVertexArray(m_GL->GetPoints());
	gm->setColorArray(colors, osg::Array::BIND_OVERALL);
	gm->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,0,m_GL->GetPoints()->size()));
	gd->addDrawable(gm);
	_root->addChild(gd);
}
