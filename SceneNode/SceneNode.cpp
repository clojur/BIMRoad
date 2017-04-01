// SceneNode.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "SceneNode.h"

SceneNode::SceneNode()
{
	m_root = new osg::Group;
	m_GL = new CGroundLine;
	m_RSL = new CRoadSurfaceLine; 
	

	CSlope::SlopeStruce slopeS;
	slopeS = { 0 };
	slopeS.m_Lgradient = 0.8f;
	slopeS.m_LIsZhi = true;

	slopeS.m_Rgradient = 0.86;
	slopeS.m_Rheight = 4.0;
	slopeS.m_RstepLength = 2.0f;
	slopeS.m_RIsZhi = false;

	m_sp = new CSlope(m_RSL, m_GL, slopeS);
	
	m_root->addChild(m_GL->GetUpdateCallBack()->getRoot());
	m_root->addChild(m_RSL->GetUpdateCallBack()->getRoot());
	m_root->addChild(m_sp->GetUpdateCallBack()->getRoot());
	m_root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
}

SceneNode::~SceneNode()
{
	delete m_GL;
	delete m_RSL;
	delete m_sp;
}
