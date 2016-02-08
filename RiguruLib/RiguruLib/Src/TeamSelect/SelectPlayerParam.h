#pragma once

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
};