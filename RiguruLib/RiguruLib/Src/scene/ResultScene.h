#pragma once

#include "IScene.h"
#include "ISceneMediator.h"
#include "../world/World.h"
#include "../Actor/Actor.h"
#include "../Math/Vector2.h"
#include "SceneParameter.h"
#include <vector>

class  ResultScene :public IScene{
public:
	//コンストラクタ 
	ResultScene(std::weak_ptr<SceneParameter> sp_);
	//デストラクタ
	~ResultScene();

	//更新前初期化
	virtual void Initialize() override;
	//更新処理
	virtual void Update(float frameTime)override;
	//描画処理
	virtual void Draw() const override;
	//終了しているか？
	virtual bool IsEnd() const override;
	//次のシーンを返す
	virtual Scene Next() const override;
	//終了時処理
	virtual void End()override;

private:
	World wa;
	std::weak_ptr<SceneParameter> sp;
	//各プレイヤーが獲得したポイント。挿入されるデータの順番はREDチームの1~4のキャラ、Blueチームの1~4のキャラの順。
	std::vector<int> redPoints, nextRedPoints;
	std::vector<int> bluePoints, nextBluePoints;
	//各プレイヤーの名前(P1,CP1等)
	std::vector<std::string> redNames;
	std::vector<std::string> blueNames;
	//画面上部のポイントゲージで使用するREDチームとBlueチームのポイントの割合。
	float teamPoint, nextTeamPoint;
	//RedチームとBlueチームの総合得点。
	float playerTeamPoint, enemyTeamPoint;
	bool mIsEnd;
	bool redTarantula, blueTarantula;
	int spiderNum, redNum, blueNum;
	//各演出のタイマ。
	float timer, pointTimer, vicTimer, pressTimer, pressLerp;
	VICTORY_ID victory;
};