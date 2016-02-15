#include "Option.h"
#include "../../Other/Device.h"
#include "../../Graphic/Graphic.h"
#include "../../Math/MathUtility.h"
#include "../../Audio/Audio.h"

#define OPTION_POP_TIME 120
#define OPTION_SHUT_TIME 120
#define OPTION_SCALE 0.8f
#define OPTION_ALPHA 0.7f

Option::Option(){
	isExitGame = false;
	for (int i = 0; i <= OPTION_SELECT_NUM - 1; i++){
		os_scale.push_back(OPTION_SCALE);
		os_nextScale.push_back(os_scale.at(i));
		os_prevScale.push_back(os_scale.at(i));
		os_alpha.push_back(OPTION_ALPHA);
		os_nextAlpha.push_back(os_alpha.at(i));
	}
}
Option::~Option(){
	os_scale.clear();
	os_nextScale.clear(); 
	os_prevScale.clear();
	os_alpha.clear(); 
	os_nextAlpha.clear();
	gaugeLerpTime.clear();
	gaugeLength.clear();
	prevGaugeLength.clear();
	nextGaugeLength.clear();
}

void Option::Initialize(bool isGamePlay_){
	isOption = false;
	isPop = false;
	isShut = false;
	isManual = false;
	isReturnMenu = false;
	manualEnd = false;
	decision = false;
	isGamePlay = isGamePlay_;
	select = OPTION_SELECT::BGM_SELECT;
	allAlpha = 0.0f;
	manualAlpha = 0.0f;
	manualBack = 1.0f;
	selectAlphaTime = 0;
	selectScaleTime = 0;
	timer = 0;
	lerpTime = 0;
	for (int i = 0; i <= OPTION_SELECT_NUM - 1; i++){
		os_scale.at(i) = OPTION_SCALE;
		os_nextScale.at(i) = os_scale.at(i);
		os_prevScale.at(i) = os_scale.at(i);
		os_alpha.at(i) = OPTION_ALPHA;
		os_nextAlpha.at(i) = os_alpha.at(i);
		if (select == i){
			os_nextScale.at(i) = 1.0f;
			os_nextAlpha.at(i) = 1.0f;
		}
	}

	/*コンフィグファイル読み込み。*/
	CSVReader::GetInstance().FileSet(FILE_ID::CONFIG_FILE, "Res/Config/config.ini");
	CSVReader::GetInstance().load(FILE_ID::CONFIG_FILE);
	//コンフィグデータ格納。
	for (int row = 0; row < CSVReader::GetInstance().rows(FILE_ID::CONFIG_FILE); row++){
		config[(CONFIG_DATA)row] = CSVReader::GetInstance().geti(FILE_ID::CONFIG_FILE, row, 1);
	}
	for (int i = 0; i <= 1; i++){
		gaugeLength.push_back(0);
		if (config[(CONFIG_DATA)i] > 0)
			gaugeLength.at(i) = config[(CONFIG_DATA)i] / 10.0f;	
		nextGaugeLength.push_back(gaugeLength.at(i));
		prevGaugeLength.push_back(gaugeLength.at(i));
		gaugeLerpTime.push_back(1.0f);
	}
}
void Option::Update(float frameTime){
	if (!IsOption())return;

	if (isPop){
		Pop(frameTime);
		return;
	}
	if (decision){
		selectScaleTime = min((selectScaleTime + 1.0f / 60.0f)*60.0f*frameTime, 1.0f);
		os_scale.at(select) = Math::lerp3(os_scale.at(select), 1.1f, selectScaleTime);
		allAlpha = Math::lerp3(1.0f, 0.0f, selectScaleTime);
		if (selectScaleTime == 1.0f)
			decision = false;
		return;
	}
	else if (isShut){
		Shut(frameTime);
		return;
	}

	if (isManual)
		Manual(frameTime);
	else
		OptionSelect(frameTime);

}
void Option::Draw(){
	//現在のスクリーンサイズに拡大。
	Vector2 screenPow = vector2(1920.0f / 1280.0f, 1080.0f / 720.0f);
	Graphic::GetInstance().DrawTexture(TEXTURE_ID::BLACK_TEXTURE, vector2(0, 0), vector2(1920, 1080), D3DXCOLOR(1, 1, 1, (allAlpha + manualAlpha)*0.75f), vector2(0, 0));
	Graphic::GetInstance().DrawTexture(TEXTURE_ID::DAMAGE_TEXTURE, vector2(0, 0), screenPow, D3DXCOLOR(1, 1, 1, (allAlpha + manualAlpha)*0.5f), vector2(0, 0));
	float selectAlpha = Math::sin((float)selectAlphaTime);
	Vector2 scrCenter = { 1920 / 2 + 128, 1080 / 2 };
	float space = 128;
	if (selectScaleTime != 1.0f){
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(scrCenter.x - 512, scrCenter.y + space * 2), vector2(0.6f, 0.6f)*os_scale[0], 0.5f, "BGM", vector3(1, 1, 1), allAlpha*(os_alpha[0] + (selectAlpha * (select == 0))), false);
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(scrCenter.x - 512, scrCenter.y + space * 1), vector2(0.6f, 0.6f)*os_scale[1], 0.5f, "SE", vector3(1, 1, 1), allAlpha*(os_alpha[1] + (selectAlpha * (select == 1))), false);
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(scrCenter.x - 512, scrCenter.y + space * 0), vector2(0.6f, 0.6f)*os_scale[2], 0.5f, "MANUAL", vector3(1, 1, 1), allAlpha*(os_alpha[2] + (selectAlpha * (select == 2))), false);
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(scrCenter.x - 512, scrCenter.y + space *-1), vector2(0.6f, 0.6f)*os_scale[3], 0.5f, "RETURN TO MENU", vector3(1, 1, 1), allAlpha*(os_alpha[3] + (selectAlpha * (select == 3))), false);
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(scrCenter.x - 512, scrCenter.y + space *-2), vector2(0.6f, 0.6f)*os_scale[4], 0.5f, "QUIT GAME", vector3(1, 1, 1), allAlpha*(os_alpha[4] + (selectAlpha * (select == 4))), false);
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(scrCenter.x - 512 - 128, scrCenter.y + space *-3), vector2(0.4f, 0.4f)*os_scale[5], 0.5f, "BACK", vector3(1, 1, 1), (allAlpha*(os_alpha[5] + (selectAlpha * (select == 5)))), false);

		//BGMとSEのゲージの長さを求める。
		for (int i = 0; i <= 1; i++){
			gaugeLength.at(i) = Math::lerp3(prevGaugeLength.at(i), nextGaugeLength.at(i), gaugeLerpTime.at(i));
		}
		//BGMとSEのゲージの描画。
		Graphic::GetInstance().DrawTexture(TEXTURE_ID::LINE_WHITE_TEXTURE, vector2(scrCenter.x - 256, scrCenter.y + space * 2), vector2(1, 1), D3DXCOLOR(1, 1, 1, allAlpha*os_alpha[0]), vector2(0, 0));
		Graphic::GetInstance().DrawTexture(TEXTURE_ID::LINE_BLACK_TEXTURE, vector2(scrCenter.x - 256, scrCenter.y + space * 2), vector2(1, 1), D3DXCOLOR(1, 1, 1, allAlpha*os_alpha[0]), vector2(0, 0), 0, 0, gaugeLength.at(0));
		Graphic::GetInstance().DrawTexture(TEXTURE_ID::LINE_WHITE_TEXTURE, vector2(scrCenter.x - 256, scrCenter.y + space * 1), vector2(1, 1), D3DXCOLOR(1, 1, 1, allAlpha*os_alpha[0]), vector2(0, 0));
		Graphic::GetInstance().DrawTexture(TEXTURE_ID::LINE_BLACK_TEXTURE, vector2(scrCenter.x - 256, scrCenter.y + space * 1), vector2(1, 1), D3DXCOLOR(1, 1, 1, allAlpha*os_alpha[0]), vector2(0, 0), 0, 0, gaugeLength.at(1));
	}
	else{
		Graphic::GetInstance().DrawTexture(TEXTURE_ID::MANUAL_TEXTURE, vector2(1920 / 2, 1080), vector2(1.7f, 1.7f), D3DXCOLOR(1, 1, 1, manualAlpha), vector2(0.5, 1.0f));
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(scrCenter.x - 512 - 128, scrCenter.y + space *-3), vector2(0.4f, 0.4f)*manualBack, 0.5f, "BACK", vector3(1, 1, 1), manualAlpha, false);
	}
}

