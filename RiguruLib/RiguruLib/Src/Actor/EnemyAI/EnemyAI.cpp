#include "EnemyAI.h"
#include "../../Audio/Audio.h"
#include "../Thread.h"
#include "../CrystalCenter.h"
#include "../Collision.h"

EnemyAI::EnemyAI(IWorld& wo, PlayerActionManager& action_, std::weak_ptr<Player> player_, CAMERA_ID& cID_, ActorParameter& parameter_, std::weak_ptr<Stage> stage_)
:world(wo), action(&action_), player(player_), cID(cID_), parameter(parameter_), stage(stage_)
{
	Initialize();
}

EnemyAI::~EnemyAI()
{

}

void EnemyAI::Initialize()
{
	nextAI = AI_STATE::FIRST_AI;
	teamID = player._Get()->GetTeam();

	for (int i = 0; i < 4; i++){
		targetMats[i] = RCMatrix4::Identity();
	}
	target.mat = RCMatrix4::Identity();
	target.pos = vector3(0, 0, 0);
	target.vec = vector3(0, 0, 0);
	target.isLooking = false;
	target.lastLookingPos = vector3(0, 0, 0);
	target.lastLookingVec = vector3(0, 0, 0);
	target.distance = 0.0f;
	target.isBattle = false;

	flag = false;
	timer = 0.0f;
	counter = 0;

	moveTime = stepTime = threadTime = 0;
	moveTimeMax = Math::random(1.0f, 5.0f);
	stepTimeMax = Math::random(1.0f, 3.0f);
	threadTimeMax = Math::random(1.0f, 2.0f);
	inputVecX = 1.0f;
	inputCameraX = 1.0f;

	myMat = player._Get()->GetParameter().matrix;
	myPos = RCMatrix4::getPosition(player._Get()->GetParameter().matrix);
	myPos2D = vector2(myPos.x, myPos.z);
	myParam = player._Get()->ReturnPlayerParameter();

	camera = Device::GetInstance().GetCamera(cID);
	cameraRotMat = RCMatrix4::Identity();
	cameraFront = vector3(0, 0, 0);
	cameraLeft = vector3(0, 0, 0);
	cameraUp = vector3(0, 0, 0);
	inputAngle = vector2(0, 0);
	inputVec = vector3(0, 0, 0);

	centerCrystalDist = iceCrystalDist = caveCrystalDist = nearestCrystalDist = 0.0f;
	nearestCrystalPos = vector3(0, 0, 0);
	targetCrystal = false;

	//サーチ対象
	if (parameter.id == ACTOR_ID::ENEMY_ACTOR) searchActor = ACTOR_ID::PLAYER_ACTOR;
	else searchActor = ACTOR_ID::ENEMY_ACTOR;

	actionFunc.clear();
	actionFunc[AI_ACTION::Curl] = std::bind(&EnemyAI::Curl, this, std::placeholders::_1);
	actionFunc[AI_ACTION::RandomMove] = std::bind(&EnemyAI::RandomMove, this, std::placeholders::_1);
	actionFunc[AI_ACTION::RandomLook] = std::bind(&EnemyAI::RandomLook, this, std::placeholders::_1);
	actionFunc[AI_ACTION::TurnLook] = std::bind(&EnemyAI::TurnLook, this, std::placeholders::_1);
	actionFunc[AI_ACTION::JumpShotRewind] = std::bind(&EnemyAI::JumpShotRewind, this, std::placeholders::_1);
	actionFunc[AI_ACTION::FrontLook] = std::bind(&EnemyAI::FrontLook, this, std::placeholders::_1);
	actionFunc[AI_ACTION::TargetPointLookH] = std::bind(&EnemyAI::TargetPointLookH, this, std::placeholders::_1);
	actionFunc[AI_ACTION::TargetPointLookV] = std::bind(&EnemyAI::TargetPointLookV, this, std::placeholders::_1);
	actionFunc[AI_ACTION::TargetPointLook] = std::bind(&EnemyAI::TargetPointLook, this, std::placeholders::_1);
	actionFunc[AI_ACTION::TargetPointLookAttack] = std::bind(&EnemyAI::TargetPointLookAttack, this, std::placeholders::_1);
	actionFunc[AI_ACTION::TargetPointMove] = std::bind(&EnemyAI::TargetPointMove, this, std::placeholders::_1);
	actionFunc[AI_ACTION::TargetPointMoveAttack] = std::bind(&EnemyAI::TargetPointMoveAttack, this, std::placeholders::_1);
	actionFunc[AI_ACTION::CrystalPointMoveAttack] = std::bind(&EnemyAI::CrystalPointMoveAttack, this, std::placeholders::_1);
	actionFunc[AI_ACTION::TargetJumpShotRewind] = std::bind(&EnemyAI::TargetJumpShotRewind, this, std::placeholders::_1);
	actionFunc[AI_ACTION::GettingThread] = std::bind(&EnemyAI::GettingThread, this, std::placeholders::_1);
	actionFunc[AI_ACTION::CreateThreadWeb] = std::bind(&EnemyAI::CreateThreadWeb, this, std::placeholders::_1);

	for (int i = 0; i < 3; i++)
	{
		level[i].viewAngle = 35.0f * (i + 1);
		level[i].viewLength = 25.0f * (i + 1);
		level[i].attackAngle = level[i].dangerLength = 5.0f * (i + 1);
		level[i].attackLength = 20.0f * (i + 1);
		level[i].rewindLength = 20.0f;
		level[i].bestLength = 15.0f;
		level[i].penetrationTime = 1.0f * (i + 1);
		level[i].inputXChangeTimeMax = 10.0f / (i + 1);
		level[i].inputXChangeTimeMin = 2.0f / (i + 1);
		level[i].stepTimeMax = 6.0f / (i + 1);
		level[i].stepTimeMin = 2.0f / (i + 1);
		level[i].battleStepTimeMax = 4.0f / (i + 1);
		level[i].battleStepTimeMin = 2.0f / (i + 1);
		level[i].rapidfire = 1.05f - (0.35f * i);
	}
	levelNum = 1;

	OnInitialize();
}
void EnemyAI::Update(float frameTime)
{
	//敵の位置を検索
	int num = 0;
	world.EachActor(searchActor, [&](const Actor& other){
		targetMats[num] = static_cast<Player*>(const_cast<Actor*>(&other))->GetParameter().matrix;
		targetDists[num] = RCVector3::distance_(myPos, RCMatrix4::getPosition(targetMats[num]));
		num++;
	});

	OnUpdate(frameTime);
}
void EnemyAI::Draw(CAMERA_ID cID)
{
	OnDraw(cID);
}
void EnemyAI::Damage()
{

}
void EnemyAI::Dead()
{
	target.mat = RCMatrix4::Identity();
	target.pos = vector3(0, 0, 0);
	target.vec = vector3(0, 0, 0);
	target.isLooking = false;
	target.lastLookingPos = vector3(0, 0, 0);
	target.lastLookingVec = vector3(0, 0, 0);
	target.distance = 0.0f;
	target.isBattle = false;

	flag = false;
	timer = 0.0f;
	counter = 0;

	OnDead();
}


