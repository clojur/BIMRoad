#pragma once

#ifdef SCENENODE_DLL
#define SCENENODE_API __declspec(dllexport)
#else         
#define SCENENODE_API  __declspec(dllimport)  
#ifdef _DEBUG
#pragma comment(lib,"SceneNoded.lib")
#pragma message("Automatically linking with SceneNoded.dll")
#else
#pragma comment(lib,"SceneNode.lib")
#pragma message("Automatically linking with SceneNode.dll")
#endif
#endif