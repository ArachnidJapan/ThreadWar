#include "SelectSpider.h"
#include "../Graphic/Graphic.h"

SelectSpider::SelectSpider(bool red_, bool spider_, Vector2 pos_, int playerNum_, std::shared_ptr<SelectSpider> spiderSelect_){
	tarantula = !red_;
	select = false;
	red = red_;
	enemyPlayerHave = !red;
	spider = spider_;
	pos = pos_;
	spiderSelect = spiderSelect_;
	playerNum = playerNum_;

	int notPlayer = -1;
	if (red){
		spp[0] = SelectPlayerParam::P1;
		notPlayer = 0;
	}
	else
	spp[3] = SelectPlayerParam::CP4;
	spp[1 + notPlayer] = SelectPlayerParam::CP1;
	spp[2 + notPlayer] = SelectPlayerParam::CP2;
	spp[3 + notPlayer] = SelectPlayerParam::CP3;

	sppString[SelectPlayerParam::P1] = "P1";
	sppString[SelectPlayerParam::CP1] = "CP1";
	sppString[SelectPlayerParam::CP2] = "CP2";
	sppString[SelectPlayerParam::CP3] = "CP3";
	sppString[SelectPlayerParam::CP4] = "CP4";
	sppString[SelectPlayerParam::NONE] = "NONE";
}
SelectSpider::~SelectSpider(){

}

void SelectSpider::Update(){
	if (select){
		if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_A, true) ||
			Device::GetInstance().GetInput()->GamePadButtonDown(0, GAMEPADKEY::BUTTON_L1, true)){
			if (!spider)
				tarantula = !tarantula;
			else{
				spiderSelect->SetPlayerParam(playerNum, false);
			}
		}
		if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_S, true) ||
			Device::GetInstance().GetInput()->GamePadButtonDown(0, GAMEPADKEY::BUTTON_R1, true)){
			if (!spider)
				tarantula = !tarantula;
			else{
				spiderSelect->SetPlayerParam(playerNum, true);
			}
		}
	}
}

void SelectSpider::SetPlayerParam(int playerNum, bool plus){
	if (plus){
		SelectPlayerParam nextParam = (SelectPlayerParam)(spp[playerNum] + 1);
		if (enemyPlayerHave){
			if (nextParam == SelectPlayerParam::NONE){
				spp[playerNum] = nextParam;
				return;
			}

			if (spp[playerNum] == SelectPlayerParam::NONE){
				nextParam = SelectPlayerParam::CP1;
				bool finish = false;
				do{
					bool same = false;
					for (auto i : spp){
						if (i.second == nextParam){
							same = true;
							break;
						}
					}
					if (!same){
						spp[playerNum] = nextParam;
						finish = true;
						break;
					}
					nextParam = (SelectPlayerParam)(nextParam + 1);
					if (nextParam == SelectPlayerParam::NONE){
						spp[playerNum] = nextParam;
						finish = true;
					}
				} while (!finish);
			}
			else{
				bool finish = false;
				do{
					bool same = false;
					for (auto i : spp){
						if (i.second == nextParam){
							same = true;
							break;
						}
					}
					if (!same){
						spp[playerNum] = nextParam;
						finish = true;
						break;
					}
					nextParam = (SelectPlayerParam)(nextParam + 1);
					if (nextParam == SelectPlayerParam::NONE){
						spp[playerNum] = nextParam;
						finish = true;
					}
				} while (!finish);
			}
		}
		else{
			if (nextParam == SelectPlayerParam::NONE){
				spp[playerNum] = SelectPlayerParam::NONE;
				return;
			}
			if (spp[playerNum] == SelectPlayerParam::NONE){
				nextParam = SelectPlayerParam::P1;

				bool finish = false;
				do{
					bool same = false;
					for (auto i : spp){
						if (i.second == nextParam){
							same = true;
							break;
						}
					}
					if (!same){
						spp[playerNum] = nextParam;
						finish = true;
						break;
					}
					nextParam = (SelectPlayerParam)(nextParam + 1);
					if (nextParam == SelectPlayerParam::NONE){
						spp[playerNum] = SelectPlayerParam::NONE;
						finish = true;
					}
				} while (!finish);
			}
			else{
				bool finish = false;
				do{
					bool same = false;
					for (auto i : spp){
						if (i.second == nextParam){
							same = true;
							break;
						}
					}
					if (!same){
						spp[playerNum] = nextParam;
						finish = true;
						break;
					}
					nextParam = (SelectPlayerParam)(nextParam + 1);
					if (nextParam == SelectPlayerParam::NONE){
						spp[playerNum] = SelectPlayerParam::NONE;
						finish = true;
					}
				} while (!finish);
			}
		}
	}
	else{
		SelectPlayerParam nextParam = (SelectPlayerParam)(spp[playerNum] - 1);
		if (enemyPlayerHave){
			if (nextParam == SelectPlayerParam::P1){
				spp[playerNum] = SelectPlayerParam::NONE;
				return;
			}
			bool finish = false;
			do{
				bool same = false;
				for (auto i : spp){
					if (i.second == nextParam){
						same = true;
						break;
					}
				}
				if (!same){
					spp[playerNum] = nextParam;
					finish = true;
					break;
				}
				nextParam = (SelectPlayerParam)(nextParam - 1);
				if (nextParam == SelectPlayerParam::P1){
					spp[playerNum] = SelectPlayerParam::NONE;
					finish = true;
				}
			} while (!finish);
		}
		else{
			//if (nextParam == SelectPlayerParam::CP4)nextParam = SelectPlayerParam::CP3;
			if (nextParam == SelectPlayerParam::P0){
				spp[playerNum] = SelectPlayerParam::NONE;
				return;
			}
			bool finish = false;
			do{
				bool same = false;
				for (auto i : spp){
					if (i.second == nextParam){
						same = true;
						break;
					}
				}
				if (!same){
					spp[playerNum] = nextParam;
					finish = true;
					break;
				}
				nextParam = (SelectPlayerParam)(nextParam - 1);
				if (nextParam == SelectPlayerParam::P0){
					spp[playerNum] = SelectPlayerParam::NONE;
					finish = true;
				}
			} while (!finish);
		}
	}
}

