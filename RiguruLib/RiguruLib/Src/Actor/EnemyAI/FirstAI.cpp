#include "FirstAI.h"

FirstAI::FirstAI(IWorld& wo, PlayerActionManager& action, std::weak_ptr<Player> player, CAMERA_ID& cID_, ActorParameter& parameter_, std::weak_ptr<Stage> stage_) :
EnemyAI(wo, action, player, cID_, parameter_, stage_)
{
	OnInitialize();
}

void FirstAI::OnInitialize(){
	aiTimer = Math::random(1.0f, 5.0f);
	lostTimer = 0.0f;
	levelTimer = 0.0f;
}

void FirstAI::OnUpdate(float frameTime){
	//AI�̃��x��
	levelTimer += frameTime;
	if (levelTimer >= 20.0f){
		levelTimer = 0.0f;
		AILevelSetting();
	}

	//���͏�ԃ��Z�b�g
	InputReset();

	//�p�����[�^���AAI���s�O�ɕK�v�Ȍv�Z
	ParamCalc();

	//AI�����s
	AIRun(frameTime);

	//�ŏI�I�ȓ���
	FinalInput();
}

void FirstAI::OnDamage(int num)
{
	//�ڕW�̃}�g���b�N�X����v�Z
	Matrix4 mat = targetMats[num];
	Vector3 pos = RCMatrix4::getPosition(mat);
	Vector3 vec = RCVector3::normalize(pos - myPos);
	float dist = targetDists[num];

	//target���m��
	target.mat = mat;
	target.pos = pos;
	target.vec = vec;
	target.distance = dist;
	targetNum = num;
}

void FirstAI::OnDead()
{
	aiTimer = 0.0f;
	lostTimer = 0.0f;
}

void FirstAI::AIRun(float frameTime)
{
	//���E���ɖڕW�����邩���ׂ�
	target.isLooking = Search();

	//�ڕW�𔭌�����܂ł̓N���X�^�����U��or���G
	if (!target.isBattle)
	{
		if (targetCrystal)
		{
			//�^�[�Q�b�g���N���X�^����
			target.lastLookingPos = nearestCrystalPos;
			target.lastLookingVec = RCVector3::normalize(nearestCrystalPos - myPos);
			target.distance = nearestCrystalDist;
			//CrystalPointMoveAttack(frameTime);
			ChangeAction(AI_ACTION::CrystalPointMoveAttack);
		}
		//���ׂẴN���X�^������������Ă�����
		else
		{
			//�Ƃ肠��������̐w�n�̃N���X�^����ڎw��
			if (parameter.id == ACTOR_ID::PLAYER_ACTOR){
				target.lastLookingPos = iceCrystalPos;
				target.lastLookingVec = RCVector3::normalize(iceCrystalPos - myPos);
				target.distance = iceCrystalDist;
			}
			else{
				target.lastLookingPos = caveCrystalPos;
				target.lastLookingVec = RCVector3::normalize(caveCrystalPos - myPos);
				target.distance = caveCrystalDist;
			}
			ChangeAction(AI_ACTION::CrystalPointMoveAttack);
			//aiTimer += frameTime;
			//if (aiTimer < 8.0f)
			//	ChangeAction(AI_ACTION::RandomMove);
			//else if (aiTimer < 11.0f)
			//	ChangeAction(AI_ACTION::CreateThreadWeb);
			//else if (aiTimer < 14.0f)
			//	ChangeAction(AI_ACTION::JumpShotRewind);
			//else
			//{
			//	ChangeAction(AI_ACTION::RandomMove);
			//	aiTimer = 0.0f;
			//}
		}

		//�G�𔭌������ꍇ
		if (target.isLooking)
		{
			//�������N���X�^�����߂��ꍇ
			if (targetDists[targetNum] < nearestCrystalDist)
			{
				//�퓬��Ԃֈڍs
				target.isBattle = true;
				aiTimer = 0.0f;
			}
		}
	}

	//�퓬��Ԃ̏ꍇ
	if (target.isBattle)
	{
		//���E���ɑ����Ă���Ȃ�
		if (target.isLooking)
		{
			//�Ō�ɑ������n�_���X�V
			target.lastLookingPos = target.pos;
			//�������擾
			target.lastLookingVec = RCVector3::normalize(target.lastLookingPos - myPos);
			lostTimer = 0;
		}
		//�����Ă��Ȃ��Ȃ�
		else
		{
			//�����������Ԃɉ��Z
			lostTimer += frameTime;
			//���x���ɉ����Ċђʂ��Č�����
			if (lostTimer <= level[levelNum].penetrationTime){
				target.lastLookingPos = target.pos;
				target.lastLookingVec = RCVector3::normalize(target.lastLookingPos - myPos);
			}
		}

		if (lostTimer <= 4.0f)
		{
			//�������߂��A�ڒn���Ă���Ȃ�
			if (myParam->onGroundFlag && target.distance <= level[levelNum].rewindLength)
				//�����˂��Ȃ���ړ�
				ChangeAction(AI_ACTION::TargetPointMoveAttack);
			//�����������Ȃ�
			else
				//��������Đڋ�
				ChangeAction(AI_ACTION::TargetJumpShotRewind);
		}
	}

	//�������Ă��琔�b�o�߂�����
	if (lostTimer > 4.0f)
	{
		target.isBattle = false;
		//if (lostTimer  < 12.0f)
		//	//���������n�_�Ɍ������Ĉړ�
		//	ChangeAction(AI_ACTION::TargetPointMove);

		//else
		//{
		//	//�ړ����I��������񂵂č��G
		//	aiTimer += frameTime;
		//	if (aiTimer < 6.0f)
		//		//���G�̂��߂ɉ�]
		//		ChangeAction(AI_ACTION::TurnLook);
		//	//TurnLook(frameTime);
		//	else if (aiTimer >= 6.0f)
		//	{
		//		//��]���Ă�������Ȃ������ꍇ�͐퓬��ԉ���
		//		target.isBattle = false;
		//		lostTimer = 0;
		//		aiTimer = 0;
		//	}
		//}
	}

	actionFunc[currentAction](frameTime);
}