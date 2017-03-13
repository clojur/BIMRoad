#include "stdafx.h"
#include "CComponentCallback.h"


CComponentCallback::CComponentCallback(osg::Group* root)
{
	_changed = true;
	_root = root;
	_root->setUpdateCallback(this);
}

CComponentCallback::~CComponentCallback()
{
}

void CComponentCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	_mutex.lock();
	if (_changed == true)
	{
		DoUpdate();
		_changed = false;
	}
	_mutex.unlock();

	traverse(node, nv);
}

void CComponentCallback::Update()
{
	_changed = true;
}

void CComponentCallback::Lock()
{
	_mutex.lock();
}

void CComponentCallback::Unlock()
{
	_mutex.unlock();
}