bool SelectSpider::PlayerHave(){
	for (auto i : spp){
		if (i.second == SelectPlayerParam::P1)return true;
	}

	return false;
}

void SelectSpider::Draw(){
	if (!spider){
		SpiderSelect();
	}
	else{
		PlayerSelect();
	}
}

void SelectSpider::SpiderSelect(){
	if (select){
		Graphic::GetInstance().DrawTexture(TEXTURE_ID::THREAD_BACK_TEXTURE,
			pos,
			vector2(1.0f, 1.0f),
			red == true ? D3DXCOLOR(1, 0, 0, 1.0f) : D3DXCOLOR(0, 0, 1, 1),
			vector2(0.5f, 0.5f),
			0.0f,
			0.0f,
			1,
			1.0f);

		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT,
			vector2(50 + (red == true ? 0 : 1920 / 2), pos.y + 55.0f),
			vector2(0.2f, 0.35f),
			0.6f,
			"L1",
			vector3(1, 1, 1),
			1,
			true);

		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT,
			vector2(1920 - 35 + (red != true ? 0 : -1920 / 2), pos.y + 55.0f),
			vector2(0.2f, 0.35f),
			0.6f,
			"R1",
			vector3(1, 1, 1),
			1,
			true);
	}
	Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT,
		pos,
		vector2(0.3f, 0.5f) * (select == true ? 1.3f : 1.0f),
		0.6f,
		"SELECT YOUR SPIDER",
		red == true ? vector3(1, 0, 0) : vector3(0, 0, 1),
		1,
		true);

	Graphic::GetInstance().DrawTexture(TEXTURE_ID::MENU_ARROW_LEFT_TEXTURE,
		vector2(50 + (red == true ? 0 : 1920 / 2), pos.y + 6.0f),
		vector2(1.0f, 1.0f)* (select == true ? 1.5f : 1.0f),
		D3DXCOLOR(1, 1, 1, 1),
		vector2(0.5f, 0.5f),
		0.0f,
		0.0f,
		1,
		1.0f);

	Graphic::GetInstance().DrawTexture(TEXTURE_ID::MENU_ARROW_RIGHT_TEXTURE,
		vector2(1920 - 50 + (red != true ? 0 : -1920 / 2), pos.y + 6.0f),
		vector2(1.0f, 1.0f)* (select == true ? 1.5f : 1.0f),
		D3DXCOLOR(1, 1, 1, 1),
		vector2(0.5f, 0.5f),
		0.0f,
		0.0f,
		1,
		1.0f);
}

