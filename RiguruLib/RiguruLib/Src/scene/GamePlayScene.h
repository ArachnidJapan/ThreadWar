#pragma once

#include "IScene.h"
#include "ISceneMediator.h"
#include "../world/World.h"
#include "../Math/Matrix4.h"
#include "../Actor/ID.h"
#include "../Actor/Stage.h"
#include "SceneParameter.h"
#include "../scene/Option/Option.h"

class  GamePlayScene :public IScene{
public:
	//コンストラクタ 
	GamePlayScene(std::weak_ptr<SceneParameter> sp_, std::weak_ptr<Option> option_);
	//デストラクタ
	~GamePlayScene();

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
	std::weak_ptr<Option> option;
	std::shared_ptr<Stage> stage;
	bool mIsEnd, returnMenu;
	World wa;
	int bvolume;
	int svolume;
	Matrix4 playerMat, enemyMat;

	//１チームの人数
	const float teamMemberCount = 4;
	CAMERA_ID drawCamera = CAMERA_ID::PLAYER_CAMERA_1P;
	float blackAlpha;
	float drawNum = 0;
};