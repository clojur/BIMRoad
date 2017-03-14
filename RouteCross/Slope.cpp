#include "stdafx.h"
#include "Slope.h"


CSlope::CSlope()
{
	m_RSL = nullptr;
	m_GL = nullptr;
}

CSlope::CSlope(CRoadSurfaceLine* RSL, CGroundLine* GL,SlopeStruce Slopemem) :
m_GL(GL), 
m_RSL(RSL),
m_Slopemem(Slopemem)
{
	m_Lpoints = new osg::Vec3Array;
	m_Rpoints = new osg::Vec3Array;
	root = new osg::Group;
	_callback=new CSlope_Callback(this);
	CalcLeftSlope();
	CalcRightSlope();
}

CSlope::~CSlope()
{

}

void CSlope::CalcLeftSlope()
{
	m_Lpoints->push_back(m_RSL->GetPointArray()->at(0));
	m_Left_RSLToGLDistance = m_Lpoints->at(0).z() - m_GL->GetZ(m_Lpoints->at(0).x());

	if (m_Slopemem.m_LIsZhi)
	{
		osg::Vec3 L_endPoint;
		//可能要另外开线程优化
		double deltaL =0.0f ;
		double deltaH = 0.0f;
		double groundX = m_Lpoints->at(0).x();
		double groundZ = m_Lpoints->at(0).z();
		bool ms = m_Left_RSLToGLDistance<0.00001f&&m_Left_RSLToGLDistance>-0.00001f;
		double range;
		while (!ms)
		{
			deltaL += 0.1;
			deltaH = m_Slopemem.m_Lgradient*deltaL;
			groundX -= deltaL;
			if (m_Left_RSLToGLDistance > 0.0f)
				groundZ -= deltaH;
			else
				groundZ += deltaH;

			range = abs(groundZ - m_GL->GetZ(groundX));
			if (range <= 0.001f)
			{
				m_Lpoints->push_back(osg::Vec3(groundX, 0.0f, groundZ));
				break;
			}
		}
	}
	else
	{
		
	}
	
}

void CSlope::CalcRightSlope()
{
	unsigned int index = m_RSL->GetPointArray()->size();
	m_Rpoints->push_back(m_RSL->GetPointArray()->at(index));
	osg::Vec3 R_endPoint;
}

void CSlope_Callback::DoUpdate()
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	osg::ref_ptr<osg::Geometry> gm = new osg::Geometry;
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(0.0f,0.0f,1.0f,1.0f));
	gm->setVertexArray(m_slp->m_Lpoints);
	gm->setColorArray(colors, osg::Array::BIND_OVERALL);
	gm->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,m_slp->m_Lpoints->size()));
	geode->addDrawable(gm);
	_root->addChild(geode);
}
