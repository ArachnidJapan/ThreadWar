#include "TitleScene.h"
#include <sstream>
#include <iomanip>
#include "Scene.h"
#include "../actor/Player.h"
#include "../Other/Device.h"
#include "../Graphic/Graphic.h"
#include "../Math/MathUtility.h"
#include "../Math/Vector2Utility.h"
#include "../Actor/Stage.h"
#include "../Actor/CrystalCenter.h"
#include "../Math/Converter.h"
#include "../Audio/Audio.h"

#define TITLE_SELECT_NUM 2
#define MOVE_AMOUNT 256
#define SCREEN_CENTER_X 1920/2
#define SCREEN_CENTER_Y 1080/2
#define DEFAULT_X SCREEN_CENTER_X - 256

//コンストラクタ
TitleScene::TitleScene(std::weak_ptr<SceneParameter> sp_, std::weak_ptr<Option> option_) :
sp(sp_),
option(option_)
{
	/************************************************テクスチャー*************************************************/
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::TITLE_BACK_TEXTURE, "Res/Texture/test_back.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::MENU_BLACK_TEXTURE, "Res/Texture/menu_black.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::THREAD_BACK_TEXTURE, "Res/Texture/thread_back.png");

	Graphic::GetInstance().LoadTexture(TEXTURE_ID::MANUAL_TEXTURE, "Res/Texture/manual1.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::MANUAL2_TEXTURE, "Res/Texture/manual2.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::MANUAL3_TEXTURE, "Res/Texture/manual3.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::MANUAL4_TEXTURE, "Res/Texture/manual4.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::LINE_WHITE_TEXTURE, "Res/Texture/line_white.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::LINE_BLACK_TEXTURE, "Res/Texture/line_black.png");

	//2つの選択肢のパラメータを初期化。
	for (int i = 0; i <= 1; i++){
		ts_scale.push_back(0.6f);
		ts_alpha.push_back(0.5f);
		ts_nextScale.push_back(ts_scale.at(i));
		ts_prevScale.push_back(ts_scale.at(i));
		ts_nextAlpha.push_back(ts_alpha.at(i));
	}
}

//デストラクタ
TitleScene::~TitleScene()
{
	ts_scale.clear();
	ts_nextScale.clear();
	ts_prevScale.clear();
	ts_alpha.clear();
	ts_nextAlpha.clear();
}

//開始
void TitleScene::Initialize()
{
	Audio::GetInstance().PlayBGM(BGM_ID::MENU_BGM, true);
	/*マップ関係*/
	std::shared_ptr<CrystalCenter> crystalCenter = std::make_shared<CrystalCenter>(wa, ACTOR_ID::CRYSTAL_CENTER_ACTOR, false, true,1000);
	std::shared_ptr<CrystalCenter> crystalPlayerSide = std::make_shared<CrystalCenter>(wa, ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR, false,true,-1000);
	std::shared_ptr<CrystalCenter> crystalEnemySide = std::make_shared<CrystalCenter>(wa, ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR, false, true, -1000);
	wa.Add(ACTOR_ID::CRYSTAL_CENTER_ACTOR, crystalCenter);
	wa.Add(ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR, crystalPlayerSide);
	wa.Add(ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR, crystalEnemySide);
	std::shared_ptr<Stage> stage = std::make_shared<Stage>(wa, crystalCenter, crystalPlayerSide, crystalEnemySide,false);
	wa.Add(ACTOR_ID::STAGE_ACTOR, stage);
	Device::GetInstance().CameraInit(CAMERA_ID::GOD_CAMERA, stage);

	Device::GetInstance().GetCamera(CAMERA_ID::GOD_CAMERA)->GotCamera(vector3(-7.0f, 2.5f, 3.0f), 0);/***********/

	mIsEnd = false;
	isSelect = false;
	timer = 0;
	lerpTime = 1;
	alphaTime = 90;
	allAlpha = 1.0f;
	selects = SELECT_GAMESTART;
	teamID = TEAM_ID::FIRST_TEAM;
	threadBackPos = vector2(SCREEN_CENTER_X, SCREEN_CENTER_Y + MOVE_AMOUNT / 2.0f + (selects * -MOVE_AMOUNT));

	//2つの選択肢のパラメータを初期化。
	for (int i = 0; i <= 1; i++){
		ts_scale.at(i) = 0.6f;
		ts_alpha.at(i) = 0.5f;
		if (selects == (TITLE_SELECT)i){
			ts_scale.at(i) = 1.0f;
			ts_alpha.at(i) = 1.0f;
		}
		ts_nextScale.at(i) = ts_scale.at(i);
		ts_prevScale.at(i) = ts_scale.at(i);
		ts_nextAlpha.at(i) = ts_alpha.at(i);
	}
	option._Get()->Initialize();
}

void TitleScene::Update(float frameTime)
{
	wa.Update(frameTime);

	if (option._Get()->IsOption()){
		option._Get()->Update(frameTime);
		if (option._Get()->ReturnMenu()){
			mIsEnd = true;
			selects == TITLE_SELECT::SELECT_RETURN;
		}
		return;
	}
	//PRESS_STARTが押されたら、"ゲームプレイ"、"オプション"の2つの選択肢。
	if (!isSelect){
		timer += 5 * 60 * frameTime;
		timer = fmodf(timer, 360.0f);
		TitleSelect(frameTime);
	}
	//2つの選択肢の一つが選択されたら、チーム選択。
	else if (isSelect){
		if (selects == TITLE_SELECT::SELECT_GAMESTART)
			mIsEnd = true;
		else if (selects == TITLE_SELECT::SELECT_OPTION){
			timer = min(timer + 1.0f / 60.0f*60.0f*frameTime, 1.0f);
			allAlpha = Math::lerp3(1.0f, 0.0f, timer);
			ts_scale.at(selects) = Math::lerp3(ts_scale.at(selects), 1.1f, timer);
			if (timer == 1.0f){
				option._Get()->Pop(frameTime);
				isSelect = false;
			}
		}
	}
}

