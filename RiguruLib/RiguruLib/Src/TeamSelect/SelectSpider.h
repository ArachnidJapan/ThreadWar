#pragma once
#include "../Actor/ID.h"
#include "../Math/Vector2.h"
#include <memory>
#include <map>
#include <vector>
#include "SelectPlayerParam.h"

class SelectSpider{
public:
	SelectSpider(bool player_, bool spider_, Vector2 pos_,int playerNum = 1000,std::shared_ptr<SelectSpider> spiderSelect = nullptr);
	~SelectSpider();
	void Update();
	void Draw();
	void SetSelect(bool flag);
	void SetTarantula(bool flag);
	bool ReturnTarantula(){ return tarantula; }

	void SpiderSelect();
	void PlayerSelect();

	void SetPlayerParam(int playerNum,bool plus);
	SelectPlayerParam ReturnMyPlayerParam(int playerNum){ return spp[playerNum]; }

	bool PlayerHave();
	void SetEnemyPlayerHave(bool flag){ enemyPlayerHave = flag; }

	std::map<int, SelectPlayerParam> ReturnSelectPlayer(){ return spp; }
private:
	bool tarantula;
	bool select;
	Vector2 pos;
	bool player;
	bool spider;
	std::shared_ptr<SelectSpider> spiderSelect;
	int playerNum;
	std::vector<SelectPlayerParam> havePlayerSpp,notHavePlayerSpp;
	std::map<SelectPlayerParam, std::string> sppString;
	std::map<int,SelectPlayerParam> spp;

	bool enemyPlayerHave;
};