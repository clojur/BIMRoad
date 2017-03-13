#pragma once
#include "common.h"
class OSGBIM_API CComponentCallback :
	public osg::NodeCallback
{
public:
	CComponentCallback(osg::Group* root);
	~CComponentCallback();
public:
	osg::Group* getRoot() const
	{
		return _root.get();
	}

	void setView(osgViewer::View* view) { _view = view; }
	osgViewer::View* getView() const { return _view.get(); }

	void operator()(osg::Node* node, osg::NodeVisitor* nv) override;
	virtual void DoUpdate() {}

public:
	void Update();
	void Lock();
	void Unlock();

protected:
	osg::observer_ptr<osg::Group> _root;
	osg::observer_ptr<osgViewer::View> _view;
	bool _changed;

private:
	OpenThreads::Mutex _mutex;
};

