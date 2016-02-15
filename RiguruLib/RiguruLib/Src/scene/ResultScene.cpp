#include "ResultScene.h"
#include "Scene.h"
#include "../Other/Device.h"
#include "../Graphic/Graphic.h"
#include "../Math/MathUtility.h"
#include "../Actor/Stage.h"
#include "../Actor/CrystalCenter.h"
#include "../TeamSelect/SelectPlayerParam.h"
#include "../Audio/Audio.h"

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
	Audio::GetInstance().PlayBGM(BGM_ID::RESULT_BGM);

	timer = 0;
	pointTimer = 0;
	vicTimer = 0;
	pressTimer = 0;

	wa.Initialize();

	/*マップ関係*/
	std::shared_ptr<CrystalCenter> crystalCenter = std::make_shared<CrystalCenter>(wa, ACTOR_ID::CRYSTAL_CENTER_ACTOR, false, true, 1000);
	std::shared_ptr<CrystalCenter> crystalPlayerSide = std::make_shared<CrystalCenter>(wa, ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR, false, true, -1000);
	std::shared_ptr<CrystalCenter> crystalEnemySide = std::make_shared<CrystalCenter>(wa, ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR, false, true, -1000);
	wa.Add(ACTOR_ID::CRYSTAL_CENTER_ACTOR, crystalCenter);
	wa.Add(ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR, crystalPlayerSide);
	wa.Add(ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR, crystalEnemySide);
	std::shared_ptr<Stage> stage = std::make_shared<Stage>(wa, crystalCenter, crystalPlayerSide, crystalEnemySide, false);
	wa.Add(ACTOR_ID::STAGE_ACTOR, stage);

	Device::GetInstance().CameraInit(CAMERA_ID::GOD_CAMERA, stage);

	Device::GetInstance().GetCamera(CAMERA_ID::GOD_CAMERA)->GotCamera(vector3(-7.0f, 2.5f, 3.0f), 0);

	redPoints.clear();
	bluePoints.clear();
	nextRedPoints.clear();
	nextBluePoints.clear();
	redNames.clear();
	blueNames.clear();

	playerTeamPoint = 0;
	enemyTeamPoint = 0;
	redNum = 0;
	blueNum = 0;
	spiderNum = 0;
	pressLerp = 0;
	pressTimer = 0;
	for (int i = 0; i <= 3; i++){
		if (i <= sp._Get()->ReturnTeamSelectResult()->redHaveCPU +
			sp._Get()->ReturnTeamSelectResult()->redHavePlayer - 1){
			nextRedPoints.push_back(sp._Get()->ReturnTeamSelectResult()->redTeamPoint[i]);
			playerTeamPoint += sp._Get()->ReturnTeamSelectResult()->redTeamPoint[i];
			spiderNum++;
			redNum++;
		}
		else{
			nextRedPoints.push_back(0);
		}
		redPoints.push_back(0);
	}
	for (int i = 0; i <= 3; i++){
		if (i <= sp._Get()->ReturnTeamSelectResult()->blueHaveCPU +
			sp._Get()->ReturnTeamSelectResult()->blueHavePlayer - 1){
			nextBluePoints.push_back(sp._Get()->ReturnTeamSelectResult()->blueTeamPoint[i]);
			enemyTeamPoint += sp._Get()->ReturnTeamSelectResult()->blueTeamPoint[i];
			spiderNum++;
			blueNum++;
		}
		else{
			nextBluePoints.push_back(0);
		}
		bluePoints.push_back(0);
	}
	//P1,CP1等の名前のプッシュバック。
	if (sp._Get()->ReturnTeamSelectResult()->redHavePlayer)
		redNames.push_back("P1");
	if (sp._Get()->ReturnTeamSelectResult()->redHaveCPU > 0){
		for (int i = 1; i <= sp._Get()->ReturnTeamSelectResult()->redHaveCPU; i++)
			redNames.push_back("CP" + std::to_string(i));
	}
	if (redNum != 4){
		for (int i = 0; i <= 4 - redNum - 1; i++)
			redNames.push_back("NONE");
	}

	if (sp._Get()->ReturnTeamSelectResult()->blueHavePlayer)
		blueNames.push_back("P1");
	if (sp._Get()->ReturnTeamSelectResult()->blueHaveCPU > 0){
		for (int i = 1; i <= sp._Get()->ReturnTeamSelectResult()->blueHaveCPU; i++)
			blueNames.push_back("CP" + std::to_string(i));
	}
	if (blueNum != 4){
		for (int i = 0; i <= 4 - blueNum - 1; i++)
			blueNames.push_back("NONE");
	}

	//チームポイントゲージ。
	float sum = playerTeamPoint + enemyTeamPoint;
	nextTeamPoint = 0.5f;
	if (sum != 0)
		nextTeamPoint = playerTeamPoint / sum;
	teamPoint = 0.5f;

	redTarantula = sp._Get()->ReturnTeamSelectResult()->redTarantula;
	blueTarantula = sp._Get()->ReturnTeamSelectResult()->blueTarantula;

	if (playerTeamPoint == enemyTeamPoint)
		victory = VICTORY_ID::DRAW_WIN;
	else if (playerTeamPoint > enemyTeamPoint)
		victory = VICTORY_ID::PLAYER_WIN;
	else
		victory = VICTORY_ID::ENEMY_WIN;
}

