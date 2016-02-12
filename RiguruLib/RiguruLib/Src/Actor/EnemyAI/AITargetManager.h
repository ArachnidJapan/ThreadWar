/**====================================================================*/
/**
* AIの目標を一括管理
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

	//プレイヤーのマトリックスリストを取得
	std::array<Matrix4, 4> GetRedTeamMatList();
	std::array<Matrix4, 4> GetBlueTeamMatList();
	//プレイヤーのポジションリストを取得
	std::array<Vector3, 4> GetRedTeamPosList();
	std::array<Vector3, 4> GetBlueTeamPosList();
	//クリスタルのポジションリストを取得
	std::array<Vector3, 3> GetCrystalPosList();

	//取得したいチームのIDを入れるとそちらを返す PLAYER_ACTOR→赤 ENEMY_ACTOR→青
	std::array<Matrix4, 4> GetTeamMatListID(ACTOR_ID id);
	std::array<Vector3, 4> GetTeamPosListID(ACTOR_ID id);
	int GetHaveNum(ACTOR_ID id);

	//プレイヤーの人数を取得
	int GetRedHaveNum();
	int GetBlueHaveNum();

private:
	//クリスタルの位置　中央
	const Vector3 centerCrystalPos = RCVector3::lerp(vector3(-2.4f, -1.3f, -1.5f), vector3(0.5f, 5.5f, 1.5f), 0.5);
	//氷側(PlayerSide)
	const Vector3 iceCrystalPos = RCVector3::lerp(vector3(-2.4f, -1.3f, 62.0f), vector3(0.5f, 5.5f, 65.3f), 0.5);
	//洞窟側(EnemySide)
	const Vector3 caveCrystalPos = RCVector3::lerp(vector3(-2.4f, -1.3f, -67.0f), vector3(0.5f, 5.5f, -64.0f), 0.5);

	//氷側チーム4体のマトリックス
	std::array<Matrix4, 4> redMatList;
	//洞窟側チーム4体のマトリックス
	std::array<Matrix4, 4> blueMatList;
	//氷側チーム4体のポジション
	std::array<Vector3, 4> redPosList;
	//洞窟側チーム4体のポジション
	std::array<Vector3, 4> bluePosList;
	//クリスタル3箇所
	std::array<Vector3, 3> crystalPosList;

	//赤チーム人数
	int redHaveNum;
	//青チーム人数
	int blueHaveNum;
};