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
	World wa;
	std::weak_ptr<SceneParameter> sp;
	//�e�v���C���[���l�������|�C���g�B�}�������f�[�^�̏��Ԃ�RED�`�[����1~4�̃L�����ABlue�`�[����1~4�̃L�����̏��B
	std::vector<int> redPoints, nextRedPoints;
	std::vector<int> bluePoints, nextBluePoints;
	//�e�v���C���[�̖��O(P1,CP1��)
	std::vector<std::string> redNames;
	std::vector<std::string> blueNames;
	//��ʏ㕔�̃|�C���g�Q�[�W�Ŏg�p����RED�`�[����Blue�`�[���̃|�C���g�̊����B
	float teamPoint, nextTeamPoint;
	//Red�`�[����Blue�`�[���̑������_�B
	float playerTeamPoint, enemyTeamPoint;
	bool mIsEnd;
	bool redTarantula, blueTarantula;
	int spiderNum, redNum, blueNum;
	//�e���o�̃^�C�}�B
	float timer, pointTimer, vicTimer, pressTimer, pressLerp;
	VICTORY_ID victory;
};