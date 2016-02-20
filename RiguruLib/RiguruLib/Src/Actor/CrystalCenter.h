#pragma once

#include "../Other/DX11User.h"
#include "../Other/D3D11User.h"
#include <map>
#include "../Math/Matrix4.h"
#include "../Math/MatrixUtility.h"
#include "../Graphic/Rgr/MeshUser.h"
#include "Actor.h"

struct CrystalParameter{
	bool crystalISPlayer;
	bool crystalISEnemy;
};

class CrystalCenter :public Actor, public std::enable_shared_from_this<CrystalCenter>{
public:
	CrystalCenter(IWorld& world_, ACTOR_ID crystalID,bool gamePlay_ = true,bool teamSelect_ = false,int point_ = 1000);
	~CrystalCenter();
	virtual void Initialize() override;
	virtual void Update(float frameTime) override;
	virtual void Draw(CAMERA_ID cID, bool first) const override;
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	void DrawEffect(Vector3 pos, Vector3 scale_, CAMERA_ID cID, Vector3 color) const;
	void Damage(bool player);
	void SetStageMatrix(Matrix4 mat){parameter.matrix = mat;};
	bool IsPlayer();
	bool IsEnemy();
	//point�����Z�b�g����
	void Reset();
	CrystalParameter ReturnCrystalParameter(){ return crystalParameter; }
private:
	CrystalParameter crystalParameter;
	int point;
	bool crystalISPlayer, crystalISEnemy;
	bool keep;//�F���|�C���g��0�ɂȂ�܂ŃL�[�v����
	float angle;
	Vector3 scaleStart;
	bool startFlag;
	bool teamSelect;
	bool gamePlay;
	float crystalAngle;
};