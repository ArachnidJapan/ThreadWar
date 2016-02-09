#include "Fade.h"
#include "../../Graphic/Graphic.h"
#include "../../Math/MathUtility.h"

// �R���X�g���N�^
Fade::Fade() :
isFadeIn(false),
time(0),
currentTime(0),
isupdate(false)
{
}

// �J�n
void Fade::start(bool _isFadeIn, float _time)
{
	isFadeIn = _isFadeIn;
	time = _time;
	isupdate = true;
	currentTime = 0;
}

// �X�V
void Fade::update(float deltaTime)
{
	if (!isupdate) return;
	if (currentTime > time) isupdate = false;

	// �^�C���̍X�V
	currentTime += deltaTime;
}

// ���쒆���H
bool Fade::getIsUpdate()
{
	return isupdate;
}

void Fade::draw() const
{
	//if (!isupdate) return;
	float alpha = 0.0f;
	if (isFadeIn)
	{
		alpha = 1.0f - Math::clamp(currentTime / time, 0.0f, 1.0f);
	}
	else
	{
		alpha = Math::clamp(currentTime / time, 0.0f, 1.0f);
	}
	Graphic::GetInstance().DrawTexture(TEXTURE_ID::BLACK_TEXTURE, vector2(0, 0), vector2(1920, 1080), D3DXCOLOR(1, 1, 1, alpha), vector2(0, 0));
	
}