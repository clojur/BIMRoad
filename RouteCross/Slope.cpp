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
		//可能要另外开线程优化
		CalcZhi(SlopePos::LEFT,m_Lpoints, m_Left_RSLToGLDistance, m_Slopemem.m_Lgradient);
	}
	else
	{
		CalcTong(SlopePos::LEFT, m_Lpoints, m_Left_RSLToGLDistance, m_Slopemem.m_Lgradient,m_Slopemem.m_Lheight);
	}


}

void CSlope::CalcRightSlope()
{
	unsigned int index = m_RSL->GetPointArray()->size();
	m_Rpoints->push_back(m_RSL->GetPointArray()->at(index-1));
	m_Right_RSLToGLDistance = m_Rpoints->at(0).z() - m_GL->GetZ(m_Rpoints->at(0).x());
	if (m_Slopemem.m_RIsZhi)
	{
		CalcZhi(SlopePos::RIGHT, m_Rpoints, m_Right_RSLToGLDistance, m_Slopemem.m_Rgradient);
	}
	else
	{
		CalcTong(SlopePos::RIGHT, m_Rpoints, m_Right_RSLToGLDistance, m_Slopemem.m_Rgradient, m_Slopemem.m_Rheight);
	}
}

osg::Vec3 CSlope::CalcTwoLineInsectPoint(double x1, double z1, double x2, double z2)
{
	osg::Vec3 result;
	double b1 = z2- z1;
	double c1 = x2 - x1;

	double b2 = m_GL->GetZ(x2) - m_GL->GetZ(x1);
	double c2 = x2 - x1;
	double x3 = x1;
	double z3 = m_GL->GetZ(x3);

	double xL = ((b1 / c1)*x1) - ((b2 / c2)*x3) - z1 + z3;
	double x = xL / ((b1 / c1) - (b2 / c2));
	double z = m_GL->GetZ(x);
	result=osg::Vec3(x, 0.0f, z);
	return result;
}

void CSlope::CalcZhi(SlopePos slopePos, osg::ref_ptr<osg::Vec3Array> points, double RtoD_distance, double gradient)
{
	double currentX = points->at(0).x();
	double currentZ = points->at(0).z();
	double oldZ = 0.1f;
	double oldX = 0.1f;
	double CurrentDeltaZ = 0.1f;
	double OldDeltaZ = 0.1f;
	bool IsInsect = RtoD_distance<0.001f&&RtoD_distance>-0.001f;
	double range = 5.0f;
	while (!IsInsect)
	{
		if (slopePos==SlopePos::LEFT)
			currentX -= 0.5f;
		if (slopePos == SlopePos::RIGHT)
			currentX += 0.5f;
		if (RtoD_distance > 0.0f)
			currentZ -= 0.5*gradient;
		else
			currentZ += 0.5*gradient;

		CurrentDeltaZ = (currentZ - m_GL->GetZ(currentX));
		if (CurrentDeltaZ*OldDeltaZ > 0.0f)
		{
			oldX = currentX;
			oldZ = currentZ;
			OldDeltaZ = currentZ - m_GL->GetZ(currentX);
			continue;
		}
		else
		{
			osg::Vec3 L_EndPoint = CalcTwoLineInsectPoint(oldX, oldZ, currentX, currentZ);
			points->push_back(L_EndPoint);
			break;
		}
	}
}

void CSlope::CalcTong(SlopePos slopePos, osg::ref_ptr<osg::Vec3Array> points, double RtoD_distance, double gradient, double slope_height)
{
	double currentX = points->at(0).x();
	double currentZ = points->at(0).z();
	double oldZ = 0.1f;
	double oldX = 0.1f;
	double CurrentDeltaZ = 0.1f;
	double OldDeltaZ = 0.1f;
	bool IsInsect = RtoD_distance<0.001f&&RtoD_distance>-0.001f;
	double range = 5.0f;
	while (!IsInsect)
	{
		if (slopePos == SlopePos::LEFT)
			currentX -= slope_height/gradient;
		else if (slopePos == SlopePos::RIGHT)
			currentX += slope_height / gradient;

		if (RtoD_distance > 0.0f)
			currentZ -= slope_height;
		else
			currentZ += slope_height;
		

		CurrentDeltaZ = (currentZ - m_GL->GetZ(currentX));
		if (CurrentDeltaZ*OldDeltaZ > 0.0f)
		{
			points->push_back(osg::Vec3(currentX,0.0f,currentZ));
			oldX = currentX;
			oldZ = currentZ;
			OldDeltaZ = currentZ - m_GL->GetZ(currentX);
			if (slopePos == SlopePos::LEFT)
			{
				points->push_back(osg::Vec3(currentX-m_Slopemem.m_LstepLength,0.0f,currentZ));
			}
			else if (slopePos == SlopePos::RIGHT)
			{
				points->push_back(osg::Vec3(currentX+m_Slopemem.m_RstepLength,0.0f,currentZ));
			}
			continue;
		}
		else
		{
			osg::Vec3 R_EndPoint = CalcTwoLineInsectPoint(oldX, oldZ, currentX, currentZ);
			points->push_back(R_EndPoint);
			break;
		}
	}
}

void CSlope_Callback::DoUpdate()
{
	m_slp->CalcLeftSlope();
	m_slp->CalcRightSlope();
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	osg::ref_ptr<osg::Geometry> gmL = new osg::Geometry;
	osg::ref_ptr<osg::Geometry> gmR = new osg::Geometry;
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0f,0.0f,1.0f,1.0f));
	gmL->setVertexArray(m_slp->m_Lpoints);
	gmL->setColorArray(colors, osg::Array::BIND_OVERALL);
	gmL->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,0,m_slp->m_Lpoints->size()));

	gmR->setVertexArray(m_slp->m_Rpoints);
	gmR->setColorArray(colors,osg::Array::BIND_OVERALL);
	gmR->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,0,m_slp->m_Rpoints->size()));
	geode->addDrawable(gmL);
	geode->addDrawable(gmR);
	_root->addChild(geode);
}
