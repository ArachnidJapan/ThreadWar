#pragma once
#include "Sound.h"
#include "../Actor/ID.h"
#include <map>
class Audio{
public:
	Audio();
	~Audio();
	static Audio &GetInstance(){
		static Audio d;
		return d;
	}
	//‚a‚f‚l‚ğƒ[ƒh
	void LoadBGM(BGM_ID id, TCHAR* FileName);
	//‚r‚d‚ğƒ[ƒh(bufferCount “¯Ä¶‚µ‚½‚¢”)
	void LoadSE(SE_ID id, TCHAR* FileName, int bufferCount);

	//‚a‚f‚lÄ¶
	void PlayBGM(BGM_ID id, bool loop = false);
	//‚r‚dÄ¶
	void PlaySE(SE_ID id, bool loop = false);

	//‚a‚f‚l’â~
	void StopBGM(BGM_ID id);
	//‚r‚d’â~
	void StopSE(SE_ID id);

	void SetPlaySpeedBGM(BGM_ID id, float speed);

	void SetPlaySpeedSE(SE_ID id, float speed);

	//‚a‚f‚lƒ{ƒŠƒ…[ƒ€ƒZƒbƒg
	void SetAllBGMVolume(int volume);
	//‚r‚dƒ{ƒŠƒ…[ƒ€ƒZƒbƒg
	void SetAllSEVolume(int volume);

	//‚a‚f‚lƒ{ƒŠƒ…[ƒ€ƒZƒbƒg
	void SetBGMVolume(BGM_ID id,int volume);
	//‚r‚dƒ{ƒŠƒ…[ƒ€ƒZƒbƒg
	void SetSEVolume(SE_ID id,int volume);

private:
	std::map<BGM_ID, Sound> bgm;
	std::map<SE_ID, Sound> se;
	std::vector<BGM_ID> bgmId;
	std::vector<SE_ID> seId;
};