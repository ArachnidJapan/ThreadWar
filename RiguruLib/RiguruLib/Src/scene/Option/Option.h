#pragma once
#include "../Scene.h"
#include <map>
#include "../../Reader/CSVReader.h"

#define OPTION_SELECT_NUM 6

enum OPTION_SELECT{
	BGM_SELECT,
	SE_SELECT,
	MANUAL_SELECT,
	RETURN_MENU_SELECT,
	EXIT_GAME_SELECT,
	BACK_SELECT,
};

class Option{
public:
	Option();
	~Option();

	void Initialize();
	void Update(float frameTime);
	void Draw();

	//����Option���J����Ă��邩�ǂ����B
	bool IsOption();
	bool ReturnMenu();
	bool ReturnExitGame();
	void ConfigSave();

	//��ʃt�F�[�h�C���B
	void Pop(float frameTime);
	//��ʃt�F�[�h�A�E�g�B
	void Shut(float frameTime);
	//�I�v�V�������ڑI���B
	void OptionSelect(float frameTime);
	//�}�j���A����ʁB
	void Manual(float frameTime);
	//���ڏ㉺�ړ��B
	void Move(float frameTime);

private:
	int timer, selectAlphaTime;
	float lerpTime, allAlpha, manualAlpha, selectScaleTime, manualBack;
	float gaugeLerpTime[2];
	float gaugeLength[2], prevGaugeLength[2], nextGaugeLength[2];
	//OptionUI���N������,�}�j���A����ʂ��N������,���j���[�ɖ߂邩,�Q�[���I����I���������B
	bool isPop, isShut, isManual, isReturnMenu, isExitGame, isOption, decision;
	bool manualEnd;
	OPTION_SELECT select;//���݂ǂꂪ�I������Ă��邩�B
	std::map<CONFIG_DATA, int> config;

	//�I�����̃p�����[�^�B
	float os_scale[OPTION_SELECT_NUM];
	float os_nextScale[OPTION_SELECT_NUM], os_prevScale[OPTION_SELECT_NUM];
	float os_alpha[OPTION_SELECT_NUM];
	float os_nextAlpha[OPTION_SELECT_NUM];
};