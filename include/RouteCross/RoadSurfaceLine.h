#pragma once
#include "../include/OSGBIM/OSGBIM.h"
class CRSF_Callback;

class ROUTECROSS_API CRoadSurfaceLine
{
public:
	CRoadSurfaceLine();
	~CRoadSurfaceLine();
public:
	osg::ref_ptr<osg::Vec3Array> GetPointArray(){ return pointArray; }
	osg::Group* GetRoot(){ return root; }
	void CalcVertices();
	CRSF_Callback* GetUpdateCallBack(){ return _callback; }

private:
	osg::ref_ptr<osg::Vec3Array> pointArray;
	osg::Group *root;
	CRSF_Callback *_callback;
};

class CRSF_Callback :public CComponentCallback
{
public:
	CRSF_Callback(CRoadSurfaceLine *RSL) :
	CComponentCallback(RSL->GetRoot()),m_RSL(RSL){
		m_model=new osg::Group;
	}
	~CRSF_Callback(){}
	osg::ref_ptr<osg::Group> GetModel() const { return m_model; }
	virtual void DoUpdate() override;
private:
	CRoadSurfaceLine *m_RSL;
	osg::ref_ptr<osg::Group> m_model;
};