void EnemyAI::ParamCalc()
{
	//自身のパラメータを取得
	myMat = player._Get()->GetParameter().matrix;
	myPos = RCMatrix4::getPosition(player._Get()->GetParameter().matrix);
	myPos2D = vector2(myPos.x, myPos.z);
	myParam = player._Get()->ReturnPlayerParameter();

	//カメラの取得
	//camera = Device::GetInstance().GetCamera(cID);
	cameraRotMat = camera->GetCameraRotMatrix();
	cameraFront = -RCVector3::normalize(RCMatrix4::getFront(cameraRotMat));
	cameraLeft = RCVector3::normalize(RCMatrix4::getLeft(cameraRotMat));
	cameraUp = RCVector3::normalize(RCMatrix4::getUp(cameraRotMat));

	//各クリスタルまでの距離
	centerCrystalDist = RCVector3::distance_(myPos, centerCrystalPos);
	iceCrystalDist = RCVector3::distance_(myPos, iceCrystalPos);
	caveCrystalDist = RCVector3::distance_(myPos, caveCrystalPos);
	//一番近い攻撃対象クリスタルを特定
	targetCrystal = true;
	if (parameter.id == ACTOR_ID::PLAYER_ACTOR){
		if (!stage._Get()->ReturnCrystal(ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR)._Get()->IsPlayer() &&
			caveCrystalDist < centerCrystalDist && caveCrystalDist < iceCrystalDist){
			nearestCrystalPos = caveCrystalPos;
			nearestCrystalDist = caveCrystalDist;
		}
		else if (!stage._Get()->ReturnCrystal(ACTOR_ID::CRYSTAL_CENTER_ACTOR)._Get()->IsPlayer() &&
			centerCrystalDist < iceCrystalDist){
			nearestCrystalPos = centerCrystalPos;
			nearestCrystalDist = centerCrystalDist;
		}
		else if (!stage._Get()->ReturnCrystal(ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR)._Get()->IsPlayer()){
			nearestCrystalPos = iceCrystalPos;
			nearestCrystalDist = iceCrystalDist;
		}
		else
			targetCrystal = false;
	}
	else{
		if (!stage._Get()->ReturnCrystal(ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR)._Get()->IsEnemy() &&
			iceCrystalDist < centerCrystalDist && iceCrystalDist < caveCrystalDist){
			nearestCrystalPos = iceCrystalPos;
			nearestCrystalDist = iceCrystalDist;
		}
		else if (!stage._Get()->ReturnCrystal(ACTOR_ID::CRYSTAL_CENTER_ACTOR)._Get()->IsEnemy() &&
			centerCrystalDist < caveCrystalDist){
			nearestCrystalPos = centerCrystalPos;
			nearestCrystalDist = centerCrystalDist;
		}
		else if (!stage._Get()->ReturnCrystal(ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR)._Get()->IsEnemy()){
			nearestCrystalPos = caveCrystalPos;
			nearestCrystalDist = caveCrystalDist;
		}
		else
			targetCrystal = false;
	}

	//入力初期化
	inputAngle = vector2(0, 0);
	inputVec = vector3(0, 0, 0);
}

