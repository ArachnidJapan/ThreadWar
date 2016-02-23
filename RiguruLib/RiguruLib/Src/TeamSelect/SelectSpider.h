#pragma once
#include "../Actor/ID.h"
#include "../Math/Vector2.h"
#include <memory>
#include <map>
#include <vector>
#include "SelectPlayerParam.h"

class SelectSpider{
public:
	SelectSpider(bool red_, bool spider_, Vector2 pos_, int playerNum = 1000, std::shared_ptr<SelectSpider> spiderSelect = nullptr,std::shared_ptr<SelectSpider> enemySpiderSelect_ = nullptr);
	~SelectSpider();
	void Update();
	void Draw();
	void SetSelect(bool flag);
	void SetTarantula(bool flag);
	bool ReturnTarantula(){ return tarantula; }
	int CPUCount();

	void SpiderSelect();
	void PlayerSelect();

	void SetPlayerParam(int playerNum,bool plus,std::shared_ptr<SelectSpider> enemySpiderSelect_);
	SelectPlayerParam ReturnMyPlayerParam(int playerNum){ return spp[playerNum]; }

	TeamSelectResult PlayerHave();

	std::map<int, SelectPlayerParam> ReturnSelectPlayer(){ return spp; }
private:
	bool tarantula;
	bool select;
	Vector2 pos;
	bool red;
	bool spider;
	std::shared_ptr<SelectSpider> spiderSelect;
	std::shared_ptr<SelectSpider> enemySpiderSelect;
	int playerNum;
	std::map<SelectPlayerParam, std::string> sppString;
	std::map<int,SelectPlayerParam> spp;
};