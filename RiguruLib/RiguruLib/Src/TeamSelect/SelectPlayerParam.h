#pragma once
<<<<<<< HEAD
#include <vector>
=======
#include<vector>
>>>>>>> facc204d0cc95285886c952492c802ed35fac0c3

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
<<<<<<< HEAD
	//�e�v���C���[���l�������|�C���g�B�}�������f�[�^�̏��Ԃ�RED�`�[����1~4�̃L�����ABlue�`�[����1~4�̃L�����̏��B
	std::vector<int> points;
	//�e�v���C���[�̖��O(P1,CP1��)
	std::vector<std::string> names;
	//Red�`�[����Blue�`�[���̑������_�B
	float playerTeamPoint, enemyTeamPoint;

=======
	std::vector<int> redTeamPoint;
	std::vector<int> blueTeamPoint;
>>>>>>> facc204d0cc95285886c952492c802ed35fac0c3
};