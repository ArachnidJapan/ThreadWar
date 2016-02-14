#include "ResultScene.h"
#include "Scene.h"
#include "../Other/Device.h"
#include "../Graphic/Graphic.h"
#include "../Math/MathUtility.h"
#include "../Actor/Stage.h"
#include "../Actor/CrystalCenter.h"
#include "../TeamSelect/SelectPlayerParam.h"

enum RESULT{
	BLANK_TIME = 60 * 2,
	GAUGE_TIME = 60 * 4,
	VICTRY_TIME = 60 * 2,
};

//コンストラクタ
ResultScene::ResultScene(std::weak_ptr<SceneParameter> sp_) :sp(sp_)
{
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::WHITE_TEXTURE, "Res/Texture/white.png");

}

//デストラクタ
ResultScene::~ResultScene()
{
}

//開始
void ResultScene::Initialize()
{
	mIsEnd = false;

	timer = 0;
	pointTimer = 0;
	vicTimer = 0;

	wa.Initialize();

	/*マップ関係*/
	std::shared_ptr<CrystalCenter> crystalCenter = std::make_shared<CrystalCenter>(wa, ACTOR_ID::CRYSTAL_CENTER_ACTOR,false, true);
	std::shared_ptr<CrystalCenter> crystalPlayerSide = std::make_shared<CrystalCenter>(wa, ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR, false);
	std::shared_ptr<CrystalCenter> crystalEnemySide = std::make_shared<CrystalCenter>(wa, ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR, false);
	wa.Add(ACTOR_ID::CRYSTAL_CENTER_ACTOR, crystalCenter);
	wa.Add(ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR, crystalPlayerSide);
	wa.Add(ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR, crystalEnemySide);
	std::shared_ptr<Stage> stage = std::make_shared<Stage>(wa, crystalCenter, crystalPlayerSide, crystalEnemySide, false);
	wa.Add(ACTOR_ID::STAGE_ACTOR, stage);

	Device::GetInstance().CameraInit(CAMERA_ID::GOD_CAMERA, stage);

	Device::GetInstance().GetCamera(CAMERA_ID::GOD_CAMERA)->GotCamera(vector3(-3.5f, 3, 2.0f), 0);

	points.clear();
	names.clear();
	int charNum = 8;
	int rnd = 0;
	/*for (int i = 0; charNum - 1; i++){
		rnd = rand() % 100;
		points.push_back(rnd);
	}*/
	rnd = rand() % 100;
	points.push_back(rnd);
	rnd = rand() % 100;
	points.push_back(rnd);
	rnd = rand() % 100;
	points.push_back(rnd);
	rnd = rand() % 100;
	points.push_back(rnd);
	rnd = rand() % 100;
	points.push_back(rnd);
	rnd = rand() % 100;
	points.push_back(rnd);
	rnd = rand() % 100;
	points.push_back(rnd);
	rnd = rand() % 100;
	points.push_back(rnd);
	names.push_back("P1");
	names.push_back("CP1");
	names.push_back("CP2");
	names.push_back("CP3");

	names.push_back("NONE");
	names.push_back("CP1");
	names.push_back("CP2");
	names.push_back("NONE");

	//チームポイントゲージ。
	playerTeamPoint = 100;//仮
	enemyTeamPoint = 100;//仮
	float sum = playerTeamPoint + enemyTeamPoint;
	nextTeamPoint = 0.5f;
	if (sum != 0)
		nextTeamPoint = playerTeamPoint / sum;
	teamPoint = 0.5f;
	prevTeamPoint = teamPoint;

	victory = *sp._Get()->ReturnVictoryID();
}

void ResultScene::Update(float frameTime)
{
	timer = min(timer + (1.0f / BLANK_TIME * 60.0f * frameTime), 1.0f);
	
	if (timer == 1.0f){
		pointTimer = min(pointTimer + (1.0f / GAUGE_TIME * 60.0f * frameTime), 1.0f);
	}
	if (pointTimer == 1.0f){
		if ((Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_Z, true) ||
			Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_SPACE, true) ||
			Device::GetInstance().GetInput()->GamePadButtonDown(0, GAMEPADKEY::BUTTON_CURCLE, true)) &&
			vicTimer == 1.0f){
			mIsEnd = true;
		}

		vicTimer = min(vicTimer + (1.0f / VICTRY_TIME * 60.0f * frameTime), 1.0f);
	}
	wa.Update(frameTime);

	teamPoint = Math::lerp(prevTeamPoint, nextTeamPoint, pointTimer);
}