//描画
void TitleScene::Draw() const
{
	wa.Draw(CAMERA_ID::GOD_CAMERA);
	if (option._Get()->IsOption()){
		option._Get()->Draw();
	}
	else{
		//現在のスクリーンサイズに拡大。
		Vector2 screenPow = vector2(1920.0f / 1280.0f, 1080.0f / 720.0f);
		Graphic::GetInstance().DrawTexture(TEXTURE_ID::MENU_BLACK_TEXTURE, vector2(0, 0), screenPow, D3DXCOLOR(1, 1, 1, allAlpha), vector2(0, 0));
		Graphic::GetInstance().DrawTexture(TEXTURE_ID::THREAD_BACK_TEXTURE, threadBackPos, vector2(1.5f, 1.5f), D3DXCOLOR(1, 1, 1, allAlpha));
		//文字
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(SCREEN_CENTER_X, SCREEN_CENTER_Y + MOVE_AMOUNT / 2), vector2(1.0f, 1.0f)*ts_scale.at(0), 0.5f, "START GAME", vector3(1, 1, 1), ts_alpha.at(0) * allAlpha, true);
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(SCREEN_CENTER_X, SCREEN_CENTER_Y - MOVE_AMOUNT / 2), vector2(1.0f, 1.0f)*ts_scale.at(1), 0.5f, "OPTIONS", vector3(1, 1, 1), ts_alpha.at(1) * allAlpha, true);
	}
}

//終了しているか？
bool TitleScene::IsEnd() const
{
	return mIsEnd;
}

//次のシーンを返す
Scene TitleScene::Next() const
{
	if (selects == TITLE_SELECT::SELECT_GAMESTART)
		return Scene::TeamSelect;
	else
		return Scene::Demo;
}

void TitleScene::TitleSelect(float frameTime){
	if ((Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_UP) ||
		Device::GetInstance().GetInput()->LeftStick(0, true).z >= 0.5f) &&
		lerpTime == 1.0f){
		selects = (TITLE_SELECT)(selects - 1);
		selects = (TITLE_SELECT)(selects < 0 ? 1 : selects);
		Move();
		Audio::GetInstance().PlaySE(SE_ID::SWITCH_SE);
	}
	if ((Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_DOWN) ||
		Device::GetInstance().GetInput()->LeftStick(0, true).z <= -0.5f) &&
		lerpTime == 1.0f){
		selects = (TITLE_SELECT)(selects + 1);
		selects = (TITLE_SELECT)(selects % TITLE_SELECT_NUM);
		Move();
		Audio::GetInstance().PlaySE(SE_ID::SWITCH_SE);
	}
	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_Z, true) ||
		Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_SPACE, true) ||
		Device::GetInstance().GetInput()->GamePadButtonDown(0, GAMEPADKEY::BUTTON_CURCLE, true)){
		isSelect = true;
		timer = 0;
		Audio::GetInstance().PlaySE(SE_ID::ENTER_SE);
	}
	else if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_X, true) ||
		Device::GetInstance().GetInput()->GamePadButtonDown(0, GAMEPADKEY::BUTTON_CROSS, true)){
		mIsEnd = true;
		selects = TITLE_SELECT::SELECT_RETURN;
		Audio::GetInstance().PlaySE(SE_ID::BACK_SE);
	}

	for (int i = 0; i <= TITLE_SELECT_NUM - 1; i++){
		ts_scale.at(i) = Math::lerp3(ts_prevScale.at(i), ts_nextScale.at(i), lerpTime);
		ts_alpha.at(i) = Math::lerp3(ts_alpha.at(i), ts_nextAlpha.at(i), lerpTime);
	}

	lerpTime = min(lerpTime + 1.0f / 15.0f, 1.0f);
	alphaTime += 2;
	alphaTime = fmodf(alphaTime, 360.0f);
	//ts_alpha[selects] = abs(Math::sin(alphaTime));
	allAlpha = min(allAlpha + 1.0f / 30.0f*60.0f*frameTime, 1.0f);
}

void TitleScene::TeamSelect(){
	switch (selects)
	{
	case SELECT_OPTION:
		mIsEnd = true;
		break;
	default:
		if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_RIGHT)){
			teamID = TEAM_ID::FIRST_TEAM;
		}
		if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_LEFT)){
			teamID = TEAM_ID::SECOND_TEAM;
		}
		if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_Z, true) ||
			Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_SPACE, true)){
			mIsEnd = true;
		}
		else if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_X, true))
			isSelect = false;
		break;
	}
}

void TitleScene::Move(){
	lerpTime = 0;

	for (int i = 0; i <= TITLE_SELECT_NUM - 1; i++){
		ts_prevScale.at(i) = ts_scale.at(i);

		ts_nextScale.at(i) = 0.6f;
		ts_nextAlpha.at(i) = 0.5f;
		if (selects == (TITLE_SELECT)i){
			ts_nextScale.at(i) = 1.0f;
			ts_nextAlpha.at(i) = 1.0f;
		}
		ts_alpha.at(i) = 0.5f;
	}
	threadBackPos = vector2(SCREEN_CENTER_X, SCREEN_CENTER_Y + MOVE_AMOUNT / 2.0f + (selects * -MOVE_AMOUNT));
}

void TitleScene::End(){
	if (selects != TITLE_SELECT::SELECT_GAMESTART)
		Audio::GetInstance().StopBGM(BGM_ID::MENU_BGM);
	wa.Clear();
}