void EnemyAI::InputReset()
{
	player._Get()->SetInputCurlOn(false);
	player._Get()->SetInputCurlOff(false);
	player._Get()->SetInputJump(false);
	player._Get()->SetInputStep(false);
	player._Get()->SetInputShot(false);
}
void EnemyAI::AILevelSetting()
{
	float per = stage._Get()->GetTeamPercentage(player._Get()->GetParameter().id);
	if (per <= 25.0f)
		levelNum = 2;
	else if (per <= 75.0f)
		levelNum = 1;
	else
		levelNum = 0;
}
void EnemyAI::ChangeAction(AI_ACTION s)
{
	if (currentAction != s)
	{
		currentAction = s;
		flag = false;
		timer = 0.0f;
		counter = 0;
	}
}
bool EnemyAI::IsCeiling()
{
	return RCV3Calc::InnerAngle(RCVector3::normalize(RCMatrix4::getUp(cameraRotMat)), vector3(0, 1, 0)) > 90;
}
void EnemyAI::Curl(float frameTime)
{
	if (!flag)
	{
		InputStep();
		InputCurlOn();
		flag = true;
	}

	timer += frameTime;
	if (timer >= 3.0f)
	{
		InputCurlOff();
	}
}
void EnemyAI::RandomMove(float frameTime)
{
	//タイマ増加
	moveTime += frameTime;
	stepTime += frameTime;

	//前進入力
	inputVec.z = -1;
	//左右入力
	inputVec.x = inputVecX;
	inputVec = RCVector3::normalize(inputVec);
	//左右入力切替
	if (moveTime > moveTimeMax){
		inputVecX = -inputVecX;
		moveTime = 0;
		moveTimeMax = Math::random(1.0f, 5.0f);
	}

	//ステップ入力
	if (stepTime > stepTimeMax){
		InputStep();
		stepTime = 0;
		stepTimeMax = Math::random(1.0f, 3.0f);
	}
}
void EnemyAI::RandomLook(float frameTime)
{
	if (!flag)
	{
		moveTimeMax = Math::random(1.0f, 3.0f);	//入力の長さ
		inputCameraX = Math::random(3.0f, 9.0f);	//入力の強さ
		if (Math::random(-1.0f, 1.0f) < 0) inputVecX *= -1;	//1/2の確率で入力方向反転
		flag = true;
	}

	//タイマ増加
	timer += frameTime;
	//カメラ入力
	inputAngle.x = inputCameraX;

	if (timer > moveTimeMax)
	{
		//もう一回
		flag = false;
		timer = 0.0f;
	}
}
void EnemyAI::TurnLook(float frameTime)
{
	if (!flag)
	{
		inputCameraX = 3.0f;	//入力の強さ
		flag = true;
	}

	//カメラ入力
	inputAngle.x = inputCameraX;
}
void EnemyAI::JumpShotRewind(float frameTime)
{
	timer += frameTime;
	inputVec.z = -1.0f;
	FrontLook(frameTime);
	if (timer < 1.0f) return;

	//一秒超えたらジャンプ→発射→巻取り
	switch (counter)
	{
	case 0:
		InputJump();
		counter++;
		break;

	case 1:
		if (timer > 1.3f)
		{
			InputShotThread();
			counter++;
		}
		break;

	case 2:
		if (timer > 1.4f)
		{
			InputRewind();
			counter++;
		}
		break;
	case 3:
		//巻き取りの成功/失敗にかかわらず、接地していたら
		if (myParam->onGroundFlag)
		{
			//初期化
			counter = 0;
			timer = 0;
		}
	default:
		break;
	}

}
void EnemyAI::SlantingShotRewind(float frameTime)
{
	if (counter >= 1)
		timer += frameTime;

	float angle = Math::acos(RCVector3::dot(cameraFront, RCVector3::normalize(RCMatrix4::getUp(myMat))));
	//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(800, 120), vector2(0.20f, 0.25f), 0.5f, "angle:" + std::to_string(angle));

	//斜め上を向いたら巻き取って移動
	switch (counter)
	{
	case 0:
		//自身の上方向にカメラを向ける
		if (angle > 30)
		{
			inputAngle.y += 6.0f;
			break;
		}
		counter++;
		break;

	case 1:
		//最初にジャンプ
		InputJump();
		counter++;
		break;

	case 2:
		if (timer > 0.3f)
		{
			//発射
			InputShotThread();
			counter++;
		}
		break;

	case 3:
		if (timer > 0.5f)
		{
			//巻き取り
			InputRewind();
			counter++;
		}
		break;

	default:
		break;
	}
}
void EnemyAI::LookV(float frameTime, Vector3 v)
{
	//向きたい方向
	Vector3 vec = v;

	if (vec.z > 0)
		vec = -vec;//+の値に制限して計算

	//上下判定
	float asin = Math::asin(RCVector2::cross(
		RCVector2::normalize(vector2(cameraFront.y, cameraFront.z)),
		RCVector2::normalize(vector2(vec.y, vec.z))));

	//なす角
	float acos = Math::acos(RCVector3::dot(
		RCVector3::normalize(vector3(0, cameraFront.y, cameraFront.z)),
		RCVector3::normalize(vector3(0, vec.y, target.vec.z))));

	//なす角を基に入力量を計算
	float add = fabsf(acos) / 2;
	add = Math::clamp(add, 1.0f, 10.0f);

	//入力により向きを変える
	if (asin < 0) inputAngle.y = -add;
	else inputAngle.y = add;

	//天井にいる場合は入力反転
	if (IsCeiling()) inputAngle.y *= -1;
}
void EnemyAI::FrontLook(float frameTime)
{
	LookV(frameTime, RCMatrix4::getFront(myMat));
}
void EnemyAI::TargetPointLookH(float frameTime)
{
	//左右判定
	float asin = Math::asin(RCVector2::cross(
		RCVector2::normalize(vector2(cameraFront.x, cameraFront.z)),
		RCVector2::normalize(vector2(target.lastLookingVec.x, target.lastLookingVec.z))));

	//なす角
	float acos = RCV3Calc::InnerAngle(
		vector3(cameraFront.x, 0, cameraFront.z),
		vector3(target.lastLookingVec.x, 0, target.lastLookingVec.z));

	//なす角を基に入力量を計算
	float add = fabsf(acos) / 2;
	add = Math::clamp(add, 1.0f, 10.0f);

	//入力により向きを変える
	if (asin > 0) inputAngle.x = -add;
	else inputAngle.x = add;

	//天井にいる場合は入力反転
	if (IsCeiling()) inputAngle.x *= -1;
}
void EnemyAI::TargetPointLookV(float frameTime)
{
	//向きたい方向
	Vector3 vec = target.lastLookingVec;

	if (vec.z > 0)
		vec = -vec;//+の値に制限して計算

	//上下判定
	float asin = Math::asin(RCVector2::cross(
		RCVector2::normalize(vector2(cameraFront.y, cameraFront.z)),
		RCVector2::normalize(vector2(vec.y, vec.z))));

	//なす角
	float acos = RCV3Calc::InnerAngle(
		vector3(0, cameraFront.y, cameraFront.z),
		vector3(0, vec.y, target.vec.z));

	//なす角を基に入力量を計算
	float add = fabsf(acos) / 2;
	add = Math::clamp(add, 1.0f, 10.0f);

	//入力により向きを変える
	if (asin < 0) inputAngle.y = -add;
	else inputAngle.y = add;

	//天井にいる場合は入力反転
	if (IsCeiling()) inputAngle.y *= -1;
}
void EnemyAI::TargetPointLook(float frameTime)
{
	TargetPointLookH(frameTime);
	TargetPointLookV(frameTime);
}
void EnemyAI::TargetPointLookAttack(float frameTime)
{
	TargetPointLookH(frameTime);
	TargetPointLookV(frameTime);

	timer += frameTime;
	if (timer >= 0.5f)
	{
		InputShotThread();
		timer = 0;
	}
}
void EnemyAI::TargetPointMove(float frameTime)
{
	TargetPointLookH(frameTime);
	TargetPointLookV(frameTime);
	//前進
	inputVec.z = -1;

	//タイマ増加
	stepTime += frameTime;
	//ステップ入力
	if (stepTime > stepTimeMax){
		InputStep();
		stepTime = 0;
		stepTimeMax = Math::random(level[levelNum].stepTimeMin, level[levelNum].stepTimeMax);
	}
}
void EnemyAI::TargetPointMoveAttack(float frameTime)
{
	TargetPointLookH(frameTime);
	TargetPointLookV(frameTime);
	//タイマ増加
	moveTime += frameTime;
	stepTime += frameTime;

	//距離によって前後の移動方向を変える
	if (target.distance > level[levelNum].bestLength - 5)
		//前進
		inputVec.z = -1;
	else
		//後退
		inputVec.z = 1;

	//左右入力切替
	if (moveTime > moveTimeMax){
		inputVecX = -inputVecX;
		moveTime = 0;
		moveTimeMax = Math::random(1.0f, 5.0f);
	}
	//左右入力
	inputVec.x = inputVecX;
	//正規化
	inputVec = RCVector3::normalize(inputVec);


	//ステップ入力
	if (stepTime > stepTimeMax){
		InputStep();
		stepTime = 0;
		stepTimeMax = Math::random(level[levelNum].battleStepTimeMin, level[levelNum].battleStepTimeMax);
	}

	//糸の発射
	timer += frameTime;

	if (timer > level[levelNum].rapidfire)
	{
		InputShotThread();
		timer = 0;
	}
}
void EnemyAI::CrystalPointMoveAttack(float frameTime)
{
	//自身とクリスタルとの間にステージポリゴンがあるか調べる
	Vector3 start = myPos + RCMatrix4::getUp(myMat) * 1.0f;
	Vector3 end = nearestCrystalPos;
	end = end - RCVector3::normalize(end - start) * 2.0f;
	CollisionParameter stageCol = ModelRay(*stage._Get()->ReturnMat(), OCT_ID::STAGE_OCT, start, end);
	bool isStageCol = stageCol.colFlag;
	//Graphic::GetInstance().DrawLine(start, end, CAMERA_ID::PLAYER_CAMERA_1P, vector3(0, 1, 0));

	//ポリゴンがある場合
	if (isStageCol)
	{
		//斜めに前進
		inputVec.z = -1;
		inputVec.x = -0.2f;
		inputVec = RCVector3::normalize(inputVec);
	}
	//ポリゴンがない場合
	else
	{
		TargetPointLookH(frameTime);
		TargetPointLookV(frameTime);
		//タイマ増加
		moveTime += frameTime;
		stepTime += frameTime;

		//距離によって前後の移動方向を変える
		if (target.distance > 20.0f)
			//前進
			inputVec.z = -1;
		else
			//後退
			inputVec.z = 1;

		//左右入力切替
		if (moveTime > moveTimeMax){
			inputVecX = -inputVecX;
			moveTime = 0;
			moveTimeMax = Math::random(1.0f, 5.0f);
		}
		//左右入力
		inputVec.x = inputVecX;
		//正規化
		inputVec = RCVector3::normalize(inputVec);

		//糸の発射
		timer += frameTime;
		if (timer > 0.2f)
		{
			InputShotThread();
			timer = 0;
		}
	}


	//ポリゴンがあってもなくても

	//ステップ入力
	stepTime += frameTime;
	if (stepTime > stepTimeMax){
		InputStep();
		stepTime = 0;
		stepTimeMax = Math::random(1.0f, 3.0f);
	}
}
void EnemyAI::TargetJumpShotRewind(float frameTime)
{
	timer += frameTime;
	//目標を向きながら前進
	TargetPointLook(frameTime);
	inputVec.z = -1.0f;

	if (timer < 1.0f) return;

	//一秒超えたらジャンプ→発射→巻取り
	switch (counter)
	{
	case 0:
		InputJump();
		counter++;
		break;

	case 1:
		if (timer > 1.3f)
		{
			InputShotThread();
			counter++;
		}
		break;

	case 2:
		if (timer > 1.4f)
		{
			InputRewind();
			counter++;
		}
		break;
	case 3:
		//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(800, 150), vector2(0.20f, 0.25f), 0.5f, "jump:" + std::to_string(target.distance < level[levelNum].bestLength));
		if (target.distance < level[levelNum].bestLength)
		{
			InputJump();
			counter++;
		}
		if (timer > 5.0f)
		{
			timer = 0;
			counter = 0;
		}
		break;
	default:
		counter = 0;
		timer = 0;
		break;
	}
}
void EnemyAI::GettingThread(float frameTime)
{
	if (counter >= 1)
		timer += frameTime;

	float angle = RCV3Calc::InnerAngle(cameraFront, RCMatrix4::getUp(myMat));

	//斜め上を向いたら糸発射→糸に乗る
	switch (counter)
	{
	case 0:
		//自身の上方向にカメラを向ける
		if (angle > 30)
		{
			inputAngle.y += 6.0f;
			break;
		}
		counter++;
		break;

	case 1:
		if (timer > 0.6f)
		{
			//発射
			InputShotThread();
			counter++;
		}
		break;

	case 2:
		if (timer > 1.2f)
		{
			////巻き取り
			//InputRewind();
			counter++;
		}
		break;

	default:
		break;
	}
}
void EnemyAI::CreateThreadWeb(float frameTime)
{
	//カメラ入力による回転
	inputAngle.x = 10.0f;
	//照準を正面より少し上に向ける
	FrontLook(frameTime);

	//糸発射
	threadTime += frameTime;
	if (threadTime >= 0.2f)
	{
		InputShotThread();
		threadTime = 0;
	}
}

