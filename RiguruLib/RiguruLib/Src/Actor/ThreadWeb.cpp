#include "ThreadWeb.h"
#include "../math/Converter.h"
#include "Thread.h"
#include "../Graphic/Graphic.h"
#include "Collision.h"
#include "Stage.h"

ThreadWeb::ThreadWeb(IWorld& world, std::shared_ptr<Thread> parent1, std::shared_ptr<Thread> parent2, std::weak_ptr<Stage> stage)
:Actor(world), mParent1(parent1), mParent2(parent2){
	parameter.id = ACTOR_ID::PLAYER_THREADWEB_ACTOR;
	if (parent1->GetRootActor() != ACTOR_ID::PLAYER_ACTOR)
		parameter.id = ACTOR_ID::ENEMY_THREADWEB_ACTOR;
	parameter.isDead = false;
	hitCount = 0;
	mWebParamater = {
		mParent1._Get()->GetThreadParameter().startPosition,
		mParent1._Get()->GetThreadParameter().endPosition,
		mParent2._Get()->GetThreadParameter().endPosition };
	mWebThreads.clear();
	mDivide = 20;

	//’wå‚Ì‘ƒ‚ÌŠÔ‚É•~‚­Ž…‚ÌÀ•W‚ðŒvŽZ‚µŠi”[
	WebThreadsCalc();

	Matrix4 stageMatrix = stage._Get()->GetParameter().matrix;
	for (int i = 0; i < mDivide * 2; i += 2)
	{
		CollisionParameter cp = ModelRay(stageMatrix, OCT_ID::STAGE_OCT, mWebThreads[i], mWebThreads[i + 1]);
		if (cp.colFlag){
			if (i <= 1){
				parameter.isDead = true;
				break;
			}
			mWebParamater.b = mWebThreads[i-2];
			mWebParamater.c = mWebThreads[i-2 + 1];

			break;
		}
	}

	mWebThreads.clear();
	//’wå‚Ì‘ƒ‚ÌŠÔ‚É•~‚­Ž…‚ÌÀ•W‚ðŒvŽZ‚µŠi”[
	WebThreadsCalc();


	selectThreadWeb = false;
}

ThreadWeb::~ThreadWeb(){

}

void ThreadWeb::Initialize(){

}
void ThreadWeb::Update(float frameTime) {
	
	if (mParent1.lock() == nullptr || mParent2.lock() == nullptr){
		parameter.isDead = true;
	}
	else{
		color = vector3(1, 0.5f, 0.5f);
		if (mParent1._Get()->GetParameter().id == ACTOR_ID::ENEMY_THREAD_ACTOR)color = vector3(0.5f, 0.5f, 1);
		if (selectThreadWeb)color = vector3(1, 1, 1);
		selectThreadWeb = false;
	}
}
void ThreadWeb::Draw(CAMERA_ID cID, bool first)const{
	for (int i = 0; i < mDivide * 2; i += 2)
	{
		Graphic::GetInstance().DrawLine(mWebThreads[i], mWebThreads[i + 1], cID, color);
	}
}
void ThreadWeb::OnCollide(Actor& other, CollisionParameter colpara){

}

ThreadWebParameter ThreadWeb::GetThreadWebParameter()
{
	return mWebParamater;
}

void ThreadWeb::WebThreadsCalc()
{
	Vector3 ab = mWebParamater.b - mWebParamater.a;
	Vector3 ac = mWebParamater.c - mWebParamater.a;
	Vector3 n_ab = RCVector3::normalize(ab);
	Vector3 n_ac = RCVector3::normalize(ac);

	//Ž…“¯Žm‚ÌŠÔŠu
	float inter_ab = RCVector3::length(ab) / mDivide;
	float inter_ac = RCVector3::length(ac) / mDivide;

	for (int i = 0; i < mDivide; i++)
	{
		Vector3 p = mWebParamater.a + n_ab * inter_ab * (float)(i + 1);
		mWebThreads.push_back(p);
		p = mWebParamater.a + n_ac * inter_ac * (float)(i + 1);
		mWebThreads.push_back(p);
	}
}

ACTOR_ID ThreadWeb::GetRootActor(){
    return mParent1._Get()->GetRootActor(); 
}

void ThreadWeb::PlusHitCount(int hitPoint){
	hitCount+=hitPoint;
	if (hitCount >= 10){
		parameter.isDead = true;
	}
}