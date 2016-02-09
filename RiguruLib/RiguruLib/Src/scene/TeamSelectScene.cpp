#include "TeamSelectScene.h"
#include<memory>
#include "../actor/Player.h"
#include "../Other/Device.h"
#include "../Graphic/Graphic.h"
#include "../Math/MathUtility.h"
#include "../Math/Vector2Utility.h"
#include "../Actor/Stage.h"
#include "../Actor/CrystalCenter.h"
#include "../Math/Converter.h"
#include "../Audio/Audio.h"
#include "../TeamSelect/SelectPlayerParam.h"

//コンストラクタ
TeamSelectScene::TeamSelectScene(std::weak_ptr<SceneParameter> sp_) :sp(sp_)
{
	mIsEnd = false;
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::NEPHILA_WHITE_TEXTURE, "Res/Texture/nephila white.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::NEPHILA_RED_TEXTURE,"Res/Texture/nephila_red.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::NEPHILA_BLUE_TEXTURE, "Res/Texture/nephila_blue.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::TARENTULA_WHITE_TEXTURE, "Res/Texture/tarentula white.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::TARENTULA_RED_TEXTURE,"Res/Texture/tarentula_red.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::TARENTULA_BLUE_TEXTURE, "Res/Texture/tarentula_blue.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::TEAM_TEXTURE,"Res/Texture/team.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::WHITE_BAR_TEXTURE, "Res/Texture/white bar.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::MENU_ARROW_LEFT_TEXTURE, "Res/Texture/menu_Arrow_Left.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::MENU_ARROW_RIGHT_TEXTURE, "Res/Texture/menu_Arrow_Right.png");
}

//デストラクタ
TeamSelectScene::~TeamSelectScene()
{
}

//開始
void TeamSelectScene::Initialize()
{
	wa.Initialize();

	/*マップ関係*/
	std::shared_ptr<CrystalCenter> crystalCenter = std::make_shared<CrystalCenter>(wa, ACTOR_ID::CRYSTAL_CENTER_ACTOR,true);
	std::shared_ptr<CrystalCenter> crystalPlayerSide = std::make_shared<CrystalCenter>(wa, ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR);
	std::shared_ptr<CrystalCenter> crystalEnemySide = std::make_shared<CrystalCenter>(wa, ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR);
	wa.Add(ACTOR_ID::CRYSTAL_CENTER_ACTOR, crystalCenter);
	wa.Add(ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR, crystalPlayerSide);
	wa.Add(ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR, crystalEnemySide);
	std::shared_ptr<Stage> stage = std::make_shared<Stage>(wa, crystalCenter, crystalPlayerSide, crystalEnemySide, false);
	wa.Add(ACTOR_ID::STAGE_ACTOR, stage);

	Device::GetInstance().CameraInit(CAMERA_ID::GOD_CAMERA, stage);

	Device::GetInstance().GetCamera(CAMERA_ID::GOD_CAMERA)->GotCamera(vector3(-3.5f,3,2.0f),0);

	mIsEnd = false;

	std::shared_ptr<SelectSpider> playerSpiderSelect = std::make_shared<SelectSpider>(true, false, vector2(1920 / 4, 1080 - 100));
	std::shared_ptr<SelectSpider> enemySpiderSelect = std::make_shared<SelectSpider>(false, false, vector2(1920 / 4 * 3, 1080 - 100));
	selectSpider.push_back(playerSpiderSelect);
	selectSpider.push_back(enemySpiderSelect);

	selectSpider.push_back(std::make_shared<SelectSpider>(true, true, vector2(1920 / 4 - 100, 1080 - 300),0, playerSpiderSelect));
	selectSpider.push_back(std::make_shared<SelectSpider>(false, true, vector2(1920 / 4 * 3 - 100, 1080 - 300),0, enemySpiderSelect));
	selectSpider.push_back(std::make_shared<SelectSpider>(true, true, vector2(1920 / 4 + 100, 1080 - 500),1, playerSpiderSelect));
	selectSpider.push_back(std::make_shared<SelectSpider>(false, true, vector2(1920 / 4 * 3 + 100, 1080 - 500),1, enemySpiderSelect));
	selectSpider.push_back(std::make_shared<SelectSpider>(true, true, vector2(1920 / 4 - 100, 1080 - 700),2, playerSpiderSelect));
	selectSpider.push_back(std::make_shared<SelectSpider>(false, true, vector2(1920 / 4 * 3 - 100, 1080 - 700),2, enemySpiderSelect));
	selectSpider.push_back(std::make_shared<SelectSpider>(true, true, vector2(1920 / 4 + 100, 1080 - 900),3, playerSpiderSelect));
	selectSpider.push_back(std::make_shared<SelectSpider>(false, true, vector2(1920 / 4 * 3 + 100, 1080 - 900),3, enemySpiderSelect));

	count = 0;
}

void TeamSelectScene::Update(float frameTime)
{
	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_SPACE, true)){
		mIsEnd = true;
		return;
	}
	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_RIGHT, true)){
		if (count % 2 == 0){
			count++;
		}
		else{
			count--;
		}
	}
	else if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_LEFT, true)){
		if (count % 2 == 0){
			count++;
		}
		else{
			count--;
		}
	}
	else if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_UP, true))count-=2;
	else if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_DOWN, true))count+=2;
	int size = selectSpider.size() - 1;
	if (count > size){
		count = count - (size + 1);
	}
	else if (count < 0){
		count = size + count + 1;
	}
	for (int i = 0; i < selectSpider.size();i++){
		selectSpider[i]->SetSelect(false);
	}
	selectSpider[count]->SetSelect(true);

	selectSpider[0]->SetEnemyPlayerHave(selectSpider[1]->PlayerHave());
	selectSpider[1]->SetEnemyPlayerHave(selectSpider[0]->PlayerHave());

	selectSpider[count]->Update();
	wa.Update(frameTime);
}

//描画
void TeamSelectScene::Draw() const
{
	wa.Draw(CAMERA_ID::GOD_CAMERA);
	Graphic::GetInstance().DrawTexture(TEXTURE_ID::TEAM_TEXTURE,
		vector2(1920 / 2, 1080 / 2),
		vector2(1,1),
		D3DXCOLOR(1, 1, 1, 1.0f),
		vector2(0.5f, 0.5f),
		0.0f,
		0.0f,
		1,
		1.0f);

	Graphic::GetInstance().DrawTexture(TEXTURE_ID::DAMAGE_TEXTURE,
		vector2(1920 / 2, 1080 / 2),
		vector2(1.5f, 1.5f),
		D3DXCOLOR(1,1,1, 0.5f),
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

	for (auto i : selectSpider){
		i->Draw();
	}
}

//終了しているか？
bool TeamSelectScene::IsEnd() const
{
	return mIsEnd;
}

//次のシーンを返す
Scene TeamSelectScene::Next() const
{
	return Scene::GamePlay;
}

void TeamSelectScene::End(){
	Audio::GetInstance().StopBGM(BGM_ID::TITLE_BGM);
	TeamSelectResult tsr;
	tsr.redTarantula = selectSpider[0]->ReturnTarantula();
	tsr.blueTarantula = selectSpider[1]->ReturnTarantula();
	tsr.redHavePlayer = selectSpider[0]->PlayerHave();
	tsr.blueHavePlayer = selectSpider[1]->PlayerHave();
	tsr.redHaveCPU = selectSpider[0]->CPUCount();
	tsr.blueHaveCPU = selectSpider[1]->CPUCount();
	sp._Get()->SetTeamSelectResult(tsr);
	selectSpider.clear();
}