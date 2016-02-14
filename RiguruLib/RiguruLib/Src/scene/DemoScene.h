#pragma once

#include "IScene.h"
#include "ISceneMediator.h"
#include "../world/World.h"
#include "../Actor/Actor.h"
#include "../Math/Vector2.h"
#include "SceneParameter.h"
class  DemoScene :public IScene{
public:
	//コンストラクタ 
	DemoScene(std::weak_ptr<SceneParameter> sp_);
	//デストラクタ
	~DemoScene();

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
	bool mIsEnd;
	bool effectEnd, pressStart;
	World wa;
	//演出のブランクタイムに使用。
	float timer;
	//演出のアルファ値用lerp。
	float logoLerp, pressLerp,blackLerp;
	std::vector<int> configData;
	Vector3 moveVec;

	bool changeFlag,moveRes;
	int cameraWork;
	float changeTime;
};