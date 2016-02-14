#pragma once

#include "IScene.h"
#include "ISceneMediator.h"
#include "../world/World.h"
#include "../TeamSelect/SelectSpider.h"
#include "SceneParameter.h"

class TeamSelectScene :public IScene{
public:
	//コンストラクタ 
	TeamSelectScene(std::weak_ptr<SceneParameter> sp_);
	//デストラクタ
	~TeamSelectScene();

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
	World wa;
	std::vector<std::shared_ptr<SelectSpider>> selectSpider;
	int count;
	bool isBackScene;
};