void SelectSpider::PlayerSelect(){
	if (select){
		Graphic::GetInstance().DrawTexture(TEXTURE_ID::THREAD_BACK_TEXTURE,
			vector2(pos.x, pos.y - 40.0f),
			vector2(0.6f, 0.6f),
			red == true ? D3DXCOLOR(1, 0, 0, 1.0f) : D3DXCOLOR(0, 0, 1, 1),
			vector2(0.5f, 0.5f),
			0.0f,
			0.0f,
			1,
			1.0f);

		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT,
			vector2(pos.x - 245.0f, pos.y + 50.0f),
			vector2(0.20f, 0.35f),
			0.6f,
			"L1",
			vector3(1, 1, 1),
			1,
			true);

		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT,
			vector2(pos.x + 260.0f, pos.y + 50.0f),
			vector2(0.20f, 0.35f),
			0.6f,
			"R1",
			vector3(1, 1, 1),
			1,
			true);
	}
	Graphic::GetInstance().DrawTexture(TEXTURE_ID::MENU_ARROW_LEFT_TEXTURE,
		vector2(pos.x - 250.0f, pos.y),
		vector2(1.0f, 1.0f)* (select == true ? 1.5f : 1.0f),
		D3DXCOLOR(1, 1, 1, 1),
		vector2(0.5f, 0.5f),
		0.0f,
		0.0f,
		1,
		1.0f);

	Graphic::GetInstance().DrawTexture(TEXTURE_ID::MENU_ARROW_RIGHT_TEXTURE,
		vector2(pos.x + 250.0f, pos.y),
		vector2(1.0f, 1.0f)* (select == true ? 1.5f : 1.0f),
		D3DXCOLOR(1, 1, 1, 1),
		vector2(0.5f, 0.5f),
		0.0f,
		0.0f,
		1,
		1.0f);
	TEXTURE_ID tID;
	TEXTURE_ID whitetID;
	if (spiderSelect->ReturnTarantula()){
		if (red){
			tID = TEXTURE_ID::TARENTULA_RED_TEXTURE;
			whitetID = TEXTURE_ID::TARENTULA_WHITE_R_TEXTURE;
		}
		else{
			tID = TEXTURE_ID::TARENTULA_BLUE_TEXTURE;
			whitetID = TEXTURE_ID::TARENTULA_WHITE_L_TEXTURE;
		}
	}
	else{
		if (red){
			tID = TEXTURE_ID::NEPHILA_RED_TEXTURE;
			whitetID = TEXTURE_ID::NEPHILA_WHITE_R_TEXTURE;
		}
		else{
			tID = TEXTURE_ID::NEPHILA_BLUE_TEXTURE;
			whitetID = TEXTURE_ID::NEPHILA_WHITE_L_TEXTURE;
		}
	}
	Graphic::GetInstance().DrawTexture(whitetID,
		vector2(pos.x, pos.y),
		vector2(1.0f, 1.0f) * (select == true ? 1.5f : 1.0f),
		spiderSelect->ReturnMyPlayerParam(playerNum) == SelectPlayerParam::P1 ?
		red ?
		D3DXCOLOR(1, 0, 0, 1) : D3DXCOLOR(0, 0, 1, 1) :
		spiderSelect->ReturnMyPlayerParam(playerNum) == SelectPlayerParam::NONE ?
		D3DXCOLOR(1, 1, 1, 1) : D3DXCOLOR(0, 1, 0, 1),
		vector2(0.5f, 0.5f),
		0.0f,
		0.0f,
		1,
		1.0f);

	Graphic::GetInstance().DrawTexture(tID,
		vector2(pos.x, pos.y),
		vector2(1.0f, 1.0f)* (select == true ? 1.5f : 1.0f),
		D3DXCOLOR(1, 1, 1, 1),
		vector2(0.5f, 0.5f),
		0.0f,
		0.0f,
		1,
		1.0f);

	Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT,
		pos,
		vector2(0.40f, 0.7f),
		0.6f,
		sppString[spiderSelect->ReturnMyPlayerParam(playerNum)],
		spiderSelect->ReturnMyPlayerParam(playerNum) == SelectPlayerParam::P1 ?
		red ?
		vector3(1, 0, 0) : vector3(0, 0, 1) :
		spiderSelect->ReturnMyPlayerParam(playerNum) == SelectPlayerParam::NONE ?
		vector3(1, 1, 1) : vector3(0, 1, 0),
		1,
		true);
}

void SelectSpider::SetSelect(bool flag){
	select = flag;
}

int SelectSpider::CPUCount(){
	int count = 0;
	for (auto i : spp){
		if (i.second != SelectPlayerParam::P0 && i.second != SelectPlayerParam::P1 && i.second != SelectPlayerParam::NONE){
			count++;
		}
	}

	return count;
}

void SelectSpider::SetTarantula(bool flag){
	tarantula = flag;
}