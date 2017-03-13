#include "stdafx.h"
#include "CViewManager.h"
#include "CMovePick.h"
#include "CComponentCallback.h"
#include "CoordAxisCallback.h"

void CViewManager::SetSceneData(HWND hWnd, CComponentCallback* cb)
{
	osg::Node* node = cb->getRoot();
	SetSceneData(hWnd, node);//将Node设置给与hWnd相关的viewSceneData
	cb->setView(_viewMap[hWnd]);
}

void CViewManager::SetSceneData(HWND hWnd, osg::Node* node)
{
	osg::Group* root = dynamic_cast<osg::Group*>(_viewMap[hWnd]->getSceneData());
	if (_rootIndex[hWnd])
		root->removeChild(_rootIndex[hWnd]);
	_rootIndex[hWnd] = root->getNumChildren();
	root->addChild(node);
}

osg::Node* CViewManager::GetSceneData(HWND hWnd)
{
	return dynamic_cast<osg::Group*>(_viewMap[hWnd]->getSceneData())->getChild(_rootIndex[hWnd]);
}

void CViewManager::RemoveNodeByName(const char *name, HWND hWnd)
{
	osg::Node* node = this->_viewMap[hWnd]->getSceneData();
	osg::Group* root = dynamic_cast<osg::Group*>(node);
	unsigned int nodeCount = root->getNumChildren();
	for (unsigned int i = 0; i < nodeCount; ++i)
	{
		if (root->getChild(i)->getName() == name)
			root->removeChild(root->getChild(i));
	}
}

void CViewManager::RemoveAllNode(HWND hWnd)
{
	osg::Node* node = this->_viewMap[hWnd]->getSceneData();
	osg::Group* root = dynamic_cast<osg::Group*>(node);
	unsigned int nodeCount = root->getNumChildren();
	for (unsigned int i = 0; i < nodeCount; ++i)
	{
		root->removeChild(root->getChild(0));
	}
}

void CViewManager::InitView(HWND hWnd, osgViewer::View* view)
{
	osg::ref_ptr<osg::Group> root = new osg::Group;
	view->setSceneData(root);
	InitCameraConfig(hWnd, view);
	//为HUD加入动态Text
	osg::ref_ptr<osgText::Text> updateText = new osgText::Text;
	//增加HUD
	root->addChild(CreateTextHUD(updateText.get()));
	//创建光照
	CreateLight(root);
	//增加坐标轴相机
	root->addChild(CreateCoordCamera(view->getCamera()));
	//实时获取里程高程HUD
	view->addEventHandler(new CMovePick(updateText.get(), root));

	//优化root
	osgUtil::Optimizer opt;
	opt.optimize(root.get());
	opt.reset();
}

void CViewManager::InitCameraConfig(HWND hWnd, osgViewer::View* view)
{
	RECT rect;
	::GetWindowRect(hWnd, &rect);
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	osg::ref_ptr<osg::Referenced> windata = new osgViewer::GraphicsWindowWin32::WindowData(hWnd);
	traits->x = 0;
	traits->y = 0;
	traits->width = rect.right - rect.left;
	traits->height = rect.bottom - rect.top;
	traits->doubleBuffer = true;
	traits->windowDecoration = false;
	traits->sharedContext = 0;
	traits->setInheritedWindowPixelFormat = true;
	traits->inheritedWindowData = windata;
	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits);
	osg::ref_ptr <osg::Camera> masterCamera = view->getCamera();
	masterCamera->setGraphicsContext(gc);
	masterCamera->setViewport(0, 0, traits->width, traits->height);
	masterCamera->setProjectionMatrixAsPerspective(45.0f, static_cast<double>(traits->width) / static_cast<double>(traits->height), 0.1f, 1000.0f);
	masterCamera->setNearFarRatio(0.000001f);

	view->setCamera(masterCamera);
	//view->setCameraManipulator(new osgGA::TrackballManipulator);
	view->addEventHandler(new osgGA::StateSetManipulator(view->getSceneData()->getOrCreateStateSet()));
	view->addEventHandler(new osgViewer::WindowSizeHandler);
	view->addEventHandler(new osgViewer::StatsHandler);
	view->getCamera()->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_PRIMITIVES);
	view->getCamera()->setNearFarRatio(0.000003f);

	//mRoot抗锯齿
	osg::ref_ptr<osg::Multisample> pms = new osg::Multisample;
	pms->setSampleCoverage(1, true);
	view->getSceneData()->getOrCreateStateSet()->setAttributeAndModes(pms, osg::StateAttribute::ON);
}

