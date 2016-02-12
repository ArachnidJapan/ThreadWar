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
	std::weak_ptr<SceneParameter> sp;
	std::vector<int> points;
	std::vector<std::string> names;
	float teamPoint, prevTeamPoint, nextTeamPoint;
	float playerTeamPoint, enemyTeamPoint;
	bool mIsEnd;
	float timer, pointTimer, vicTimer;
	World wa;
	VICTORY_ID victory;
};