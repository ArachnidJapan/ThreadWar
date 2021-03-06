#include "UI.h"
#include  "../Other/Device.h"
#include "../Math/Vector2.h"
#include "../Math/MathUtility.h"
#include "../Graphic/Graphic.h"
#include "ID.h"
#include <string>
#include "Collision.h"
#include "../Math/Converter.h"
#include "CrystalCenter.h"
#include "Player.h"
#include "Stage.h"

UI::UI(IWorld& world_, std::weak_ptr<Stage> stage_,ACTOR_ID playerTeam_) :Actor(world_), stage(stage_),playerTeam(playerTeam_){
	Initialize();
}

UI::~UI(){

}
void UI::Initialize(){
	parameter.isDead = false;
	parameter.id = ACTOR_ID::UI_ACTOR;
	parameter.cubeScale = vector3(10.0f, 1.0f, 1.0f);
	hp = 2.0f;
	cursorAngle = 0;
	cursorSize = 0;
	cursorSizeTime = 0;
	cursorSizeTimeInterbal = 0;
	timeAlpha =90;
	arrowAlphaEndless = 0;
}
void UI::Update(float frameTime){
	world.EachActor(playerTeam, [&](const Actor& other){
		if (static_cast<Player*>(const_cast<Actor*>(&other))->ReturnP1()){
			hp = static_cast<Player*>(const_cast<Actor*>(&other))->ReturnPlayerParameter()->hp;
		}
	});

	////rectのデバッグ。
	//if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_O))
	//	barNum = max(barNum - 0.01f, 0.0f);
	//if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_P))
	//	barNum = min(barNum + 0.01f, 1.0f);
	//if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_U, true))drawFlag = !drawFlag;

	cursorAngle -= 360.0f * frameTime * 2.0f;
	if (cursorAngle <= -360){
		cursorAngle = 0;
	}

	cursorSize = 1.0f;
	cursorSizeTimeInterbal += (1.0f + abs(hp - 2.0f) * 4.0f) * frameTime;
	if (cursorSizeTimeInterbal > 2.0f){
		cursorSizeTime += 10.0f * frameTime;
		if (cursorSizeTime < 1.0f){
			cursorSize = Math::lerp(1.0f, 1.5f, cursorSizeTime);
		}
		else if (cursorSizeTime < 2.0f){
			cursorSize = Math::lerp(1.5f, 1.0f, cursorSizeTime - 1.0f);
		}
		else if (cursorSizeTime < 3.0f){
			cursorSize = Math::lerp(1.0f, 1.3f, (cursorSizeTime - 2.0f) * 2.0f);
		}
		else if (cursorSizeTime < 3.5f){
			cursorSize = Math::lerp(1.3f, 1.0f, (cursorSizeTime - 3.0f) * 2.0f);
		}
		else{
			cursorSizeTime = 0.0f;
			cursorSizeTimeInterbal = 0.0f;
		}
	}
	int sec = stage._Get()->ReturnGameTime();
	if (sec <= 10)
	timeAlpha += 5.0f * frameTime;

	arrowAlphaEndless += 15.0f * frameTime;
	if (arrowAlphaEndless > 360)arrowAlphaEndless = 0;
}

