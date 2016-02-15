#include "FirstAI.h"
#include "AITargetManager.h"

FirstAI::FirstAI(IWorld& wo, PlayerActionManager& action, std::weak_ptr<Player> player, CAMERA_ID& cID_, ActorParameter& parameter_, std::weak_ptr<Stage> stage_, int& playerNum_) :
EnemyAI(wo, action, player, cID_, parameter_, stage_, playerNum_)
{
	OnInitialize();
}

void FirstAI::OnInitialize(){
	aiTimer = Math::random(1.0f, 5.0f);
	lostTimer = 0.0f;
	levelTimer = 0.0f;
	levelChangeNum = 0;
}

void FirstAI::OnUpdate(float frameTime){
	//AIのレベル
	levelTimer += frameTime;
	if (levelTimer >= 5.0f){
		if (levelChangeNum == playerNum % 4)
			AILevelSetting();
		levelChangeNum++;
		levelTimer = 0.0f;
		if (levelChangeNum > 3)
			levelChangeNum = 0;
	}

	//入力状態リセット
	InputReset();

	//パラメータ等、AI実行前に必要な計算
	ParamCalc();

	//AIを実行
	AIRun(frameTime);

	//最終的な入力
	FinalInput();
}

void FirstAI::OnDamage(int num)
{
	//目標のマトリックスから計算
	int check = num % 4;
	Matrix4 mat;
	if (searchActor == ACTOR_ID::PLAYER_ACTOR) mat = AITargetManager::GetInstance().GetRedTeamMatList()[check];
	else mat = AITargetManager::GetInstance().GetBlueTeamMatList()[check];

	Vector3 pos = RCMatrix4::getPosition(mat);
	Vector3 vec = RCVector3::normalize(pos - myPos);
	float dist = targetDists[check];

	//targetが確定
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
	//視界内に目標がいるか調べる
	target.isLooking = Search();

	//目標を発見するまではクリスタルを攻撃or索敵
	if (!target.isBattle)
	{
		if (targetCrystal)
		{
			//ターゲットをクリスタルに
			target.lastLookingPos = nearestCrystalPos;
			target.lastLookingVec = RCVector3::normalize(nearestCrystalPos - myPos);
			target.distance = nearestCrystalDist;
			//CrystalPointMoveAttack(frameTime);
			ChangeAction(AI_ACTION::CrystalPointMoveAttack);
		}
		//すべてのクリスタルが制圧されていたら
		else if (!target.isLooking)
		{
			//とりあえず相手の陣地のクリスタルを目指す
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

		//敵を発見した場合
		if (target.isLooking)
		{
			//距離がクリスタルより近い場合
			if (targetDists[targetNum % 4] < nearestCrystalDist)
			{
				//戦闘状態へ移行
				target.isBattle = true;
				aiTimer = 0.0f;
			}
		}
	}

	//戦闘状態の場合
	if (target.isBattle)
	{
		//視界内に捉えているなら
		if (target.isLooking)
		{
			//最後に捉えた地点を更新
			target.lastLookingPos = target.pos - RCVector3::normalize(RCMatrix4::getUp(target.mat)) * 0.2f;
			//方向も取得
			target.lastLookingVec = RCVector3::normalize(target.lastLookingPos - myPos);
			lostTimer = 0;
		}
		//捉えていないなら
		else
		{
			//見失った時間に加算
			lostTimer += frameTime;
			//レベルに応じて貫通して見える
			if (lostTimer <= level[levelNum].penetrationTime){
				target.lastLookingPos = target.pos;
				target.lastLookingVec = RCVector3::normalize(target.lastLookingPos - myPos);
			}
		}

		if (lostTimer <= 4.0f)
		{
			//距離が近い場合
			if (target.distance <= level[levelNum].rewindLength)
				//糸発射しながら移動
				ChangeAction(AI_ACTION::TargetPointMoveAttack);
			//距離が遠い場合
			else
				//巻き取って接近
				ChangeAction(AI_ACTION::TargetJumpShotRewind);
		}
	}

	//見失ってから数秒経過したら
	if (lostTimer > 4.0f)
	{
		target.isBattle = false;
		//if (lostTimer  < 12.0f)
		//	//見失った地点に向かって移動
		//	ChangeAction(AI_ACTION::TargetPointMove);

		//else
		//{
		//	//移動し終えたら旋回して索敵
		//	aiTimer += frameTime;
		//	if (aiTimer < 6.0f)
		//		//索敵のために回転
		//		ChangeAction(AI_ACTION::TurnLook);
		//	//TurnLook(frameTime);
		//	else if (aiTimer >= 6.0f)
		//	{
		//		//回転しても見つからなかった場合は戦闘状態解除
		//		target.isBattle = false;
		//		lostTimer = 0;
		//		aiTimer = 0;
		//	}
		//}
	}

	actionFunc[currentAction](frameTime);
}