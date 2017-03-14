// SceneNode.h : SceneNode DLL 的主头文件
//

#pragma once
#include "macro.h"
#include "../include/OSGBIM/OSGBIM.h"
#include "../include/RouteCross/RouteCross.h"

class SCENENODE_API SceneNode
{
public:
	SceneNode();
	~SceneNode();
	osg::Group* GetRoot(){ return m_root; }
private:
	osg::Group *m_root;
	CRoadSurfaceLine *m_RSL;
	CGroundLine *m_GL;
	CSlope *m_sp;
};

