#include "stdafx.h"
#include "RoadSurfaceLine.h"

CRoadSurfaceLine::CRoadSurfaceLine()
{
	this->root = new osg::Group;
	this->pointArray = new osg::Vec3Array;
	CalcVertices();
	_callback = new CRSF_Callback(this);
}


CRoadSurfaceLine::~CRoadSurfaceLine()
{
}

void CRoadSurfaceLine::CalcVertices()
{
	pointArray->push_back(osg::Vec3(-2 * osg::PI, 0.0f, 15.0f));
	pointArray->push_back(osg::Vec3(2 * osg::PI, 0.0f, 15.0f));
}

void CRSF_Callback::DoUpdate()
{
	osg::ref_ptr<osg::Geometry> gm = new osg::Geometry;
	osg::ref_ptr<osg::Geode> gd = new osg::Geode;
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0f,0.0f,0.0f,1.0f));
	gm->setVertexArray(m_RSL->GetPointArray());
	gm->setColorArray(colors, osg::Array::Binding::BIND_OVERALL);
	gm->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,2));
	gd->addDrawable(gm);
	//osg::Node *node = osgDB::readNodeFile("glider.osg");
	_root->addChild(gd);
	m_model->addChild(gd);
	//osg::ref_ptr<osgGA::GUIEventAdapter> dummyEvent = _view->getEventQueue()->createEvent();
	//_view->getCameraManipulator()->home(*dummyEvent, *_view);
}