osg::ref_ptr<osg::Node> CViewManager::CreateTextHUD(osg::ref_ptr<osgText::Text> updateText)
{
	osg::Vec3 position(10.0f, 585.0f, 0.0f);
	osg::Vec3 delta(0.0f, -12.0f, 0.0f);

	osg::ref_ptr<osg::Camera> hudCamera = new osg::Camera;
	hudCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	hudCamera->setProjectionMatrixAsOrtho2D(0, 800, 0, 600);
	hudCamera->setViewMatrix(osg::Matrix::identity());
	hudCamera->setRenderOrder(osg::Camera::POST_RENDER);
	hudCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
	hudCamera->setName("TextHUD");

	{ // this displays what has been selected
		osg::Geode* geode = new osg::Geode();
		osg::StateSet* stateset = geode->getOrCreateStateSet();
		stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
		geode->setName("The text label");
		geode->addDrawable(updateText);
		hudCamera->addChild(geode);

		updateText->setCharacterSize(12.0f);
		updateText->setColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
		//updateText->setText("");
		updateText->setPosition(position);
		updateText->setDataVariance(osg::Object::DYNAMIC);

		position += delta;
	}

	return hudCamera;
}

void CViewManager::RemoveAllHandle(HWND hWnd)
{
	osgViewer::Viewer::EventHandlers ehs = this->getView(hWnd)->getEventHandlers();
	for (osgViewer::Viewer::EventHandlers::iterator iter = ehs.begin(); iter != ehs.end(); iter++)
		getView(hWnd)->removeEventHandler(*iter);
}

