#include "DemoScene.h"
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
#include "../Reader/CSVReader.h"
#include <vector>

#define SCREEN_CENTER_X 1920/2
#define SCREEN_CENTER_Y 1080/2

//各演出にかかる時間
#define BACK_ALPHA_TIME 20
#define BACK_BLANK_TIME 30
#define LOGO_ALPHA_TIME 60*4
#define START_ALPHA_TIME 20
#define CHANGE_SCENE_TIME 20

//コンストラクタ
DemoScene::DemoScene(std::weak_ptr<SceneParameter> sp_) :sp(sp_)
{
	/************************************************テクスチャー*************************************************/
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::TITLE_LOGO_TEXTURE, "Res/Texture/title.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::BLACK_TEXTURE, "Res/Texture/black.png");
	//Audio::GetInstance().LoadSE(SE_ID::)
}

//デストラクタ
DemoScene::~DemoScene()
{
}

//開始
void DemoScene::Initialize()
{
	/*マップ関係*/
	std::shared_ptr<CrystalCenter> crystalCenter = std::make_shared<CrystalCenter>(wa, ACTOR_ID::CRYSTAL_CENTER_ACTOR, false);
	std::shared_ptr<CrystalCenter> crystalPlayerSide = std::make_shared<CrystalCenter>(wa, ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR, false);
	std::shared_ptr<CrystalCenter> crystalEnemySide = std::make_shared<CrystalCenter>(wa, ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR, false);
	wa.Add(ACTOR_ID::CRYSTAL_CENTER_ACTOR, crystalCenter);
	wa.Add(ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR, crystalPlayerSide);
	wa.Add(ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR, crystalEnemySide);
	std::shared_ptr<Stage> stage = std::make_shared<Stage>(wa, crystalCenter, crystalPlayerSide, crystalEnemySide,false);
	wa.Add(ACTOR_ID::STAGE_ACTOR, stage);

	Device::GetInstance().CameraInit(CAMERA_ID::PLAYER_CAMERA_1P, stage);
	Device::GetInstance().CameraInit(CAMERA_ID::PLAYER_CAMERA_2P, stage);
	Device::GetInstance().CameraInit(CAMERA_ID::PLAYER_CAMERA_3P, stage);
	Device::GetInstance().CameraInit(CAMERA_ID::PLAYER_CAMERA_4P, stage);
	Device::GetInstance().CameraInit(CAMERA_ID::ENEMY_CAMERA_5P, stage);
	Device::GetInstance().CameraInit(CAMERA_ID::ENEMY_CAMERA_6P, stage);
	Device::GetInstance().CameraInit(CAMERA_ID::ENEMY_CAMERA_7P, stage);
	Device::GetInstance().CameraInit(CAMERA_ID::ENEMY_CAMERA_8P, stage);
	Device::GetInstance().CameraInit(CAMERA_ID::GOD_CAMERA, stage);

	//プレイヤーチームを追加
	Device::GetInstance().GetCamera(CAMERA_ID::PLAYER_CAMERA_1P)->SetPadNum(0);
	Device::GetInstance().GetCamera(CAMERA_ID::PLAYER_CAMERA_2P)->SetPadNum(1);
	Device::GetInstance().GetCamera(CAMERA_ID::PLAYER_CAMERA_3P)->SetPadNum(2);
	Device::GetInstance().GetCamera(CAMERA_ID::PLAYER_CAMERA_4P)->SetPadNum(3);
	wa.Add(ACTOR_ID::PLAYER_ACTOR, std::make_shared<Player>(wa, stage, CAMERA_ID::PLAYER_CAMERA_1P, 0, 0,false,false));
	wa.Add(ACTOR_ID::PLAYER_ACTOR, std::make_shared<Player>(wa, stage, CAMERA_ID::PLAYER_CAMERA_2P, 1, 1,false,false));
	wa.Add(ACTOR_ID::PLAYER_ACTOR, std::make_shared<Player>(wa, stage, CAMERA_ID::PLAYER_CAMERA_3P, 2, 2,false,false));
	wa.Add(ACTOR_ID::PLAYER_ACTOR, std::make_shared<Player>(wa, stage, CAMERA_ID::PLAYER_CAMERA_4P, 3, 3,false,false));
	//敵チームを追加
	Device::GetInstance().GetCamera(CAMERA_ID::ENEMY_CAMERA_5P)->SetPadNum(4);
	Device::GetInstance().GetCamera(CAMERA_ID::ENEMY_CAMERA_6P)->SetPadNum(5);
	Device::GetInstance().GetCamera(CAMERA_ID::ENEMY_CAMERA_7P)->SetPadNum(6);
	Device::GetInstance().GetCamera(CAMERA_ID::ENEMY_CAMERA_8P)->SetPadNum(7);
	wa.Add(ACTOR_ID::ENEMY_ACTOR, std::make_shared<Player>(wa, stage, CAMERA_ID::ENEMY_CAMERA_5P, 4, 4,false,true));
	wa.Add(ACTOR_ID::ENEMY_ACTOR, std::make_shared<Player>(wa, stage, CAMERA_ID::ENEMY_CAMERA_6P, 5, 5,false,true));
	wa.Add(ACTOR_ID::ENEMY_ACTOR, std::make_shared<Player>(wa, stage, CAMERA_ID::ENEMY_CAMERA_7P, 6, 6,false,true));
	wa.Add(ACTOR_ID::ENEMY_ACTOR, std::make_shared<Player>(wa, stage, CAMERA_ID::ENEMY_CAMERA_8P, 7, 7,false,true));

	Device::GetInstance().GetCamera(CAMERA_ID::GOD_CAMERA)->GotCamera(vector3(-2.4f, 3.3f, -75.0f), 0,0);
	/***********/
	/*コンフィグファイル読み込み。*/
	CSVReader::GetInstance().FileSet(FILE_ID::CONFIG_FILE, "Res/Config/config.ini");
	CSVReader::GetInstance().load(FILE_ID::CONFIG_FILE);
	configData.clear();
	//コンフィグデータ格納。
	for (int row = 0; row < CSVReader::GetInstance().rows(FILE_ID::CONFIG_FILE); row++){
		configData.push_back(CSVReader::GetInstance().geti(FILE_ID::CONFIG_FILE, row, 1));
	}
	//音量の設定。
	Audio::GetInstance().SetAllBGMVolume(configData.at(0) * 10);
	Audio::GetInstance().SetAllSEVolume(configData.at(1) * 10);

	Audio::GetInstance().PlayBGM(BGM_ID::TITLE_BGM, true);
	moveVec = vector3(0, 0, 0);
	mIsEnd = false;
	pressStart = false;
	effectEnd = false;
	timer = 0;
	logoLerp = 0;
	pressLerp = 0;
	blackLerp = 0;
	changeFlag = false;
	moveRes = false;
}

