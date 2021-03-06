#pragma once
#include "Actor.h"
#include <vector>
class Thread;
class IWorld;
class Stage;

//蜘蛛の巣3頂点
struct ThreadWebParameter
{
	Vector3 a, b, c;
};

class ThreadWeb : public Actor, public std::enable_shared_from_this<ThreadWeb>
{
public:
	ThreadWeb(IWorld& world, std::shared_ptr<Thread> parent1, std::shared_ptr<Thread> parent2,std::weak_ptr<Stage> stage);
	~ThreadWeb();
	virtual void Initialize() override;
	virtual void Update(float frameTime) override;
	virtual void Draw(CAMERA_ID cID) const override;
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	void PlusHitCount(int hitPoint);
	ThreadWebParameter GetThreadWebParameter();

	//蜘蛛の巣の間に敷かれる糸を計算
	//mWebParamater.aを起点として計算
	void WebThreadsCalc();

	ACTOR_ID GetRootActor();
	void SetSelectThreadWeb(){ selectThreadWeb = true; };

private:
	//生成の元になった糸
	std::weak_ptr<Thread> mParent1;
	std::weak_ptr<Thread> mParent2;

	ThreadWebParameter mWebParamater;
	//蜘蛛の巣の間に敷かれる糸を格納
	std::vector<Vector3> mWebThreads;
	//蜘蛛の巣の間に敷かれる糸の分割数
	int mDivide;

	int hitCount;
	Vector3 color;

	bool selectThreadWeb;
};