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
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::MANUAL_TEXTURE, "Res/Texture/gamepad.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::LINE_WHITE_TEXTURE, "Res/Texture/line_white.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::LINE_BLACK_TEXTURE, "Res/Texture/line_black.png");
}
Option::~Option(){}

void Option::Initialize(){
	isOption = false;
	isPop = false;
	isShut = false;
	isManual = false;
	isReturnMenu = false;
	isExitGame = false;
	manualEnd = false;
	decision = false;
	select = OPTION_SELECT::BGM_SELECT;
	allAlpha = 0.0f;
	manualAlpha = 0.0f;
	manualBack = 1.0f;
	selectAlphaTime = 0;
	selectScaleTime = 0;
	timer = 0;
	lerpTime = 0;
	for (auto i : gaugeLerpTime){
		i = 1.0f;
	}

	for (int i = 0; i <= OPTION_SELECT_NUM - 1; i++){
		os_scale[i] = OPTION_SCALE;
		os_nextScale[i] = os_scale[i];
		os_prevScale[i] = os_scale[i];
		os_alpha[i] = OPTION_ALPHA;
		os_nextAlpha[i] = os_alpha[i];
		if (select == i){
			os_nextScale[i] = 1.0f;
			os_nextAlpha[i] = 1.0f;
		}
	}

	/*�R���t�B�O�t�@�C���ǂݍ��݁B*/
	CSVReader::GetInstance().FileSet(FILE_ID::CONFIG_FILE, "Res/Config/config.ini");
	CSVReader::GetInstance().load(FILE_ID::CONFIG_FILE);
	//�R���t�B�O�f�[�^�i�[�B
	for (int row = 0; row < CSVReader::GetInstance().rows(FILE_ID::CONFIG_FILE); row++){
		config[(CONFIG_DATA)row] = CSVReader::GetInstance().geti(FILE_ID::CONFIG_FILE, row, 1);
	}
	for (int i = 0; i <= 1; i++){
		gaugeLength[i] = 0;
		if (config.at((CONFIG_DATA)i) > 0)
			gaugeLength[i] = config.at((CONFIG_DATA)i) / 10.0f;	
		nextGaugeLength[i] = gaugeLength[i];
		prevGaugeLength[i] = gaugeLength[i];
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
		os_scale[select] = Math::lerp3(os_scale[select], 1.1f, selectScaleTime);
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
	else{
		OptionSelect(frameTime);
	}

}
void Option::Draw(){
	//���݂̃X�N���[���T�C�Y�Ɋg��B
	Vector2 screenPow = vector2(1920.0f / 1280.0f, 1080.0f / 720.0f);
	Graphic::GetInstance().DrawTexture(TEXTURE_ID::BLACK_TEXTURE, vector2(0, 0), vector2(1920, 1080), D3DXCOLOR(1, 1, 1, (allAlpha + manualAlpha)*0.5f), vector2(0, 0));
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

		//BGM��SE�̃Q�[�W�̒��������߂�B
		for (int i = 0; i <= 1; i++){
			gaugeLength[i] = Math::lerp3(prevGaugeLength[i], nextGaugeLength[i], gaugeLerpTime[i]);
		}
		//BGM��SE�̃Q�[�W�̕`��B
		Graphic::GetInstance().DrawTexture(TEXTURE_ID::LINE_WHITE_TEXTURE, vector2(scrCenter.x - 256, scrCenter.y + space * 2), vector2(1, 1), D3DXCOLOR(1, 1, 1, allAlpha*os_alpha[0]), vector2(0, 0));
		Graphic::GetInstance().DrawTexture(TEXTURE_ID::LINE_BLACK_TEXTURE, vector2(scrCenter.x - 256, scrCenter.y + space * 2), vector2(1, 1), D3DXCOLOR(1, 1, 1, allAlpha*os_alpha[0]), vector2(0, 0), 0, 0, gaugeLength[0]);
		Graphic::GetInstance().DrawTexture(TEXTURE_ID::LINE_WHITE_TEXTURE, vector2(scrCenter.x - 256, scrCenter.y + space * 1), vector2(1, 1), D3DXCOLOR(1, 1, 1, allAlpha*os_alpha[0]), vector2(0, 0));
		Graphic::GetInstance().DrawTexture(TEXTURE_ID::LINE_BLACK_TEXTURE, vector2(scrCenter.x - 256, scrCenter.y + space * 1), vector2(1, 1), D3DXCOLOR(1, 1, 1, allAlpha*os_alpha[0]), vector2(0, 0), 0, 0, gaugeLength[1]);
	}
	else{
		Graphic::GetInstance().DrawTexture(TEXTURE_ID::MANUAL_TEXTURE, vector2(1920 / 2, 1080), vector2(1.7f, 1.7f), D3DXCOLOR(1, 1, 1, manualAlpha), vector2(0.5, 1.0f));
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(scrCenter.x - 512 - 128, scrCenter.y + space *-3), vector2(0.4f, 0.4f)*manualBack, 0.5f, "BACK", vector3(1, 1, 1), manualAlpha, false);
	}

	Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(0, 256), vector2(0.3f, 0.3f), 0.5f, "select:" + std::to_string(select), vector3(1, 1, 1), 1.0f);
	Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(0, 256+32), vector2(0.3f, 0.3f), 0.5f, "bgm:" + std::to_string(config.at((CONFIG_DATA)0)), vector3(1, 1, 1), 1.0f);
	Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(0, 256+64), vector2(0.3f, 0.3f), 0.5f, "se:" + std::to_string(config.at((CONFIG_DATA)1)), vector3(1, 1, 1), 1.0f);
	Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(0, 256 + 64+32), vector2(0.3f, 0.3f), 0.5f, "allalpha:" + std::to_string(allAlpha), vector3(1, 1, 1), 1.0f);

}

