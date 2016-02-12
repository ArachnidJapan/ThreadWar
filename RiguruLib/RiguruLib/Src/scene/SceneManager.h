#pragma once

#include "ISceneMediator.h"
#include"IScenePtr.h"
#include "Fade\Fade.h"
#include <unordered_map>
enum class Scene;

class SceneManager : public ISceneMediator{
public:
	static const int MaxStageCount;

public:
	static SceneManager &GetInstance(){
		static SceneManager s;
		return s;
	}
	//�R���X�g���N�^
	SceneManager();
	//�X�V�O������
	void Initialize();
	//�X�V����
	void Update(float frameTime);
	//�`�揈��
	void Draw() const;
	//�I��������
	void End();

	void Change();

	//�V�[���̒ǉ�
	void Add(Scene name, const IScenePtr& scene);
	void SetScene(Scene name);
	template<typename T, typename... Args> 
	void Add(Scene name, Args&&... arg)
	{
		Add(name, std::make_shared<T>(arg...));
	}
	//�V�[���̕ύX
	virtual void Change(Scene name) override;
	// ���������w�肷��
	virtual void Init(Scene name) override;
	// �I���������w�肷��
	virtual void Final(Scene name) override;

	virtual int GetStageCount() const override
	{
		return mStageCount;
	}
	virtual void SetStageCount(int n) override;

	__declspec(property(get = GetStageCount, put = SetStageCount)) int StageCount;

	// �t�F�[�h�J�n
	void fadestart(bool isFadeIn, float time);

	// �t�F�[�h���Ă��邩�H
	bool isFadeUpdate();
private:
	//�R�s�[�֎~
	SceneManager(const SceneManager& other) = delete;
	SceneManager& operator = (const SceneManager& other) = delete;
private:
	//�V�[��
	std::unordered_map<Scene, IScenePtr> mScenes;
	//���݂̃V�[��
	IScenePtr mCurrentScene;
	int mStageCount;

	// �t�F�[�h�p�N���X
	Fade fade;
	bool isFade;
};