void DemoScene::Update(float frameTime)
{
	wa.Update(frameTime);
	//Device::GetInstance().GetCamera(CAMERA_ID::PLAYER_CAMERA_1P)->SetCamera(vector3(-10, 0.0f, -10.0f), vector3(0, 0, 0), 1.0f / 60.0f);
	Device::GetInstance().GetCamera(CAMERA_ID::PLAYER_CAMERA_1P)->SetCamera(vector3(0, 0.0f, -3.0f), vector3(0, 0, 0), frameTime);
	Device::GetInstance().GetCamera(CAMERA_ID::PLAYER_CAMERA_2P)->SetCamera(vector3(0, 0.0f, -3.0f), vector3(0, 0, 0), frameTime);
	Device::GetInstance().GetCamera(CAMERA_ID::PLAYER_CAMERA_3P)->SetCamera(vector3(0, 0.0f, -3.0f), vector3(0, 0, 0), frameTime);
	Device::GetInstance().GetCamera(CAMERA_ID::PLAYER_CAMERA_4P)->SetCamera(vector3(0, 0.0f, -3.0f), vector3(0, 0, 0), frameTime);
	Device::GetInstance().GetCamera(CAMERA_ID::ENEMY_CAMERA_5P)->SetCamera(vector3(0, 0.0f, -3.0f), vector3(0, 0, 0), frameTime);
	Device::GetInstance().GetCamera(CAMERA_ID::ENEMY_CAMERA_6P)->SetCamera(vector3(0, 0.0f, -3.0f), vector3(0, 0, 0), frameTime);
	Device::GetInstance().GetCamera(CAMERA_ID::ENEMY_CAMERA_7P)->SetCamera(vector3(0, 0.0f, -3.0f), vector3(0, 0, 0), frameTime);
	Device::GetInstance().GetCamera(CAMERA_ID::ENEMY_CAMERA_8P)->SetCamera(vector3(0, 0.0f, -3.0f), vector3(0, 0, 0), frameTime);
	//Device::GetInstance().GetCamera(CAMERA_ID::GOD_CAMERA)->GotCamera(frameTime);
	Device::GetInstance().GetCamera(CAMERA_ID::GOD_CAMERA)->GotCamera(vector3(10.4f, 3.3f, -80.0f) + moveVec, 0, -40);

	moveVec += vector3(0, 0, 15) * frameTime;

	if (moveVec.z > 150.0f){
		changeFlag = true;
	}
	if (changeFlag){
		blackLerp += 80.0f * frameTime;
		if (blackLerp > 180.0f){
			blackLerp = 0;
			changeFlag = false;
			moveRes = false;
		}
		else if (blackLerp > 90.0f){
			if (!moveRes){
				moveRes = true;
				moveVec = vector3(0, 0, 0);
			}
		}
	}

	//最初のPRESS_STARTが押されていなければ、PRESS_START文字を明滅。
	if (!effectEnd){
		if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_Z, true) ||
			Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_SPACE, true) ||
			Device::GetInstance().GetInput()->GamePadAnyButton(0,true)){
			pressStart = false;
			effectEnd = false;
			timer = BACK_ALPHA_TIME + BACK_BLANK_TIME + LOGO_ALPHA_TIME + START_ALPHA_TIME;
			logoLerp = 1.0f;
			pressLerp = 1.0f;
		}

		if (timer >= BACK_ALPHA_TIME + BACK_BLANK_TIME){
			logoLerp = min(logoLerp + (1.0f / (60.0f*4.0f) * 60.0f*frameTime), 1.0f);
		}
		if (timer >= BACK_ALPHA_TIME + BACK_BLANK_TIME + LOGO_ALPHA_TIME){
			pressLerp = min(pressLerp + (1.0f / 20.0f) * 60.0f*frameTime, 1.0f);
		}
		if (timer >= BACK_ALPHA_TIME + BACK_BLANK_TIME + LOGO_ALPHA_TIME + START_ALPHA_TIME){
			effectEnd = true;
			timer = 0;
		}
		timer += 60 * frameTime;

	}
	else if (effectEnd && !pressStart){
		timer += 3 * 60 * frameTime;
		timer = fmodf(timer, 360.0f);
		pressLerp = Math::sin(timer) + 1.0f;

		if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_Z, true) ||
			Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_SPACE, true) ||
			Device::GetInstance().GetInput()->GamePadAnyButton(0,true)){
			mIsEnd = true;
		}
		//if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_X, true) ||
		//	Device::GetInstance().GetInput()->GamePadButtonDown(0, GAMEPADKEY::BUTTON_CROSS, true)){
		//	Initialize();
		//}
	}
}