void UI::Draw(CAMERA_ID cID) const{
	//if (drawFlag){
	std::vector<Vector3> pointPos;
	std::vector<bool>	isRespown;
	std::vector<bool>	damageDelay;
	Vector3 playerPos, playerFront, playerUp;
	world.EachActor(playerTeam, [&](const Actor& other){
		if (static_cast<Player*>(const_cast<Actor*>(&other))->ReturnP1()){
			playerPos = RCMatrix4::getPosition(const_cast<Actor*>(&other)->GetParameter().matrix);
			playerFront = RCVector3::normalize(RCMatrix4::getFront(const_cast<Actor*>(&other)->GetParameter().matrix));
			playerUp = RCVector3::normalize(RCMatrix4::getUp(const_cast<Actor*>(&other)->GetParameter().matrix));
		}
		else{
			pointPos.push_back(RCMatrix4::getPosition(const_cast<Actor*>(&other)->GetParameter().matrix));
			isRespown.push_back(static_cast<Player*>(const_cast<Actor*>(&other))->IsRespawn());
			damageDelay.push_back(static_cast<Player*>(const_cast<Actor*>(&other))->ReturnDamageDelay());
		}
	});
	for (int a = pointPos.size() - 1; a >= 0; a--){
		Vector3 p = pointPos[a] * RConvert(&Device::GetInstance().GetCamera(cID)->returnView());
		Vector3 vec = RCVector3::normalize(pointPos[a] - playerPos);
		p = p * RConvert(&Device::GetInstance().GetCamera(cID)->returnProj());
		p.x /= p.z;
		p.y /= p.z;
		float arrowAlpha = 1.0f;
		if(damageDelay[a])arrowAlpha = Math::lerp(0.0f, 1.0f, abs(sin(arrowAlphaEndless)));
		if (isRespown[a])arrowAlpha = 0.3f;
		//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 100), vector2(0.20f, 0.25f), 0.5f, "PlayerPos.x:" + std::to_string(p.x) + "f");
		//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 125), vector2(0.20f, 0.25f), 0.5f, "PlayerPos.y:" + std::to_string(p.y) + "f");
		if (p.x >= -1.0f && p.x <= 1.0f && p.y >= -1.0f && p.y <= 1.0f && p.z >= 0.0f && p.z <= 1.0f){
			p.x += 1.0f;
			p.y += 1.0f;
			p.x *= (1920.0f / 2.0f);
			p.y *= (1080.0f / 2.0f);
			Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(p.x, p.y + 60.0f), vector2(0.25f, 0.40f), 0.5f, "cp" + std::to_string(a + 1), vector3(0, 1, 0), arrowAlpha, true);
			Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(p.x, p.y + 30.0f), vector2(0.20f, 0.30f), 0.5f, std::to_string((int)(RCVector3::length(playerPos - pointPos[a]))) + "m", vector3(0, 1, 0), arrowAlpha, true);
		}
		else{
			vec.y = 0;
			playerFront.y = 0;
			float angle = Math::angle(acos(RCVector3::dot(vec, playerFront)));
			Vector3 center = vector3(1920.0f / 2.0f, 1080.0f / 2.0f, 0);
			Matrix4 centerMat = RCMatrix4::translate(vector3(1920.0f / 2.0f, 1080.0f / 2.0f, 0));
			Matrix4 trans = RCMatrix4::translate(vector3(0, -200, 0));
			if (RCVector3::cross(vec, playerFront).y < 0 && playerUp.y > 0 || RCVector3::cross(vec, playerFront).y > 0 && playerUp.y < 0){
				angle = -angle;
			}
			Matrix4 rotate = RCMatrix4::rotateZ(angle);
			centerMat = trans * rotate * centerMat;
			center = RCMatrix4::getPosition(centerMat);
			Graphic::GetInstance().DrawTexture(TEXTURE_ID::ARROW_TEXTURE, vector2(center.x, center.y), vector2(0.3f, 1.0f), D3DXCOLOR(1, 1, 1, arrowAlpha), vector2(0.5f, 0.5f), 0, 0, 1, 1, angle);
			Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(center.x, center.y + 50.0f), vector2(0.25f, 0.40f), 0.5f, "CP" + std::to_string(a + 1), vector3(0, 1, 0), arrowAlpha, true);
			Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(center.x, center.y + 20.0f), vector2(0.20f, 0.30f), 0.5f, std::to_string((int)(RCVector3::length(playerPos - pointPos[a]))) + "m", vector3(0, 1, 0), arrowAlpha, true);
		}
	}
	Graphic::GetInstance().DrawTexture(TEXTURE_ID::FLOOR_TEXTURE, vector2(1920 - 640 + 175, 0), vector2(1.0f, 1.0f), D3DXCOLOR(1, 1, 1, 1), vector2(0.0f, 0.0f), 0.0f, 0.0f, 1.0f, 1.0f);

	Vector3 crystalPos = vector3(-2.4f, 0, -1.5f) + (vector3(0.5f, 0, 1.5f) - vector3(-2.4f, 0, -1.5f)) * 0.5f;
	crystalPos *= 4.55f;
	world.EachActor(ACTOR_ID::CRYSTAL_CENTER_ACTOR, [&](const Actor& other){
		Vector3 color = vector3(1, 1, 1);
		CrystalParameter c = static_cast<CrystalCenter*>(const_cast<Actor*>(&other))->ReturnCrystalParameter();
		if (c.crystalISPlayer)color = vector3(1, 0, 0);
		if (c.crystalISEnemy)color = vector3(0, 0, 1);
		Graphic::GetInstance().DrawTexture(TEXTURE_ID::POINT_TEXTURE, vector2(1920.0f - 320.0f + crystalPos.x + 175, 320.0f + crystalPos.z * 2.0f / 3.0f), vector2(2, 2), D3DXCOLOR(color.x, color.y, color.z, 1), vector2(0.5f, 0.5f), 0.0f, 0.0f, 1.0f, 1.0f);
	});
	crystalPos = vector3(-2.4f, 0, 62.0f) + (vector3(0.5f, 0, 65.3f) - vector3(-2.4f, 0, 62.0f)) * 0.5f;
	crystalPos *= 4.55f;
	world.EachActor(ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR, [&](const Actor& other){
		Vector3 color = vector3(1, 1, 1);
		CrystalParameter c = static_cast<CrystalCenter*>(const_cast<Actor*>(&other))->ReturnCrystalParameter();
		if (c.crystalISPlayer)color = vector3(1, 0, 0);
		if (c.crystalISEnemy)color = vector3(0, 0, 1);
		Graphic::GetInstance().DrawTexture(TEXTURE_ID::POINT_TEXTURE, vector2(1920.0f - 320.0f + crystalPos.x + 175, 320.0f + crystalPos.z * 2.0f / 3.0f), vector2(2, 2), D3DXCOLOR(color.x, color.y, color.z, 1), vector2(0.5f, 0.5f), 0.0f, 0.0f, 1.0f, 1.0f);
	});
	crystalPos = vector3(-2.4f, 0, -67.0f) + (vector3(0.5f, 0, -64.0f) - vector3(-2.4f, 0, -67.0f)) * 0.5f;
	crystalPos *= 4.55f;
	world.EachActor(ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR, [&](const Actor& other){
		Vector3 color = vector3(1, 1, 1);
		CrystalParameter c = static_cast<CrystalCenter*>(const_cast<Actor*>(&other))->ReturnCrystalParameter();
		if (c.crystalISPlayer)color = vector3(1, 0, 0);
		if (c.crystalISEnemy)color = vector3(0, 0, 1);
		Graphic::GetInstance().DrawTexture(TEXTURE_ID::POINT_TEXTURE, vector2(1920.0f - 320.0f + crystalPos.x + 175, 320.0f + crystalPos.z * 2.0f / 3.0f), vector2(2, 2), D3DXCOLOR(color.x, color.y, color.z, 1), vector2(0.5f, 0.5f), 0.0f, 0.0f, 1.0f, 1.0f);
	});
	int resCount = 0;
	for (auto ia : pointPos){
		float arrowAlpha = 1.0f;
		if (damageDelay[resCount])arrowAlpha = Math::lerp(0.0f, 1.0f, abs(sin(arrowAlphaEndless)));
		Vector3 i = ia * 4.55f;
		D3DXCOLOR resColor = D3DXCOLOR(0, 1, 0, arrowAlpha);
		if (isRespown[resCount])resColor = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1);
		resCount++;
		Graphic::GetInstance().DrawTexture(TEXTURE_ID::POINT_TEXTURE, vector2(1920.0f - 320.0f + i.x + 175, 320.0f + i.z * 2.0f / 3.0f), vector2(1.0f, 1.0f), resColor, vector2(0.5f, 0.5f), 0.0f, 0.0f, 1.0f, 1.0f);
	}
	playerPos *= 4.55f;
	Graphic::GetInstance().DrawTexture(TEXTURE_ID::POINT_TEXTURE, vector2(1920.0f - 320.0f + playerPos.x + 175, 320.0f + playerPos.z * 2.0f / 3.0f), vector2(1.0f, 1.0f), playerTeam == ACTOR_ID::PLAYER_ACTOR ? D3DXCOLOR(1, 0, 0, 1) : D3DXCOLOR(0, 0, 1, 1), vector2(0.5f, 0.5f), 0.0f, 0.0f, 1.0f, 1.0f);
	Graphic::GetInstance().DrawTexture(TEXTURE_ID::CURSOR_TEXTURE, vector2(1920 / 2, 1080 / 2), vector2(0.6f, 0.6f), D3DXCOLOR(1, 1, 1, 1), vector2(0.5f, 0.5f), 0, 0, 1, 1, cursorAngle);
	Graphic::GetInstance().DrawTexture(TEXTURE_ID::CURSOR2_TEXTURE, vector2(1920 / 2, 1080 / 2), vector2(0.4f, 0.4f) * cursorSize, D3DXCOLOR(1, 1, 1, 1), vector2(0.5f, 0.5f), 0, 0, 1, 1, cursorAngle);
	Graphic::GetInstance().DrawTexture(TEXTURE_ID::CURSOR2_TEXTURE, vector2(1920 / 2, 1080 / 2), vector2(0.4f, 0.4f) * cursorSize, D3DXCOLOR(1, 1, 1, 1), vector2(0.5f, 0.5f), 0, 0, 1, 1, -cursorAngle);
	int min = stage._Get()->ReturnGameTime() / 60.0f;
	int sec = stage._Get()->ReturnGameTime() - min * 60;
	int sec2 = stage._Get()->ReturnGameTime();
	Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(1920.0f / 2.0f, 1080.0f - 140.0f + (sec2 <= 10 ? -30.0f : 0.0f)), vector2(0.5f, 0.8f) * (sec2 <= 10 ? 2.0f : 1.0f), 0.5f, "0" + std::to_string(min) + ":" + (std::to_string(sec).size() > 1 ? "" : "0") + std::to_string(sec), vector3(1, 1, 1), Math::lerp(0.5f, 1.0f, abs(sin(timeAlpha))), true);
	//ゲージ
	if (stage._Get()->ReturnTotalPoint() == 0)
	{
		//各チームゲージ
		Graphic::GetInstance().DrawTexture(TEXTURE_ID::GAUGE_TEXTURE, vector2(1920 / 2, 1080 - 56), vector2(1.3f, 1.3f), D3DXCOLOR(1, 0, 0, 1), vector2(0.5f, 0.5f), 0.0f, 0.0f, 0.5f, 1.0f);
		Graphic::GetInstance().DrawTexture(TEXTURE_ID::GAUGE_TEXTURE, vector2(1920 / 2, 1080 - 56), vector2(1.3f, 1.3f), D3DXCOLOR(0, 0, 1, 1), vector2(0.5f, 0.5f), 0.5f, 0.0f, 1.0f, 1.0f);
		//真ん中の黒棒
		Graphic::GetInstance().DrawTexture(TEXTURE_ID::GAUGE_CENTER_TEXTURE, vector2(1920 / 2, 1080 - 56), vector2(1, 1), D3DXCOLOR(1, 1, 1, 1), vector2(0.5f, 0.5f), 0, 0.0f, 1.0f, 1.0f);

		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(1920.0f / 2.0f - 600.0f,1080.0f - 60.0f), vector2(0.3f, 0.5f) * 1.5f , 0.5f, std::to_string(stage._Get()->ReturnPlayerTeamPoint()) + "pts", vector3(1, 0, 0), 1.0f, true);
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(1920.0f / 2.0f + 600.0f, 1080.0f - 60.0f), vector2(0.3f, 0.5f) * 1.5f, 0.5f, std::to_string(stage._Get()->ReturnEnemyTeamPoint()) + "pts", vector3(0, 0, 1), 1.0f, true);
	}
	else
	{
		//各チームゲージ
		Graphic::GetInstance().DrawTexture(TEXTURE_ID::GAUGE_TEXTURE, vector2(1920 / 2, 1080 - 56), vector2(1.3f, 1.3f), D3DXCOLOR(1, 0, 0, 1), vector2(0.5f, 0.5f), 0.0f, 0.0f, (float)stage._Get()->ReturnPlayerTeamPoint() / (float)stage._Get()->ReturnTotalPoint(), 1.0f);
		Graphic::GetInstance().DrawTexture(TEXTURE_ID::GAUGE_TEXTURE, vector2(1920 / 2, 1080 - 56), vector2(1.3f, 1.3f), D3DXCOLOR(0, 0, 1, 1), vector2(0.5f, 0.5f), (float)stage._Get()->ReturnPlayerTeamPoint() / (float)stage._Get()->ReturnTotalPoint(), 0.0f, 1.0f, 1.0f);
		//真ん中の黒棒
		Graphic::GetInstance().DrawTexture(TEXTURE_ID::GAUGE_CENTER_TEXTURE, vector2(1920 / 2 + (1024.0f * 1.3f / (float)stage._Get()->ReturnTotalPoint()) * ((float)stage._Get()->ReturnPlayerTeamPoint() - ((float)stage._Get()->ReturnTotalPoint() / 2)), 1080 - 56), vector2(1, 1), D3DXCOLOR(1, 1, 1, 1), vector2(0.5f, 0.5f), 0, 0.0f, 1.0f, 1.0f);
		
		//ポイント
		float playerWin = Math::lerp(1.0f, 2.0f, (float)stage._Get()->ReturnPlayerTeamPoint() / (float)stage._Get()->ReturnTotalPoint());
		float enemyWin = Math::lerp(1.0f, 2.0f, (float)stage._Get()->ReturnEnemyTeamPoint() / (float)stage._Get()->ReturnTotalPoint());
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(1920.0f / 2.0f - 600.0f, 1080.0f - 60.0f), vector2(0.3f, 0.5f) * playerWin, 0.5f, std::to_string(stage._Get()->ReturnPlayerTeamPoint()) + "pt", vector3(1, 0, 0), 1.0f, true);
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(1920.0f / 2.0f + 600.0f, 1080.0f - 60.0f), vector2(0.3f, 0.5f) * enemyWin, 0.5f, std::to_string(stage._Get()->ReturnEnemyTeamPoint()) + "pt", vector3(0, 0, 1), 1.0f, true);
	}
	
	Graphic::GetInstance().DrawTexture(TEXTURE_ID::DAMAGE_TEXTURE, vector2(1920 / 2, 1080 / 2), vector2(1.5f, 1.5f), D3DXCOLOR(1, 1, 1, (float)(2 - max(0, hp)) / 2.0f), vector2(0.5f, 0.5f), 0.0f, 0.0f, 1, 1.0f);
	
	float startTime = stage._Get()->ReturnStartTime();
	if (startTime > 2.0f){
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(1920.0f / 2.0f, 1080.0f / 2.0f), vector2(3.0f, 5.0f) * 0.5f,// * (3.0f - startTime),
			0.5f,"3", vector3(1, 1, 1), 1.0f - (3.0f - startTime), true);													  //
	}																														  //
	else if (startTime > 1.0f){																								  //
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(1920.0f / 2.0f, 1080.0f / 2.0f), vector2(3.0f, 5.0f) * 0.5f,// * (2.0f - startTime),
			0.5f, "2", vector3(1, 1, 1), 1.0f - (2.0f - startTime), true);													  //
	}																														  //
	else if (startTime > 0.0f){																								  //
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(1920.0f / 2.0f, 1080.0f / 2.0f), vector2(3.0f, 5.0f) * 0.5f,// * (1.0f - startTime),
			0.5f, "1", vector3(1, 1, 1), 1.0f - (1.0f - startTime), true);
	}
	else if (startTime > -1.0f){
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(1920.0f / 2.0f, 1080.0f / 2.0f), vector2(3.0f, 5.0f) * 0.3f + vector2(3,5) *abs(startTime) * 0.1f,
			0.5f, "START", vector3(1, 1, 1), 1.0f - abs(startTime), true);
	}
	
	if (sec2 <= 0){
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(1920.0f / 2.0f, 1080.0f / 2.0f), vector2(0.5f, 0.8f) * 3, 0.5f,"TIME UP", vector3(1, 1, 1), 1.0f, true);
	}
	//	}

	pointPos.clear();
	isRespown.clear();
	damageDelay.clear();
}

void UI::OnCollide(Actor& other, CollisionParameter colpara){
}

void UI::SetHP(float hp_){
	hp = hp_;
}