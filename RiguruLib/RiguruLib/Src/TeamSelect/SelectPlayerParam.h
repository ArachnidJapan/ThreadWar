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
	//�e�v���C���[���l�������|�C���g�B�}�������f�[�^�̏��Ԃ�RED�`�[����1~4�̃L�����ABlue�`�[����1~4�̃L�����̏��B
	std::vector<int> points;
	//�e�v���C���[�̖��O(P1,CP1��)
	std::vector<std::string> names;
	//Red�`�[����Blue�`�[���̑������_�B
	float playerTeamPoint, enemyTeamPoint;

};