void Option::Pop(float frameTime){
	isOption = true;//オプション中。
	isPop = true;//ポップ中。
	int max = 120;
	timer = min(timer + 60 * frameTime, max);
	allAlpha = Math::lerp3(0.0f, 1.0f, (float)(timer / 120.0f));
	if (timer >= max){
		isPop = false;
		timer = 0;
	}
}
void Option::Shut(float frameTime){ 
	int max = 120;
	timer = min(timer + 60 * frameTime, max);
	allAlpha = Math::lerp3(1.0f, 0.0f, timer / 120.0f);
	selectScaleTime = min((selectScaleTime + 1.0f / 120.0f)*60.0f*frameTime, 1.0f);
	os_scale[5] = Math::lerp(os_scale[5], 1.1f, selectScaleTime);

	if (timer >= max)
		Initialize(isGamePlay);
}
bool Option::ReturnMenu(){
	return isReturnMenu;
}
bool Option::ReturnExitGame(){
	return isExitGame;
}
void Option::ConfigSave(){
	std::map<int, std::vector<std::string>> strmap;
	strmap[CONFIG_DATA::MUSIC_VOL].push_back("MusicVolume");
	strmap[CONFIG_DATA::SE_VOL].push_back("SeVolume");
	//読み込むファイル名のセット。
	CSVReader::GetInstance().FileSet(FILE_ID::CONFIG_FILE, "Res/Config/config.ini");
	//コンフィグデータ読み込み。
	for (int row = 0; row < (int)strmap.size(); row++){
		strmap[row].push_back(std::to_string(config[(CONFIG_DATA)row]));
	}
	CSVReader::GetInstance().WriteData(FILE_ID::CONFIG_FILE, strmap);
	CSVReader::GetInstance().save(FILE_ID::CONFIG_FILE);

	Audio::GetInstance().SetAllBGMVolume((config[CONFIG_DATA::MUSIC_VOL] * 7) + 30);
	Audio::GetInstance().SetAllSEVolume((config[CONFIG_DATA::SE_VOL] * 7) + 30);
}

