#pragma once
#include "../include/OSGBIM/OSGBIM.h"
#include "GroundLine.h"
#include "RoadSurfaceLine.h"

class CSlope_Callback;
class ROUTECROSS_API CSlope
{
public:
	struct SlopeStruce
	{
		double m_Rgradient;
		double m_Lgradient;
		double m_Rheight;
		double m_Lheight;
		double m_LstepLength;
		double m_RstepLength;
		unsigned int m_LSlopeCount;
		unsigned int m_RSlopeCount;
		bool m_LIsZhi;
		bool m_RIsZhi;
	};
	enum class SlopePos{
		LEFT,RIGHT
	};
public:
	CSlope();
	CSlope(CRoadSurfaceLine* RSL, CGroundLine* GL,SlopeStruce Slopemem);
	~CSlope();
	CSlope_Callback* GetUpdateCallBack(){ return _callback; }
	osg::Group* GetRoot(){ return root; }

	/*±ßÆÂ¼ÆËã*/
	void CalcLeftSlope();
	void CalcRightSlope();
	osg::Vec3 CalcTwoLineInsectPoint(double tempX, double tempZ, double groundX, double groundZ);
	void CalcZhi(SlopePos slopePos,osg::ref_ptr<osg::Vec3Array> points,double RtoD_distance,double gradient);
	void CalcTong(SlopePos slopePos, osg::ref_ptr<osg::Vec3Array> points, double RtoD_distance, double gradient, double slope_height);
public:
	CGroundLine *m_GL;
	CRoadSurfaceLine *m_RSL;
	osg::ref_ptr<osg::Vec3Array> m_Lpoints;
	osg::ref_ptr<osg::Vec3Array> m_Rpoints;
	double m_Left_RSLToGLDistance;
	double m_Right_RSLToGLDistance;
	SlopeStruce m_Slopemem;
private:
	CSlope_Callback *_callback;
	osg::Group* root;
};

class CSlope_Callback :public CComponentCallback
{
public:
	CSlope_Callback(CSlope *slp):
		 CComponentCallback(slp->GetRoot()),
		 m_slp(slp)
		{
	
	}
	~CSlope_Callback(){}
	virtual void DoUpdate() override;
private:
	CSlope *m_slp;
};