#pragma once
#include "StalkerAPI.h"
#define ENGINE_VERSION  "1.7.01_RC1"
class XRAPI_API GameVersionController
{
public:
	enum Game
	{
		NGame=0,
		SOC,
		CS,
		COP,
		
	};
	enum Path
	{
		NPath=0,
		SOC_1004,
		SOC_1007,//1006 סמגלוסעטל
		CS_1510,
		COP_1602,
	};
	Game getGame()const;
	Path getPath()const;

	GameVersionController(Path path);
	~GameVersionController() {}
private:
	Path m_path;

};
extern XRAPI_API GameVersionController* gameVersionController;