osg::ref_ptr<osg::Camera> CViewManager::CreateCoordCamera(osg::Camera* cm)
{
	osg::ref_ptr < osg::Camera > camera = new osg::Camera;
	osg::ref_ptr<osg::Box> box = new osg::Box(osg::Vec3(0.5f, 0.5f, 0.5f), 1.0f);
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;

	//创建立方体
	osg::ref_ptr<osg::ShapeDrawable> drawable = new osg::ShapeDrawable(box);
	drawable->setColor(osg::Vec4(0.2f, 0.4f, 0.4f, 0.4f));
	drawable->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
	geode->addDrawable(drawable);
	geode->setName("box");
	//创建字体‘前’
	osg::ref_ptr<osg::Texture2D> textureFront = new osg::Texture2D;
	osg::ref_ptr<osgText::Text> Tfront = new osgText::Text;
	Tfront->setColor(osg::Vec4(0.45f, 0.4f, 0.4f, 1.0f));
	Tfront->setText(L"前");
	Tfront->setPosition(osg::Vec3(0.5f, -0.05f, 0.5f));
	Tfront->setAxisAlignment(osgText::Text::XZ_PLANE);
	Tfront->setAlignment(osgText::Text::CENTER_CENTER);
	Tfront->setFont("C:\\Windows\\Fonts\\simhei.ttf");
	Tfront->setCharacterSize(0.6f);
	geode->addDrawable(Tfront);

	//创建字体‘后’
	osg::ref_ptr<osgText::Text> Tback = new osgText::Text;
	Tback->setColor(osg::Vec4(0.45f, 0.4f, 0.4f, 1.0f));
	Tback->setText(L"后");
	Tback->setPosition(osg::Vec3(0.5f, 1.05f, 0.5f));
	Tback->setRotation(osg::Quat(osg::PI, osg::Vec3(0.0f, 0.0f, 1.0f))*osg::Quat(-osg::PI_2, osg::Vec3(1.0f, 0.0f, 0.0f)));
	Tback->setAlignment(osgText::Text::CENTER_CENTER);
	Tback->setFont("C:\\Windows\\Fonts\\simhei.ttf");
	Tback->setCharacterSize(0.6f);
	geode->addDrawable(Tback);
	//创建字体‘左’
	osg::ref_ptr<osgText::Text> Tleft = new osgText::Text;
	Tleft->setColor(osg::Vec4(0.45f, 0.4f, 0.4f, 1.0f));
	Tleft->setText(L"左");
	Tleft->setPosition(osg::Vec3(-0.05f, 0.5f, 0.5f));
	Tleft->setRotation(osg::Quat(-osg::PI_2, osg::Vec3(0.0f, 1.0f, 0.0f))*osg::Quat(osg::PI_2, osg::Vec3(1.0f, 0.0f, 0.0f)));
	Tleft->setAlignment(osgText::Text::CENTER_CENTER);
	Tleft->setFont("C:\\Windows\\Fonts\\simhei.ttf");
	Tleft->setCharacterSize(0.6f);
	geode->addDrawable(Tleft);
	//创建字体‘右’
	osg::ref_ptr<osgText::Text> Tright = new osgText::Text;
	Tright->setColor(osg::Vec4(0.45f, 0.4f, 0.4f, 1.0f));
	Tright->setText(L"右");
	Tright->setPosition(osg::Vec3(1.05f, 0.5f, 0.5f));
	Tright->setAxisAlignment(osgText::Text::YZ_PLANE);
	Tright->setAlignment(osgText::Text::CENTER_CENTER);
	Tright->setFont("C:\\Windows\\Fonts\\simhei.ttf");
	Tright->setCharacterSize(0.6f);
	geode->addDrawable(Tright);
	//创建字体上
	osg::ref_ptr<osgText::Text> Tup = new osgText::Text;
	Tup->setColor(osg::Vec4(0.45f, 0.4f, 0.4f, 1.0f));
	Tup->setText(L"上");
	Tup->setPosition(osg::Vec3(0.5f, 0.5f, 1.05f));
	Tup->setAxisAlignment(osgText::Text::XY_PLANE);
	Tup->setAlignment(osgText::Text::CENTER_CENTER);
	Tup->setFont("C:\\Windows\\Fonts\\simhei.ttf");
	Tup->setCharacterSize(0.6f);
	geode->addDrawable(Tup);
	//创建字体下
	osg::ref_ptr<osgText::Text> Tdown = new osgText::Text;
	Tdown->setColor(osg::Vec4(0.45f, 0.4f, 0.4f, 1.0f));
	Tdown->setText(L"下");
	Tdown->setPosition(osg::Vec3(0.5f, 0.5f, -0.05f));
	Tdown->setAxisAlignment(osgText::Text::XY_PLANE);
	Tdown->setAlignment(osgText::Text::CENTER_CENTER);
	Tdown->setRotation(osg::Quat(osg::PI, osg::Vec3(1.0f, 0.0f, 0.0f)));
	Tdown->setFont("C:\\Windows\\Fonts\\simhei.ttf");
	Tdown->setCharacterSize(0.6f);
	geode->addDrawable(Tdown);

	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setRenderOrder(osg::Camera::POST_RENDER);
	camera->setName("CoordCamera");
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("axes.osgt");
	osg::BoundingSphere bs = node->getBound();
	osg::ref_ptr<osg::MatrixTransform> matf = new osg::MatrixTransform;
	matf->addChild(node);
	matf->addChild(geode);
	matf->setUpdateCallback(new CroodAxisCallBack(cm));

	double viewDistance = 2 * bs.radius();
	double znear = viewDistance - bs.radius();
	double zfar = viewDistance + bs.radius();
	float right = bs.radius();
	float top = bs.radius();
	camera->setProjectionMatrixAsOrtho(-right*1.4, right*1.4, -top*1.4, top*1.4, znear, zfar);
	camera->setViewport(5.0, -20.0f, 200, 200);
	osg::Vec3d upDirection(0.0, 0.0, 1.0);
	osg::Vec3d viewDirection(0.0, -1.0, 0.0);
	osg::Vec3d center = bs.center();
	osg::Vec3d eyePoint = center + viewDirection*viewDistance;
	camera->setViewMatrixAsLookAt(eyePoint, center, upDirection);
	camera->addChild(matf);
	camera->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	return camera.release();
}

osg::Group* CViewManager::CreateLight(osg::Group* root)
{
	osg::ref_ptr<osg::Light> light = new osg::Light();
	light->setLightNum(2);
	light->setPosition(osg::Vec4(0.0, 0.0f, 1.0f, 0.0f));
	//light->setDirection(osg::Vec3(0.0f, -1.0f, -1.0f));
	//light->setDirection(osg::Vec3(-0.85f, 0.9f, -0.8f));
	light->setAmbient(osg::Vec4(0.8f, 0.8f, 0.8f, 1.0f));
	light->setDiffuse(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//light->setSpecular(osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f));
	osg::ref_ptr<osg::LightSource> lightSource = new osg::LightSource();
	lightSource->setLight(light.get());
	lightSource->setName("light1");
	lightSource->setLocalStateSetModes(osg::StateAttribute::ON);
	osg::StateSet* rootStateSet = new osg::StateSet;
	root->setStateSet(rootStateSet);
	lightSource->setStateSetModes(*rootStateSet, osg::StateAttribute::ON);

	root->addChild(lightSource);
	root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);
	root->getOrCreateStateSet()->setMode(GL_LIGHT2, osg::StateAttribute::ON);
	return lightSource;
}

void CViewManager::SetCameraManipulator(HWND hWnd, osgGA::CameraManipulator *cm)
{
	RemoveAllHandle(hWnd);
	getView(hWnd)->setCameraManipulator(cm);
}
