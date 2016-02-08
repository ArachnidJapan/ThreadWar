#pragma once
#include "../Actor/ID.h"
#include "../TeamSelect/SelectPlayerParam.h"

class SceneParameter{
public:
	SceneParameter(){
		vID = VICTORY_ID::PLAYER_WIN;
	}
	~SceneParameter(){}
	void SetVictoryID(VICTORY_ID vID_){ vID = vID_; }
	void SetTeamSelectResult(TeamSelectResult& tsr_){ tsr = tsr_; }

	VICTORY_ID* ReturnVictoryID(){ return &vID; }
	TeamSelectResult* ReturnTeamSelectResult(){ return &tsr; }
private:
	VICTORY_ID vID;
	TeamSelectResult tsr;
};