//描画
void ResultScene::Draw() const
{
	wa.Draw(CAMERA_ID::GOD_CAMERA);

	//現在のスクリーンサイズに拡大。
	Vector2 screenPow = vector2(1920.0f / 1280.0f, 1080.0f / 720.0f);

	Graphic::GetInstance().DrawTexture(TEXTURE_ID::DAMAGE_TEXTURE,
		vector2(1920 / 2, 1080 / 2),
		vector2(1.5f, 1.5f),
		D3DXCOLOR(1, 1, 1, 0.5f),
		vector2(0.5f, 0.5f),
		0.0f,
		0.0f,
		1,
		1.0f);

	Graphic::GetInstance().DrawTexture(TEXTURE_ID::TEAM_TEXTURE,
		vector2(1920 / 2, 1080 / 2),
		vector2(1, 1),
		D3DXCOLOR(1, 1, 1, 1.0f),
		vector2(0.5f, 0.5f),
		0.0f,
		0.0f,
		1,
		1.0f);

	Graphic::GetInstance().DrawTexture(TEXTURE_ID::WHITE_BAR_TEXTURE,
		vector2(1920 / 2, 1080 / 2),
		vector2(1.5f, 1.5f),
		D3DXCOLOR(1, 1, 1, 0.5f),
		vector2(0.5f, 0.5f),
		0.0f,
		0.0f,
		1,
		1.0f);

	//各チームゲージ
	Graphic::GetInstance().DrawTexture(TEXTURE_ID::GAUGE_TEXTURE, vector2(1920 / 2, 1080 - 56), vector2(1.3f, 1.3f), D3DXCOLOR(1, 0, 0, 1), vector2(0.5f, 0.5f), 0.0f, 0.0f, teamPoint, 1.0f);
	Graphic::GetInstance().DrawTexture(TEXTURE_ID::GAUGE_TEXTURE, vector2(1920 / 2, 1080 - 56), vector2(1.3f, 1.3f), D3DXCOLOR(0, 0, 1, 1), vector2(0.5f, 0.5f), teamPoint, 0.0f, 1.0f, 1.0f);
	//真ん中の黒棒
	Graphic::GetInstance().DrawTexture(TEXTURE_ID::GAUGE_CENTER_TEXTURE, vector2(1920 / 2 + ((teamPoint-0.5f)*1024*1.3f), 1080 - 56), vector2(1, 1), D3DXCOLOR(1, 1, 1, 1), vector2(0.5f, 0.5f), 0, 0.0f, 1.0f, 1.0f);
	//数値
	int psp = Math::lerp(0, playerTeamPoint, pointTimer);
	int esp = Math::lerp(0, enemyTeamPoint, pointTimer);
	Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(1920.0f / 2.0f - 600.0f, 1080.0f - 60.0f), vector2(0.3f, 0.5f) * 1.5f, 0.5f, std::to_string(psp) + "pts", vector3(1, 0, 0), 1.0f, true);
	Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(1920.0f / 2.0f + 600.0f, 1080.0f - 60.0f), vector2(0.3f, 0.5f) * 1.5f, 0.5f, std::to_string(esp) + "pts", vector3(0, 0, 1), 1.0f, true);
	

	bool red = true;
	int SpiderNum = 7;
	int redCount=0, blueCount=0;
	bool isTarantula = true;
	for (int i = 0; i <= SpiderNum; i++){
		TEXTURE_ID tID;
		Vector2 pos;
		int point = rand() % 100;
		if (red){
			if (isTarantula){
				tID = TEXTURE_ID::TARENTULA_RED_TEXTURE;
			}
			else{
				tID = TEXTURE_ID::NEPHILA_RED_TEXTURE;
			}
			pos = vector2(1920 / 4 * (1 + 2 * !red) + (150*(1 * (-redCount % 2))), 1080 - 300 - (200 * redCount));
			redCount++;
		}
		else{
			if (isTarantula){
				tID = TEXTURE_ID::TARENTULA_BLUE_TEXTURE;
			}
			else{
				tID = TEXTURE_ID::NEPHILA_BLUE_TEXTURE;
			}
			pos = vector2(1920 / 4 * (1 + 2 * !red) - (150 * (1 * (-blueCount % 2))), 1080 - 300 - (200 * blueCount));
			blueCount++;
		}
		Graphic::GetInstance().DrawTexture(tID,
			vector2(pos.x, pos.y),
			vector2(1.0f, 1.0f),
			D3DXCOLOR(1, 1, 1, 1),
			vector2(0.5f, 0.5f),
			0.0f,
			0.0f,
			1,
			1.0f);

		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT,
			pos,
			vector2(0.3f, 0.6f),
			0.6f,
			names.at(i),
			names.at(i) == "P1" ?
			red ?
			vector3(1, 0, 0) : vector3(0, 0, 1) :
			names.at(i) == "NONE" ?
			vector3(1, 1, 1) : vector3(0, 1, 0),
			1,
			true);
		pos.y += -64;
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT,
			pos,
			vector2(0.40f, 0.4f),
			0.6f,
			std::to_string(points.at(i)) + " pts",
			red ?
			vector3(1, 0, 0) : vector3(0, 0, 1),
			1,
			true);

		if (redCount >= 4)
			red = false;
	}
	if (pointTimer == 1.0f){
		sp._Get()->SetVictoryID(VICTORY_ID::DRAW_WIN);
		if (*sp._Get()->ReturnVictoryID() == VICTORY_ID::PLAYER_WIN){
			Graphic::GetInstance().DrawTexture(TEXTURE_ID::WHITE_TEXTURE, vector2(0, 0), vector2(1920, 1080), D3DXCOLOR(1, 0, 0, vicTimer * 0.5f), vector2(0.0f, 0.0f), 0, 0, 1.0f, 1.0f, 0);
			Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(1920 / 2.0f, 1080 / 2.0f), vector2(0.6f, 0.6f), 0.5f, "RED TEAM WINS !!", vector3(1, 0, 0), vicTimer, true);
		}
		else if (*sp._Get()->ReturnVictoryID() == VICTORY_ID::ENEMY_WIN){
			Graphic::GetInstance().DrawTexture(TEXTURE_ID::WHITE_TEXTURE, vector2(0, 0), vector2(1920, 1080), D3DXCOLOR(0, 0, 1, vicTimer * 0.5f), vector2(0.0f, 0.0f), 0, 0, 1.0f, 1.0f, 0);
			Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(1920 / 2.0f, 1080 / 2.0f), vector2(0.6f, 0.6f), 0.5f, "BLUE TEAM WINS !!", vector3(0, 0, 1), vicTimer, true);
		}
		else if (*sp._Get()->ReturnVictoryID() == VICTORY_ID::DRAW_WIN){
			Graphic::GetInstance().DrawTexture(TEXTURE_ID::WHITE_TEXTURE, vector2(0, 0), vector2(1920, 1080), D3DXCOLOR(1, 1, 1, vicTimer * 0.5f), vector2(0.0f, 0.0f), 0, 0, 1.0f, 1.0f, 0);
			Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(1920 / 2.0f, 1080 / 2.0f), vector2(0.6f, 0.6f), 0.5f, "DRAW", vector3(1, 1, 1), vicTimer, true);
		}
	}
	/*Graphic::GetInstance().DrawTexture(TEXTURE_ID::DAMAGE_TEXTURE, vector2(0, 0), screenPow, D3DXCOLOR(1, 1, 1, 1), vector2(0.0f, 0.0f), 0, 0, 1.0f, 1.0f, 0);
	if (*sp._Get()->ReturnVictoryID() == VICTORY_ID::PLAYER_WIN)
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(1920 / 2.0f, 1080 / 2.0f), vector2(0.6f, 0.6f), 0.5f, "PLAYER TEAM WINS", vector3(1, 0, 0), 1.0f, true);
	else if (*sp._Get()->ReturnVictoryID() == VICTORY_ID::ENEMY_WIN)
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(1920 / 2.0f, 1080 / 2.0f), vector2(0.6f, 0.6f), 0.5f, "ENEMY TEAM WINS", vector3(0, 0, 1), 1.0f, true);
	else if (*sp._Get()->ReturnVictoryID() == VICTORY_ID::DRAW_WIN)
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(1920 / 2.0f, 1080 / 2.0f), vector2(0.6f, 0.6f), 0.5f, "DRAW", vector3(0, 1, 0), 1.0f, true);

	float backAlpha = Math::lerp3(1.0f, 0.0f, pointTimer);
	Graphic::GetInstance().DrawTexture(TEXTURE_ID::BLACK_TEXTURE, vector2(0, 0), vector2(1920, 1080), D3DXCOLOR(1, 1, 1, backAlpha), vector2(0.0f, 0.0f), 0, 0, 1.0f, 1.0f, 0);*/
}

//終了しているか？
bool ResultScene::IsEnd() const
{
	return mIsEnd;
}

//次のシーンを返す
Scene ResultScene::Next() const
{
	return Scene::Demo;
}


void ResultScene::End(){
}