void ResultScene::Update(float frameTime)
{
	wa.Update(frameTime);
	timer = min(timer + (1.0f / BLANK_TIME * 60.0f * frameTime), 1.0f);
	
	if (timer == 1.0f){
		pointTimer = min(pointTimer + (1.0f / GAUGE_TIME * 60.0f * frameTime), 1.0f);
		for (int i = 0; i <= 3; i++){
			redPoints.at(i) = Math::lerp3(0, nextRedPoints.at(i), pointTimer);
		}
		for (int i = 0; i <= 3; i++){
			bluePoints.at(i) = Math::lerp3(0, nextBluePoints.at(i), pointTimer);
		}
	}
	else{
		if ((Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_Z, true) ||
			Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_SPACE, true) ||
			Device::GetInstance().GetInput()->GamePadButtonDown(0, GAMEPADKEY::BUTTON_CURCLE, true))){
			timer = 1.0f;
			pointTimer = 1.0f;
			vicTimer = 1.0f;
			return;
		}
	}
	if (pointTimer == 1.0f){
		if ((Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_Z, true) ||
			Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_SPACE, true) ||
			Device::GetInstance().GetInput()->GamePadAnyButton(0, true)) &&
			vicTimer == 1.0f){
			mIsEnd = true;
			Audio::GetInstance().PlaySE(SE_ID::ENTER_SE);
		}

		vicTimer = min(vicTimer + (1.0f / VICTRY_TIME * 60.0f * frameTime), 1.0f);
	}
	else{
		if ((Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_Z, true) ||
			Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_SPACE, true) ||
			Device::GetInstance().GetInput()->GamePadButtonDown(0, GAMEPADKEY::BUTTON_CURCLE, true))){
			pointTimer = 1.0f;
			vicTimer = 1.0f;
			return;
		}
	}
	if (vicTimer == 1.0f){
		pressTimer = min(pressTimer + ((1.0f / (60.0f * 3.0f)) * 60.0f * frameTime), 1.0f);
	}
	if (pressTimer == 1.0f){
		pressLerp+=2;
		pressLerp = fmodf(pressLerp, 360.0f);
	}

	//ゲージの中央である0.5から目標割合へ向かう。
	teamPoint = Math::lerp(0.5f, nextTeamPoint, pointTimer);
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
	int redCount=0, blueCount=0;
	for (int i = 0; i <= 7; i++){
		TEXTURE_ID tID;
		TEXTURE_ID whitetID;
		Vector2 pos;
		if (red){
			if (redTarantula){
				tID = TEXTURE_ID::TARENTULA_RED_TEXTURE;
				whitetID = TEXTURE_ID::TARENTULA_WHITE_R_TEXTURE;
			}
			else{
				tID = TEXTURE_ID::NEPHILA_RED_TEXTURE;
				whitetID = TEXTURE_ID::NEPHILA_WHITE_R_TEXTURE;
			}
			pos = vector2(1920 / 4 * (1 + 2 * !red) + (150*(1 * (-redCount % 2))), 1080 - 300 - (200 * redCount));
			redCount++;

			Graphic::GetInstance().DrawTexture(whitetID,
				vector2(pos.x, pos.y),
				vector2(1.0f, 1.0f),
				redNames.at(i) == "P1" ?
				red ?
				D3DXCOLOR(1, 0, 0, 1) : D3DXCOLOR(0, 0, 1, 1) :
				redNames.at(i) == "NONE" ?
				D3DXCOLOR(1, 1, 1, 1) : D3DXCOLOR(0, 1, 0, 1),
				vector2(0.5f, 0.5f),
				0.0f,
				0.0f,
				1,
				1.0f);

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
				redNames.at(i),
				redNames.at(i) == "P1" ?
				red ?
				vector3(1, 0, 0) : vector3(0, 0, 1) :
				redNames.at(i) == "NONE" ?
				vector3(1, 1, 1) : vector3(0, 1, 0),
				1,
				true);

			pos.y += -64;
			if (redNames.at(i) != "NONE")
				Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT,
				pos,
				vector2(0.40f, 0.4f),
				0.6f,
				std::to_string(redPoints.at(i)) + " pts",
				redNames.at(i) == "P1" ?
				red ?
				vector3(1, 0, 0) : vector3(0, 0, 1) :
				redNames.at(i) == "NONE" ?
				vector3(1, 1, 1) : vector3(0, 1, 0),
				1,
				true);

		}
		else{
			if (blueTarantula){
				tID = TEXTURE_ID::TARENTULA_BLUE_TEXTURE;
				whitetID = TEXTURE_ID::TARENTULA_WHITE_L_TEXTURE;
			}
			else{
				tID = TEXTURE_ID::NEPHILA_BLUE_TEXTURE;
				whitetID = TEXTURE_ID::NEPHILA_WHITE_L_TEXTURE;
			}
			pos = vector2(1920 / 4 * (1 + 2 * !red) - (150 * (1 * (-blueCount % 2))), 1080 - 300 - (200 * blueCount));
			blueCount++;

			Graphic::GetInstance().DrawTexture(whitetID,
				vector2(pos.x, pos.y),
				vector2(1.0f, 1.0f),
				blueNames.at(i-4) == "P1" ?
				red ?
				D3DXCOLOR(1, 0, 0, 1) : D3DXCOLOR(0, 0, 1, 1) :
				blueNames.at(i-4) == "NONE" ?
				D3DXCOLOR(1, 1, 1, 1) : D3DXCOLOR(0, 1, 0, 1),
				vector2(0.5f, 0.5f),
				0.0f,
				0.0f,
				1,
				1.0f);

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
				blueNames.at(i-4),
				blueNames.at(i-4) == "P1" ?
				red ?
				vector3(1, 0, 0) : vector3(0, 0, 1) :
				blueNames.at(i-4) == "NONE" ?
				vector3(1, 1, 1) : vector3(0, 1, 0),
				1,
				true);
			pos.y += -64;
			if (blueNames.at(i-4) != "NONE")
				Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT,
				pos,
				vector2(0.40f, 0.4f),
				0.6f,
				std::to_string(bluePoints.at(i-4)) + " pts",
				blueNames.at(i - 4) == "P1" ?
				red ?
				vector3(1, 0, 0) : vector3(0, 0, 1) :
				blueNames.at(i - 4) == "NONE" ?
				vector3(1, 1, 1) : vector3(0, 1, 0),
				1,
				true);

		}
		
		if (redCount >= 4)
			red = false;
	}
	if (pointTimer == 1.0f){
		float textSize = 0.8f;
		if (victory == VICTORY_ID::PLAYER_WIN){
			Graphic::GetInstance().DrawTexture(TEXTURE_ID::WHITE_TEXTURE, vector2(0, 0), vector2(1920, 1080), D3DXCOLOR(1, 0, 0, vicTimer * 0.5f), vector2(0.0f, 0.0f), 0, 0, 1.0f, 1.0f, 0);
			Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(1920 / 2.0f, 1080 / 2.0f), vector2(1.0f, 1.0f) * textSize, 0.5f, "RED TEAM WINS !!", vector3(1, 0, 0), vicTimer, true);
		}
		else if (victory == VICTORY_ID::ENEMY_WIN){
			Graphic::GetInstance().DrawTexture(TEXTURE_ID::WHITE_TEXTURE, vector2(0, 0), vector2(1920, 1080), D3DXCOLOR(0, 0, 1, vicTimer * 0.5f), vector2(0.0f, 0.0f), 0, 0, 1.0f, 1.0f, 0);
			Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(1920 / 2.0f, 1080 / 2.0f), vector2(1.0f, 1.0f) * textSize, 0.5f, "BLUE TEAM WINS !!", vector3(0, 0, 1), vicTimer, true);
		}
		else if (victory == VICTORY_ID::DRAW_WIN){
			Graphic::GetInstance().DrawTexture(TEXTURE_ID::WHITE_TEXTURE, vector2(0, 0), vector2(1920, 1080), D3DXCOLOR(1, 1, 1, vicTimer * 0.5f), vector2(0.0f, 0.0f), 0, 0, 1.0f, 1.0f, 0);
			Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(1920 / 2.0f, 1080 / 2.0f), vector2(1.0f, 1.0f) * textSize, 0.5f, "DRAW", vector3(1, 1, 1), vicTimer, true);
		}
	}
	if (pressTimer == 1.0f){
		float alpha = abs(Math::sin(pressLerp));
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(1920 / 2.0f, 1080 / 4.0f), vector2(0.5f, 0.5f), 0.5f, "press any button", vector3(1, 1, 1), alpha, true);
	}
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
	Audio::GetInstance().StopBGM(BGM_ID::RESULT_BGM);
}