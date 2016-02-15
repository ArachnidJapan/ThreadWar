#pragma once

#include "IScene.h"
#include "ISceneMediator.h"
#include "../world/World.h"
#include "../Actor/Actor.h"
#include "../Math/Vector2.h"
#include "../scene/Option/Option.h"
#include "SceneParameter.h"
#include <vector>
#define TITLE_SELECT_NUM 2

enum TITLE_SELECT{
	SELECT_GAMESTART,
	SELECT_OPTION,
	SELECT_RETURN,
};

class  TitleScene :public IScene{
public:
	//コンストラクタ 
	TitleScene(std::weak_ptr<SceneParameter> sp_, std::weak_ptr<Option> option_);
	//デストラクタ
	~TitleScene();

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

	//タイトルセレクト。
	void TitleSelect(float frameTime);
	//チームセレクト。
	void TeamSelect();

	/*タイトルセレクトの2つの選択肢移動。*/
	void Move();
	/*******************/

private:
	std::weak_ptr<SceneParameter> sp; 
	std::weak_ptr<Option> option;

private:
	bool mIsEnd;
	//2つの選択肢のうちの1つが選択されたか。
	bool isSelect;
	World wa;
	//演出の点滅やブランクタイムなどに使用。
	float timer;
	//演出のlerp用。
	float lerpTime, alphaTime, allAlpha;
	//2つの選択肢で選択されたものを格納。
	TITLE_SELECT selects;
	//2つのチームで選択されたものを格納。
	TEAM_ID teamID;
	Vector2 threadBackPos;

	//選択肢のパラメータ。
	std::vector<float> ts_scale, ts_nextScale, ts_prevScale;
	std::vector<float> ts_alpha, ts_nextAlpha;
	/*float ts_scale[TITLE_SELECT_NUM];
	float ts_nextScale[TITLE_SELECT_NUM], ts_prevScale[TITLE_SELECT_NUM];
	float ts_alpha[TITLE_SELECT_NUM];
	float ts_nextAlpha[TITLE_SELECT_NUM];*/
};