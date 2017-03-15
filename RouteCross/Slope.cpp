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
	//CalcLeftSlope();
	//CalcRightSlope();
	_callback=new CSlope_Callback(this);
	
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
		double currentX = m_Lpoints->at(0).x();
		double currentZ = m_Lpoints->at(0).z();
		double deltaZ=0.1f;
		double oldZ = 0.1f;
		double oldX = 0.1f;
		bool ms = m_Left_RSLToGLDistance<0.00001f&&m_Left_RSLToGLDistance>-0.00001f;
		double range=5.0f;
		while (!ms)
		{
			currentX -= 0.5f;
			if (m_Left_RSLToGLDistance > 0.0f)
				currentZ -= 0.5*m_Slopemem.m_Lgradient;
			else
				currentZ += 0.5*m_Slopemem.m_Lgradient;

			if (deltaZ*(currentZ - m_GL->GetZ(currentX)) > 0.0f)
			{
				oldX = currentX;
				oldZ = currentZ;
				deltaZ = currentZ - m_GL->GetZ(currentX);
				continue;
			}
			else
			{
				CalcTwoLineInsectPoint(oldX, oldZ, currentX, currentZ);
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

void CSlope::CalcTwoLineInsectPoint(double x1, double z1, double x2, double z2)
{
	double b1 = z2- z1;
	double c1 = x2 - x1;

	double b2 = m_GL->GetZ(x2) - m_GL->GetZ(x1);
	double c2 = x2 - x1;
	double x3 = x1;
	double z3 = m_GL->GetZ(x3);

	double xL = ((b1 / c1)*x1) - ((b2 / c2)*x3) - z1 + z3;
	double x = xL / ((b1 / c1) - (b2 / c2));
	double z = m_GL->GetZ(x);
	m_Lpoints->push_back(osg::Vec3(x, 0.0f, z));
}

void CSlope_Callback::DoUpdate()
{
	m_slp->CalcLeftSlope();
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	osg::ref_ptr<osg::Geometry> gm = new osg::Geometry;
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0f,0.0f,1.0f,1.0f));
	gm->setVertexArray(m_slp->m_Lpoints);
	gm->setColorArray(colors, osg::Array::BIND_OVERALL);
	gm->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,m_slp->m_Lpoints->size()));
	geode->addDrawable(gm);
	_root->addChild(geode);
}
