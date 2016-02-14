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
	//�R���X�g���N�^ 
	GamePlayScene(std::weak_ptr<SceneParameter> sp_, std::weak_ptr<Option> option_);
	//�f�X�g���N�^
	~GamePlayScene();

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
	std::weak_ptr<Option> option;
	std::shared_ptr<Stage> stage;
	bool mIsEnd, returnMenu;
	World wa;
	int bvolume;
	int svolume;
	Matrix4 playerMat, enemyMat;

	//�P�`�[���̐l��
	const float teamMemberCount = 4;
	CAMERA_ID drawCamera = CAMERA_ID::PLAYER_CAMERA_1P;
	float blackAlpha;
	float drawNum = 0;
};