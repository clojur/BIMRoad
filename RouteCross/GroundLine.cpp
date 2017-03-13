#include "stdafx.h"
#include "GroundLine.h"


CGroundLine::CGroundLine()
{
}


CGroundLine::~CGroundLine()
{
}


CGL_Callback::CGL_Callback(CGroundLine *GL) 
	:CComponentCallback(GL->GetRoot()),
	m_GL(GL)
{

}

CGL_Callback::~CGL_Callback()
{

}

void CGL_Callback::DoUpdate()
{

}