bool EnemyAI::Search()
{
	for (int i = 0; i < teamMemberCount; i++)
	{
		//目標のマトリックスから計算
		Matrix4 mat = targetMats[i];
		Vector3 pos = RCMatrix4::getPosition(mat);
		Vector3 vec = RCVector3::normalize(pos - myPos);
		float dist = targetDists[i];
		//強制的に戦闘状態に入る距離に敵がいるか
		if (dist < level[levelNum].dangerLength)
		{
			//targetが確定
			target.mat = mat;
			target.pos = pos;
			target.vec = vec;
			target.distance = dist;
			targetNum = i;
			return true;
		}

		//視野距離以内にいるか
		//視野角内にいるか
		Vector3 front = RCVector3::normalize(-RCMatrix4::getFront(myMat));
		float acos = Math::acos(RCVector3::dot(front, vec));
		//敵から自身の間の線分にステージのポリゴンがあるか
		Vector3 start = myPos + RCMatrix4::getUp(myMat) * 3.0f;
		Vector3 end = pos + RCMatrix4::getUp(mat) * 3.0f;
		CollisionParameter stageCol = ModelRay(*stage._Get()->ReturnMat(), OCT_ID::STAGE_OCT, start, end);
		if (dist < level[levelNum].viewLength && acos < level[levelNum].viewAngle && !stageCol.colFlag)
		{
			//targetが確定
			target.mat = mat;
			target.pos = pos;
			target.vec = vec;
			target.distance = dist;
			targetNum = i;
			return true;
		}
	}
	return false;
}

