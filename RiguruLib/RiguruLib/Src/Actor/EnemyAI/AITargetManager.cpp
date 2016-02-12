#include "AITargetManager.h"
#include "../../Actor/Player.h"

AITargetManager::AITargetManager()
{

}
AITargetManager::~AITargetManager()
{
	
}

void AITargetManager::Initialize(IWorld& world)
{
	//クリスタルの位置
	crystalPosList[0] = iceCrystalPos;
	crystalPosList[1] = centerCrystalPos;
	crystalPosList[2] = caveCrystalPos;
}
void AITargetManager::Update(IWorld& world)
{
	//赤チーム(洞窟側)
	redHaveNum = 0;
	world.EachActor(ACTOR_ID::PLAYER_ACTOR, [&](const Actor& other){
		Player* p = static_cast<Player*>(const_cast<Actor*>(&other));
		if (!p->IsRespawn())
		{
			redMatList[redHaveNum] = p->GetParameter().matrix;
			redPosList[redHaveNum] = RCMatrix4::getPosition(blueMatList[redHaveNum]);
			redHaveNum++;
		}
		else
		{
			redHaveNum = 0;
		}
	});

	//青チーム(氷側)
	blueHaveNum = 0;
	world.EachActor(ACTOR_ID::ENEMY_ACTOR, [&](const Actor& other){
		Player* p = static_cast<Player*>(const_cast<Actor*>(&other));
		if (!p->IsRespawn())
		{
			blueMatList[blueHaveNum] = p->GetParameter().matrix;
			bluePosList[blueHaveNum] = RCMatrix4::getPosition(blueMatList[blueHaveNum]);
			blueHaveNum++;
		}
		else
		{
			blueHaveNum = 0;
		}
	});
}


std::array<Matrix4, 4> AITargetManager::GetRedTeamMatList()
{
	return redMatList;
}
std::array<Matrix4, 4> AITargetManager::GetBlueTeamMatList()
{
	return blueMatList;
}
std::array<Vector3, 4> AITargetManager::GetRedTeamPosList()
{
	return redPosList;
}
std::array<Vector3, 4> AITargetManager::GetBlueTeamPosList()
{
	return bluePosList;
}
std::array<Vector3, 3> AITargetManager::GetCrystalPosList()
{
	return crystalPosList;
}

std::array<Matrix4, 4> AITargetManager::GetTeamMatListID(ACTOR_ID id)
{
	if (id == ACTOR_ID::PLAYER_ACTOR) return redMatList;
	else  return blueMatList;
}

std::array<Vector3, 4> AITargetManager::GetTeamPosListID(ACTOR_ID id)
{
	if (id == ACTOR_ID::PLAYER_ACTOR) return redPosList;
	else  return bluePosList;
}
int AITargetManager::GetHaveNum(ACTOR_ID id)
{
	if (id == ACTOR_ID::PLAYER_ACTOR) return redHaveNum;
	else  return blueHaveNum;
}

int AITargetManager::GetRedHaveNum()
{
	return redHaveNum;
}
int AITargetManager::GetBlueHaveNum()
{
	return blueHaveNum;
}
