#pragma once

#ifdef OSGBIM_DLL
	#define OSGBIM_API __declspec(dllexport)
#else         
	#define OSGBIM_API  __declspec(dllimport)  
	#ifdef _DEBUG
		#pragma comment(lib,"OSGBIMd.lib")
		#pragma message("Automatically linking with OSGBIMd.dll")
	#else
		#pragma comment(lib,"OSGBIM.lib")
		#pragma message("Automatically linking with OSGBIM.dll")
	#endif
#endif