void EnemyAI::SearchLineDraw()
{
	//Vector3 playerFront = -RCVector3::normalize(RCMatrix4::getFront(myMat));
	//Vector3 playerUp = RCVector3::normalize(RCMatrix4::getUp(myMat));
	//Vector3 playerLeft = RCVector3::normalize(RCMatrix4::getLeft(myMat));
	////上下(x軸回転)
	//Matrix4 rollmat = RCQuaternion::rotate(playerLeft, level[levelNum].viewAngle);
	//Vector3 frontRise = RCVector3::normalize(playerFront * rollmat);
	//rollmat = RCQuaternion::rotate(playerLeft, -level[levelNum].viewAngle);
	//Vector3 frontFall = RCVector3::normalize(playerFront * rollmat);
	////左右(y軸回転)
	//Matrix4 pitchmat = RCQuaternion::rotate(playerUp, level[levelNum].viewAngle);
	//Vector3 frontTurnRight = RCVector3::normalize(playerFront * pitchmat);
	//pitchmat = RCQuaternion::rotate(playerUp, -level[levelNum].viewAngle);
	//Vector3 frontTurnLeft = RCVector3::normalize(playerFront * pitchmat);

	//Graphic::GetInstance().DrawLine(myPos, myPos + frontRise * level[levelNum].viewLength, CAMERA_ID::ENEMY_CAMERA, vector3(0, 1, 0));
	//Graphic::GetInstance().DrawLine(myPos, myPos + frontFall * level[levelNum].viewLength, CAMERA_ID::ENEMY_CAMERA, vector3(0, 1, 0));
	//Graphic::GetInstance().DrawLine(myPos, myPos + frontTurnRight * level[levelNum].viewLength, CAMERA_ID::ENEMY_CAMERA, vector3(0, 1, 0));
	//Graphic::GetInstance().DrawLine(myPos, myPos + frontTurnLeft * level[levelNum].viewLength, CAMERA_ID::ENEMY_CAMERA, vector3(0, 1, 0));
}

