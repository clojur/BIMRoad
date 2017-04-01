#pragma once
#include "../include/OSGBIM/OSGBIM.h"
class CGL_Callback;
class ROUTECROSS_API CGroundLine
{
public:
	CGroundLine();
	~CGroundLine();
	osg::Group* GetRoot(){ return root; };
	osg::ref_ptr<osg::Vec3Array> GetPoints(){ return points; }
	void CalcVertices();
	CGL_Callback* GetUpdateCallBack(){ return _callback; }
	float GetZ(float x);
private:
	osg::Group *root;
	CGL_Callback *_callback;
	osg::ref_ptr<osg::Vec3Array> points;
};

class CGL_Callback :public CComponentCallback
{
public:
	CGL_Callback(CGroundLine *GL);
	~CGL_Callback();
	virtual void DoUpdate() override;
private:
	CGroundLine *m_GL;
};

