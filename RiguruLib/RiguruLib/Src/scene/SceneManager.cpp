#define NOMINMAX
#include"SceneManager.h"
#include"IScene.h"
#include <algorithm>

const int SceneManager::MaxStageCount = 6;

//コンストラクタ
SceneManager::SceneManager() :
isFade(false),
mStageCount(1){
	nextResult = false;
}

//更新前初期化
void SceneManager::Initialize(){
	End();
	mScenes.clear();
}

//更新
void SceneManager::Update(float frameTime){
	if (!fade.getIsUpdate())
	{
		mCurrentScene->Update(frameTime);
		if (mCurrentScene->IsEnd() && !isFade)
		{
			isFade = true;
			fadestart(false, 1);
		}
	}
	else if (mCurrentScene == mScenes[Scene::GamePlay] || mCurrentScene == mScenes[Scene::Demo]){
		mCurrentScene->Update(frameTime);
	}
	// フェードの更新
	fade.update(nextResult ? frameTime / 4.0f: frameTime);
}

//描画
void SceneManager::Draw() const{
	mCurrentScene->Draw();
	fade.draw();
}

//終了
void SceneManager::End(){
	mCurrentScene->End();
}

void SceneManager::Change()
{
	if (mCurrentScene->Next() == Scene::Ending){
		nextResult = true;
	}
	else{
		nextResult = false;
	}
	if (mCurrentScene->IsEnd())
	{
		if (isFade&&!fade.getIsUpdate())
		{
			isFade = false;
			Change(mCurrentScene->Next());
		}
		//Change(mCurrentScene->Next());
	}
}

//シーンの追加
void SceneManager::Add(Scene name, const IScenePtr& scene){
	mScenes[name] = scene;
}

void SceneManager::SetScene(Scene name){
	mCurrentScene = mScenes[name];
	mCurrentScene->Initialize();
}

//シーンの変更
void SceneManager::Change(Scene name){
	Scene now = Scene::None;
	for (auto& i : mScenes)
	{
		if (mCurrentScene == i.second)
			now = i.first;
	}
	if (name == now)
		return;

	End();
	mCurrentScene = mScenes[name];
	mCurrentScene->Initialize();
	fadestart(true, 1);
}

// 初期化を指定する
void SceneManager::Init(Scene name)
{
	mScenes.at(name)->Initialize();
}

// 終了処理を指定する
void SceneManager::Final(Scene name)
{
	mScenes.at(name)->End();
}

static int _Clamp(int t, int min, int max)
{
	return std::min(max, std::max(t, min));
}

void SceneManager::SetStageCount(int n)
{
	mStageCount = _Clamp(n, 0, SceneManager::MaxStageCount);
}

// フェード開始
void SceneManager::fadestart(bool isFadeIn, float time)
{
	fade.start(isFadeIn, time);
}

// フェードしているか？
bool SceneManager::isFadeUpdate()
{
	return fade.getIsUpdate();
}