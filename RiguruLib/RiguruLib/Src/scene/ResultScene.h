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
	//�R���X�g���N�^ 
	ResultScene(std::weak_ptr<SceneParameter> sp_);
	//�f�X�g���N�^
	~ResultScene();

	//�X�V�O������
	virtual void Initialize() override;
	//�X�V����
	virtual void Update(float frameTime)override;
	//�`�揈��
	virtual void Draw() const override;
	//�I�����Ă��邩�H
	virtual bool IsEnd() const override;
	//���̃V�[����Ԃ�
	virtual Scene Next() const override;
	//�I��������
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