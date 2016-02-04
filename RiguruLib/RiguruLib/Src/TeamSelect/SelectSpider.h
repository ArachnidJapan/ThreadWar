#pragma once
#include "../Actor/ID.h"
#include "../Math/Vector2.h"
#include <memory>

class SelectSpider{
public:
	SelectSpider(bool player_, bool spider_, Vector2 pos_,std::shared_ptr<SelectSpider> spiderSelect = nullptr);
	~SelectSpider();
	void Update();
	void Draw();
	void SetSelect(bool flag);
	void SetTarantula(bool flag);
	bool ReturnTarantula(){ return tarantula; }

private:
	bool tarantula;
	bool select;
	Vector2 pos;
	bool player;
	bool spider;
	std::shared_ptr<SelectSpider> spiderSelect;
};