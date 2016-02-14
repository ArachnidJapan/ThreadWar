#pragma once

#include "IScene.h"
#include "ISceneMediator.h"
#include "../world/World.h"
#include "../Actor/Actor.h"
#include "../Math/Vector2.h"
#include "SceneParameter.h"
class  DemoScene :public IScene{
public:
	//�R���X�g���N�^ 
	DemoScene(std::weak_ptr<SceneParameter> sp_);
	//�f�X�g���N�^
	~DemoScene();

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
	bool mIsEnd;
	bool effectEnd, pressStart;
	World wa;
	//���o�̃u�����N�^�C���Ɏg�p�B
	float timer;
	//���o�̃A���t�@�l�plerp�B
	float logoLerp, pressLerp,blackLerp;
	std::vector<int> configData;
	Vector3 moveVec;

	bool changeFlag,moveRes;
	int cameraWork;
	float changeTime;
};