bool Option::IsOption(){
	return isOption;
}

void Option::Move(float frameTime){
	lerpTime = 0;
	timer = 0;
	selectAlphaTime = 0;
	for (int i = 0; i <= OPTION_SELECT_NUM - 1; i++){
		os_prevScale.at(i) = os_scale.at(i);
		os_nextScale.at(i) = OPTION_SCALE;
		os_nextAlpha.at(i) = OPTION_ALPHA;
		if (select == i){
			os_nextScale.at(i) = 1.0f;
			os_nextAlpha.at(i) = 1.0f;
		}
	}
}

void Option::OptionSelect(float frameTime){

	//項目選択
	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_UP, true) || 
		Device::GetInstance().GetInput()->LeftStick(0, true).z <= -0.5f &&
		lerpTime >= 0.5f){
		select = (OPTION_SELECT)(select - 1);
		select = select < 0 ? select = (OPTION_SELECT)5 : select;
		Move(frameTime);
		Audio::GetInstance().PlaySE(SE_ID::SWITCH_SE);
	}
	else if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_DOWN, true) ||
		Device::GetInstance().GetInput()->LeftStick(0, true).z >= 0.5f &&
		lerpTime >= 0.5f){
		select = (OPTION_SELECT)(select + 1);
		select = (OPTION_SELECT)(select%OPTION_SELECT_NUM);
		Move(frameTime);
		Audio::GetInstance().PlaySE(SE_ID::SWITCH_SE);
	}

	//音量調整
	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_LEFT, true) ||
		Device::GetInstance().GetInput()->LeftStick(0, true).x <= -0.5f){
		if (select != OPTION_SELECT::BGM_SELECT && select != OPTION_SELECT::SE_SELECT)
			return;

		config[(CONFIG_DATA)select] = max(config[(CONFIG_DATA)select]--, 0);
		gaugeLerpTime.at(select) = 0;
		prevGaugeLength.at(select) = gaugeLength.at(select);
		nextGaugeLength.at(select) = 0;
		if (config[(CONFIG_DATA)select] > 0)
			nextGaugeLength.at(select) = config[(CONFIG_DATA)select] / 10.0f;

		Audio::GetInstance().SetAllBGMVolume((config[CONFIG_DATA::MUSIC_VOL] * 7) + 30);
		Audio::GetInstance().SetAllSEVolume((config[CONFIG_DATA::SE_VOL] * 7) + 30);
		Audio::GetInstance().PlaySE(SE_ID::SWITCH_SE);
}
	else if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_RIGHT, true) ||
		Device::GetInstance().GetInput()->LeftStick(0, true).x >= 0.5f){
		if (select != OPTION_SELECT::BGM_SELECT && select != OPTION_SELECT::SE_SELECT)
			return;

		config[(CONFIG_DATA)select] = min(config[(CONFIG_DATA)select]++, 10);
		gaugeLerpTime.at(select) = 0;
		prevGaugeLength.at(select) = gaugeLength.at(select);
		nextGaugeLength.at(select) = 0;
		if (config.at((CONFIG_DATA)select) > 0)
			nextGaugeLength.at(select) = config.at((CONFIG_DATA)select) / 10.0f;

		Audio::GetInstance().SetAllBGMVolume((config[CONFIG_DATA::MUSIC_VOL] * 7) + 30);
		Audio::GetInstance().SetAllSEVolume((config[CONFIG_DATA::SE_VOL] * 7) + 30);
		Audio::GetInstance().PlaySE(SE_ID::SWITCH_SE);
	}
	//決定
	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_Z, true) ||
		Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_SPACE, true) ||
		Device::GetInstance().GetInput()->GamePadButtonDown(0, GAMEPADKEY::BUTTON_CURCLE, true)){
		Decision();
	}
	if (isGamePlay){
		if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_ESC, true) ||
			Device::GetInstance().GetInput()->GamePadButtonDown(0, GAMEPADKEY::BUTTON_START, true)){
			select = OPTION_SELECT::BACK_SELECT;
			Decision();
		}
	}
	//キャンセル
	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_X, true) ||
		Device::GetInstance().GetInput()->GamePadButtonDown(0, GAMEPADKEY::BUTTON_CROSS, true)){
		switch (select)
		{
		case BGM_SELECT:
			select = OPTION_SELECT::BACK_SELECT;
			break;
		case SE_SELECT:
			select = OPTION_SELECT::BACK_SELECT;
			break;
		}
		isShut = true;
		ConfigSave();
		Audio::GetInstance().PlaySE(SE_ID::BACK_SE);
	}
	//各演出のLerp
	for (int i = 0; i <= OPTION_SELECT_NUM - 1; i++){
		os_scale.at(i) = Math::lerp4(os_prevScale.at(i), os_nextScale.at(i), lerpTime);
		os_alpha.at(i) = Math::lerp4(os_alpha.at(i), os_nextAlpha.at(i), lerpTime);
	}
	lerpTime = min(lerpTime + 1.0f / 10.0f * 60.0f * frameTime, 1.0f);
	for (int i = 0; i <= 1; i++)
		gaugeLerpTime.at(i) = min(gaugeLerpTime.at(i) + 1.0f / 5.0f, 1.0f);
	selectAlphaTime = min(selectAlphaTime + 5 * 60 * frameTime, 360);
	selectAlphaTime %= 360;
}