bool EnemyAI::AttackArea()
{
	//①攻撃距離以内にいるか
	if (target.distance > level[levelNum].attackLength) return false;

	//②視野角内にいるか
	//AttackAreaLineDraw();
	float acos = Math::acos(RCVector3::dot(cameraFront, target.vec));
	if (acos > level[levelNum].attackAngle) return false;

	return true;
}

void EnemyAI::AttackAreaLineDraw()
{
	//Vector3 cameraPos = camera->CameraParam()->Eye;
	////上下(x軸回転)
	//Matrix4 rollmat = RCQuaternion::rotate(cameraLeft, level[levelNum].attackAngle);
	//Vector3 frontRise = RCVector3::normalize(cameraFront * rollmat);
	//rollmat = RCQuaternion::rotate(cameraLeft, -level[levelNum].attackAngle);
	//Vector3 frontFall = RCVector3::normalize(cameraFront * rollmat);
	////左右(y軸回転)
	//Matrix4 pitchmat = RCQuaternion::rotate(cameraUp, level[levelNum].attackAngle);
	//Vector3 frontTurnRight = RCVector3::normalize(cameraFront * pitchmat);
	//pitchmat = RCQuaternion::rotate(cameraUp, -level[levelNum].attackAngle);
	//Vector3 frontTurnLeft = RCVector3::normalize(cameraFront * pitchmat);

	//Graphic::GetInstance().DrawLine(myPos, myPos + frontRise * level[levelNum].attackLength, CAMERA_ID::ENEMY_CAMERA, vector3(1, 0, 0));
	//Graphic::GetInstance().DrawLine(myPos, myPos + frontFall * level[levelNum].attackLength, CAMERA_ID::ENEMY_CAMERA, vector3(1, 0, 0));
	//Graphic::GetInstance().DrawLine(myPos, myPos + frontTurnRight * level[levelNum].attackLength, CAMERA_ID::ENEMY_CAMERA, vector3(1, 0, 0));
	//Graphic::GetInstance().DrawLine(myPos, myPos + frontTurnLeft * level[levelNum].attackLength, CAMERA_ID::ENEMY_CAMERA, vector3(1, 0, 0));
}

void EnemyAI::InputCurlOn()
{
	player._Get()->SetInputCurlOn(true);
}

void EnemyAI::InputCurlOff()
{
	player._Get()->SetInputCurlOff(true);
}

void EnemyAI::InputJump()
{
	player._Get()->SetInputJump(true);
}

void EnemyAI::InputStep()
{
	player._Get()->SetInputStep(true);
}

void EnemyAI::InputShotThread()
{
	player._Get()->SetInputShot(true);
}

void EnemyAI::InputRewind()
{
	if (!player._Get()->ReturnThread().expired())//thread(weak_ptr)がemptyでなければ巻取り実行
		action->ChangeAction(ACTION_ID::REWIND_ACTION);
}

void EnemyAI::InputStepCurl()
{
	InputStep();
	InputCurlOn();
}

void EnemyAI::FinalInput()
{
	//最終的な入力結果(自身の前後左右)を渡す。移動の計算はその後プレイヤークラスに任せる。
	myParam->vec = inputVec;
	//最終的な入力結果(カメラ)を渡す。カメラの方向計算はその後カメラクラスに任せる。
	camera->AddInputAngleHV(inputAngle.x, inputAngle.y);
}