void Option::Pop(float frameTime){
	isOption = true;//�I�v�V�������B
	isPop = true;//�|�b�v���B
	int max = 120;
	timer = min(timer++, max);
	allAlpha = Math::lerp3(0.0f, 1.0f, (float)(timer / 120.0f));
	//allAlpha = 1.0f;
	if (timer >= max){
		isPop = false;
		timer = 0;
	}
}
void Option::Shut(float frameTime){ 
	int max = 120;
	timer = min(timer++, max);
	allAlpha = Math::lerp3(1.0f, 0.0f, timer / 120.0f);
	selectScaleTime = min((selectScaleTime + 1.0f / 120.0f)*60.0f*frameTime, 1.0f);
	os_scale[5] = Math::lerp(os_scale[5], 1.1f, selectScaleTime);

	if (timer >= max)
		Initialize();
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
	//�ǂݍ��ރt�@�C�����̃Z�b�g�B
	CSVReader::GetInstance().FileSet(FILE_ID::CONFIG_FILE, "Res/Config/config.ini");
	//�R���t�B�O�f�[�^�ǂݍ��݁B
	for (int row = 0; row < (int)strmap.size(); row++){
		strmap[row].push_back(std::to_string(config[(CONFIG_DATA)row]));
	}
	CSVReader::GetInstance().WriteData(FILE_ID::CONFIG_FILE, strmap);
	CSVReader::GetInstance().save(FILE_ID::CONFIG_FILE);

	Audio::GetInstance().SetBGMVolume(config[CONFIG_DATA::MUSIC_VOL] * 10);
	Audio::GetInstance().SetSEVolume(config[CONFIG_DATA::SE_VOL] * 10); 
}

bool Option::IsOption(){
	return isOption;
}

void Option::Move(float frameTime){
	lerpTime = 0;
	timer = 0;
	selectAlphaTime = 0;
	for (int i = 0; i <= OPTION_SELECT_NUM - 1; i++){
		os_prevScale[i] = os_scale[i];
		os_nextScale[i] = OPTION_SCALE;
		os_nextAlpha[i] = OPTION_ALPHA;
		if (select == i){
			os_nextScale[i] = 1.0f;
			os_nextAlpha[i] = 1.0f;
		}
	}
}

void Option::OptionSelect(float frameTime){

	//���ڑI��
	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_UP) &&
		lerpTime == 1.0f){
		select = (OPTION_SELECT)(select - 1);
		select = select < 0 ? select = (OPTION_SELECT)5 : select;
		Move(frameTime);
	}
	else if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_DOWN) &&
		lerpTime == 1.0f){
		select = (OPTION_SELECT)(select + 1);
		select = (OPTION_SELECT)(select%OPTION_SELECT_NUM);
		Move(frameTime);
	}

	//���ʒ���
	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_LEFT, true)){
		if (select != OPTION_SELECT::BGM_SELECT && select != OPTION_SELECT::SE_SELECT)
			return;

		config[(CONFIG_DATA)select] = max(config[(CONFIG_DATA)select]--, 0);
		gaugeLerpTime[select] = 0;
		prevGaugeLength[select] = gaugeLength[select];
		nextGaugeLength[select] = 0;
		if (config.at((CONFIG_DATA)select) > 0)
			nextGaugeLength[select] = config.at((CONFIG_DATA)select) / 10.0f;

		Audio::GetInstance().SetBGMVolume(config[CONFIG_DATA::MUSIC_VOL] * 10);
		Audio::GetInstance().SetSEVolume(config[CONFIG_DATA::SE_VOL] * 10);
}
	else if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_RIGHT, true)){
		if (select != OPTION_SELECT::BGM_SELECT && select != OPTION_SELECT::SE_SELECT)
			return;

		config[(CONFIG_DATA)select] = min(config[(CONFIG_DATA)select]++, 10);
		gaugeLerpTime[select] = 0;
		prevGaugeLength[select] = gaugeLength[select];
		nextGaugeLength[select] = 0;
		if (config.at((CONFIG_DATA)select) > 0)
			nextGaugeLength[select] = config.at((CONFIG_DATA)select) / 10.0f;

		Audio::GetInstance().SetBGMVolume(config[CONFIG_DATA::MUSIC_VOL] * 10);
		Audio::GetInstance().SetSEVolume(config[CONFIG_DATA::SE_VOL] * 10);
	}
	//����
	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_Z, true) ||
		Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_SPACE, true)){
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
	}
	//�L�����Z��
	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_X, true)){
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
	}
	//�e���o��Lerp
	for (int i = 0; i <= OPTION_SELECT_NUM - 1; i++){
		os_scale[i] = Math::lerp4(os_prevScale[i], os_nextScale[i], lerpTime);
		os_alpha[i] = Math::lerp4(os_alpha[i], os_nextAlpha[i], lerpTime);
	}
	lerpTime = min(lerpTime + 1.0f / 10.0f, 1.0f);
	for (int i = 0; i <= 1; i++)
		gaugeLerpTime[i] = min(gaugeLerpTime[i] + 1.0f / 5.0f, 1.0f);
	selectAlphaTime = min(selectAlphaTime + 5 * 60 * frameTime, 360);
	selectAlphaTime %= 360;
}

void Option::Manual(float frameTime){
	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_X, true) ||
		Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_Z, true) ||
		Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_SPACE, true)){
		manualEnd = true;
		os_scale[select] = 1.0f;
		os_alpha[select] = 1.0f;
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