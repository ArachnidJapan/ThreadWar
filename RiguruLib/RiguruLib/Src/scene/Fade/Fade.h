#ifndef FADE_H
#define FADE_H

// �t�F�[�h�N���X
class Fade
{
public:
	// �R���X�g���N�^
	Fade();

	// �N��
	void start(bool _isFadeIn, float _time);

	// �X�V
	void update(float deltaTime);

	// �`��
	void draw() const;

	// ���쒆���H
	bool getIsUpdate();

private:
	// �t�F�[�h�ɂ����鎞��
	float time;
	
	// ���݂̌o�ߎ���
	float currentTime;

	// �t�F�[�h�C�����H
	bool isFadeIn;

	// ���쒆���H
	bool isupdate;
};

#endif