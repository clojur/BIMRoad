#pragma once
#include "common.h"
class CComponentCallback;
class OSGBIM_API CViewManager
{
public:
	CViewManager(){}
	virtual ~CViewManager(){}
public:
	virtual void AddView(HWND hWnd) {}
	virtual void RemoveView(HWND hWnd) {}
	virtual void SetSceneData(HWND hWnd, CComponentCallback* cb);
	virtual void SetSceneData(HWND hWnd, osg::Node* node);
	virtual osg::Node* GetSceneData(HWND hWnd);
	virtual void Display(HWND hWnd) {}

	//通过节点名删除节点
	void RemoveNodeByName(const char *name, HWND hWnd);
	//删除全部默认节点
	void RemoveAllNode(HWND hWnd);
	//移除全部事务
	void RemoveAllHandle(HWND hWnd);
	//设置操作器
	void SetCameraManipulator(HWND hWnd, osgGA::CameraManipulator *cm);
	//返回view
	osg::ref_ptr<osgViewer::View> getView(HWND hWnd){ return _viewMap[hWnd]; }
protected:
	void InitView(HWND hWnd, osgViewer::View* view);
	void InitCameraConfig(HWND hWnd, osgViewer::View* view);
	// 创建TextHUD
	osg::ref_ptr<osg::Node> CreateTextHUD(osg::ref_ptr<osgText::Text> updateText);
	//创建坐标轴相机
	osg::ref_ptr<osg::Camera> CreateCoordCamera(osg::Camera* cm);
	//创建光源
	osg::Group* CreateLight(osg::Group* root);
protected:
	std::map<HWND, osg::ref_ptr<osgViewer::View>> _viewMap;
	std::map<HWND, unsigned int> _rootIndex;
};

