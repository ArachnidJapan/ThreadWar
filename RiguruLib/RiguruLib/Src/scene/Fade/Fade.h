#ifndef FADE_H
#define FADE_H

// フェードクラス
class Fade
{
public:
	// コンストラクタ
	Fade();

	// 起動
	void start(bool _isFadeIn, float _time);

	// 更新
	void update(float deltaTime);

	// 描画
	void draw() const;

	// 動作中か？
	bool getIsUpdate();

private:
	// フェードにかける時間
	float time;
	
	// 現在の経過時間
	float currentTime;

	// フェードインか？
	bool isFadeIn;

	// 動作中か？
	bool isupdate;
};

#endif