//描画
void DemoScene::Draw() const
{
	wa.Draw(CAMERA_ID::GOD_CAMERA);
	//現在のスクリーンサイズに拡大。
	Vector2 screenPow = vector2(1920.0f / 1280.0f, 1080.0f / 768.0f);

	Graphic::GetInstance().DrawTexture(TEXTURE_ID::BLACK_TEXTURE, vector2(SCREEN_CENTER_X, SCREEN_CENTER_Y), vector2(1920,1080), D3DXCOLOR(1, 1, 1,abs(Math::sin(blackLerp))), vector2(0.5f, 0.5f), 0, 0, 1.0f, 1.0f, 0);

	float logoAlpha = Math::lerp3(0.0f, 1.0f, logoLerp);
	Graphic::GetInstance().DrawTexture(TEXTURE_ID::TITLE_LOGO_TEXTURE, vector2(SCREEN_CENTER_X, SCREEN_CENTER_Y), screenPow, D3DXCOLOR(1, 1, 1, logoLerp), vector2(0.5f, 0.5f), 0, 0, 1.0f, 1.0f, 0);

	float startAlpha = Math::lerp3(0.0f, 1.0f, pressLerp);
	Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(SCREEN_CENTER_X, SCREEN_CENTER_Y - 256), vector2(0.5f, 0.5f), 0.5f, "press any button", vector3(1, 1, 1), pressLerp, true);
}

//終了しているか？
bool DemoScene::IsEnd() const
{
	return mIsEnd;
}

//次のシーンを返す
Scene DemoScene::Next() const
{
	return Scene::Title;
}


void DemoScene::End(){
	wa.Clear();
}