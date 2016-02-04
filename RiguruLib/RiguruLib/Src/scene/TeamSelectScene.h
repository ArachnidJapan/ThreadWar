#pragma once

#include "IScene.h"
#include "ISceneMediator.h"
#include "../world/World.h"
#include "../TeamSelect/SelectSpider.h"

class TeamSelectScene :public IScene{
public:
	//�R���X�g���N�^ 
	TeamSelectScene();
	//�f�X�g���N�^
	~TeamSelectScene();

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
	bool mIsEnd;
	World wa;
	std::vector<std::shared_ptr<SelectSpider>> selectSpider;
	int count;
};