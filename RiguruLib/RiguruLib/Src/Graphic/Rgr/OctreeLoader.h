#pragma once
#include "OctreeUser.h"
#include "../../Other/DX11User.h"
class OctreeLoader{
public:
	OctreeLoader();
	~OctreeLoader();
	
	//octファイルを読み込みOctreeUserに保存する
	HRESULT Load(const char* modelName, OctreeUser* o);


private:
};