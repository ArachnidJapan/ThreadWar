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

}
void AITargetManager::Update(IWorld& world)
{
	//ê‘É`Å[ÉÄ(ì¥åAë§)
	redHaveNum = 0;
	world.EachActor(ACTOR_ID::PLAYER_ACTOR, [&](const Actor& other){
		Player* p = static_cast<Player*>(const_cast<Actor*>(&other));
		if (!p->IsRespawn())
		{
			redMatList[redHaveNum] = p->GetParameter().matrix;
			redPosList[redHaveNum] = RCMatrix4::getPosition(blueMatList[redHaveNum]);
			redHaveNum++;
		}
	});

	//ê¬É`Å[ÉÄ(ïXë§)
	blueHaveNum = 0;
	world.EachActor(ACTOR_ID::ENEMY_ACTOR, [&](const Actor& other){
		Player* p = static_cast<Player*>(const_cast<Actor*>(&other));
		if (!p->IsRespawn())
		{
			blueMatList[blueHaveNum] = p->GetParameter().matrix;
			bluePosList[blueHaveNum] = RCMatrix4::getPosition(blueMatList[blueHaveNum]);
			blueHaveNum++;
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

void AITargetManager::SetAllPosList(int playerNum, Vector3 pos)
{
	allPosList[playerNum] = pos;
}
std::array<Vector3, 8> AITargetManager::GetAllPosList()
{
	return allPosList;
}