void Option::Manual(float frameTime){
	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_X, true) ||
		Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_Z, true) ||
		Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_SPACE, true) ||
		Device::GetInstance().GetInput()->GamePadButtonDown(0, GAMEPADKEY::BUTTON_CURCLE, true)){
		manualEnd = true;
		os_scale.at(select) = 1.0f;
		os_alpha.at(select) = 1.0f;
		Audio::GetInstance().PlaySE(SE_ID::ENTER_SE);
	}

	if (!manualEnd)
		manualAlpha = min(manualAlpha + 1.0f / 30.0f * 60.0f*frameTime, 1.0f);
	else{
		manualAlpha = max(manualAlpha - 1.0f / 30.0f * 60.0f*frameTime, 0.0f);
		manualBack = min(manualBack + 0.1f / 30.0f * 60.0f*frameTime, 1.1f);
	}

	if (manualEnd&&manualAlpha == 0.0f){
		selectScaleTime = 0;
		isManual = false;
		manualEnd = false;
		manualBack = 1.0f;
		Pop(frameTime);
	}
}


void Option::Decision(){
	selectAlphaTime = 0.0f;
	switch (select)
	{
	case MANUAL_SELECT:
		decision = true;
		isManual = true;
		break;
	case RETURN_MENU_SELECT:
		decision = true;
		isReturnMenu = true;
		ConfigSave();
		break;
	case EXIT_GAME_SELECT:
		decision = true;
		isExitGame = true;
		ConfigSave();
		break;
	case BACK_SELECT:
		isShut = true;
		ConfigSave();
		break;
	default:
		break;
	}
	Audio::GetInstance().PlaySE(SE_ID::ENTER_SE);
}
