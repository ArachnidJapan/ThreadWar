/**====================================================================*/
/**
* AI�̖ڕW���ꊇ�Ǘ�
* */
/**====================================================================*/
#pragma once
#include <array>
#include "../Actor.h"

class AITargetManager
{
private:
	AITargetManager();
	~AITargetManager();

public:
	static AITargetManager &GetInstance(){
		static AITargetManager m;
		return m;
	}
	void Initialize(IWorld& world);
	void Update(IWorld& world);

	//�v���C���[�̃}�g���b�N�X���X�g���擾
	std::array<Matrix4, 4> GetRedTeamMatList();
	std::array<Matrix4, 4> GetBlueTeamMatList();
	//�v���C���[�̃|�W�V�������X�g���擾
	std::array<Vector3, 4> GetRedTeamPosList();
	std::array<Vector3, 4> GetBlueTeamPosList();

	//�L�����ꂽ�Ƃ��̃J�����p
	void SetAllPosList(int playerNum,Vector3 pos);
	std::array<Vector3, 8> GetAllPosList();

	//�擾�������`�[����ID������Ƃ������Ԃ� PLAYER_ACTOR���� ENEMY_ACTOR����
	std::array<Matrix4, 4> GetTeamMatListID(ACTOR_ID id);
	std::array<Vector3, 4> GetTeamPosListID(ACTOR_ID id);
	int GetHaveNum(ACTOR_ID id);

	//�v���C���[�̐l�����擾
	int GetRedHaveNum();
	int GetBlueHaveNum();

private:
	//�X���`�[��4�̂̃}�g���b�N�X
	std::array<Matrix4, 4> redMatList;
	//���A���`�[��4�̂̃}�g���b�N�X
	std::array<Matrix4, 4> blueMatList;
	//�X���`�[��4�̂̃|�W�V����
	std::array<Vector3, 4> redPosList;
	//���A���`�[��4�̂̃|�W�V����
	std::array<Vector3, 4> bluePosList;
	//�N���X�^��3�ӏ�
	std::array<Vector3, 3> crystalPosList;

	//�S���̃|�W�V����
	std::array<Vector3, 8> allPosList;

	//�ԃ`�[���l��
	int redHaveNum;
	//�`�[���l��
	int blueHaveNum;
};