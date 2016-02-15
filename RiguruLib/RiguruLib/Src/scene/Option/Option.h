#pragma once
#include "../Scene.h"
#include <map>
#include "../../Reader/CSVReader.h"
#include <vector>

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

	void Initialize(bool isGamePlay_ = false);
	void Update(float frameTime);
	void Draw();

	//現在Optionが開かれているかどうか。
	bool IsOption();
	bool ReturnMenu();
	bool ReturnExitGame();
	void ConfigSave();
	void Decision();

	//画面フェードイン。
	void Pop(float frameTime);
	//画面フェードアウト。
	void Shut(float frameTime);
	//オプション項目選択。
	void OptionSelect(float frameTime);
	//マニュアル画面。
	void Manual(float frameTime);
	//項目上下移動。
	void Move(float frameTime);

private:
	int timer, selectAlphaTime;
	float lerpTime, allAlpha, manualAlpha, pageAlpha, selectScaleTime, manualBack;
	std::vector<float> gaugeLerpTime;
	std::vector<float> gaugeLength, prevGaugeLength, nextGaugeLength;
	//OptionUIを起動中か,マニュアル画面を起動中か,メニューに戻るか,ゲーム終了を選択したか。
	bool isPop, isShut, isManual, isReturnMenu, isExitGame, isOption, decision;
	bool manualEnd, isNextPage;
	int manualPage, nextManualPage;
	OPTION_SELECT select;//現在どれが選択されているか。
	std::map<CONFIG_DATA, int> config;
	bool isGamePlay;
	bool downCircle, downCross;

	//選択肢のパラメータ。
	std::vector<float> os_scale, os_nextScale, os_prevScale;
	std::vector<float> os_alpha, os_nextAlpha;
	/*float os_scale[OPTION_SELECT_NUM];
	float os_nextScale[OPTION_SELECT_NUM], os_prevScale[OPTION_SELECT_NUM];
	float os_alpha[OPTION_SELECT_NUM];
	float os_nextAlpha[OPTION_SELECT_NUM];*/
};