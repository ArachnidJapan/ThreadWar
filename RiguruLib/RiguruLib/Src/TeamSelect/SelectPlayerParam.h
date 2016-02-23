#pragma once
#include <vector>

enum SelectPlayerParam{
	P0,
	P1,
	P2,
	P3,
	P4,
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
	bool redHavePlayer2 = false;
	bool blueHavePlayer2 = false;
	bool redHavePlayer3 = false;
	bool blueHavePlayer3 = false;
	bool redHavePlayer4 = false;
	bool blueHavePlayer4 = false;
	int redHaveCPU = 3;
	int blueHaveCPU = 4;

	std::vector<int> redTeamPoint;
	std::vector<int> blueTeamPoint;
};