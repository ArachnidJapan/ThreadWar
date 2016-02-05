#pragma once
#include "EnemyAI.h"

class FirstAI : public EnemyAI{
public:
	FirstAI(IWorld& wo, PlayerActionManager& action, std::weak_ptr<Player> player, CAMERA_ID& cID_, ActorParameter& parameter_, std::weak_ptr<Stage> stage_);
	virtual void OnInitialize() override;
	virtual void OnUpdate(float frameTime) override;
	virtual void OnDamage(int num) override;
	virtual void OnDead() override;

private:
	//AI実行
	void AIRun(float frameTime);

private:
	/*****=AI実行=******/
	//実行用タイマー
	float aiTimer;
	//戦闘状態時、視界内に目標を捉えずに経過した時間
	float lostTimer;
	//警戒レベル切り替えタイマー
	float levelTimer;
};