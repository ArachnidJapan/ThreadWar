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
	//AI���s
	void AIRun(float frameTime);

private:
	/*****=AI���s=******/
	//���s�p�^�C�}�[
	float aiTimer;
	//�퓬��Ԏ��A���E���ɖڕW�𑨂����Ɍo�߂�������
	float lostTimer;
	//�x�����x���؂�ւ��^�C�}�[
	float levelTimer;
};