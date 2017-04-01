#pragma once

#ifdef ROUTECROSS_DLL
#define ROUTECROSS_API __declspec(dllexport)
#else         
#define ROUTECROSS_API  __declspec(dllimport)  
#ifdef _DEBUG
#pragma comment(lib,"RouteCrossd.lib")
#pragma message("Automatically linking with RouteCrossd.dll")
#else
#pragma comment(lib,"RouteCross.lib")
#pragma message("Automatically linking with RouteCross.dll")
#endif
#endif
