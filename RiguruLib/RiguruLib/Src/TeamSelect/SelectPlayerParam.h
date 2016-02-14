#pragma once
#include <vector>

enum SelectPlayerParam{
	P0,
	P1,
	CP1,
	CP2,
	CP3,
	CP4,
	NONE,
};

struct TeamSelectResult{
	bool redTarantula = false;
	bool blueTarantula = true;
	bool redHavePlayer = true;
	bool blueHavePlayer = false;
	int redHaveCPU = 3;
	int blueHaveCPU = 4;
	//各プレイヤーが獲得したポイント。挿入されるデータの順番はREDチームの1~4のキャラ、Blueチームの1~4のキャラの順。
	std::vector<int> points;
	//各プレイヤーの名前(P1,CP1等)
	std::vector<std::string> names;
	//RedチームとBlueチームの総合得点。
	float playerTeamPoint, enemyTeamPoint;

};