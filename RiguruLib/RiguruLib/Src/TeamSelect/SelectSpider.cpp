#include "SelectSpider.h"
#include "../Graphic/Graphic.h"

SelectSpider::SelectSpider(bool player_, bool spider_, Vector2 pos_,std::shared_ptr<SelectSpider> spiderSelect_){
	tarantula = !player;
	select = false;
	player = player_;
	spider = spider_;
	pos = pos_;
	spiderSelect = spiderSelect_;
}
SelectSpider::~SelectSpider(){

}

void SelectSpider::Update(){
	if (select){
		if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_A, true) || Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_S, true)){
			tarantula = !tarantula;
		}
	}
}

void SelectSpider::Draw(){
	if (!spider){
		if (select){
			Graphic::GetInstance().DrawTexture(TEXTURE_ID::THREAD_BACK_TEXTURE,
				pos,
				vector2(1.0f, 1.0f),
				player == true ? D3DXCOLOR(1, 0, 0,1.0f) : D3DXCOLOR(0, 0, 1,1),
				vector2(0.5f, 0.5f),
				0.0f,
				0.0f,
				1,
				1.0f);

			Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT,
				vector2(50 + (player == true ? 0 : 1920 / 2), pos.y + 55.0f),
				vector2(0.15f, 0.35f) ,
				0.6f,
				"L1",
				vector3(1,1,1),
				1,
				true);

			Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT,
				vector2(1920 - 50 + (player != true ? 0 : -1920 / 2), pos.y + 55.0f),
				vector2(0.15f, 0.35f) ,
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
			player == true ? vector3(1, 0, 0) : vector3(0, 0, 1),
			1,
			true);

		Graphic::GetInstance().DrawTexture(TEXTURE_ID::MENU_ARROW_LEFT_TEXTURE,
			vector2(50 + (player == true ? 0:1920 / 2), pos.y + 6.0f),
			vector2(1.0f, 1.0f)* (select == true ? 1.5f : 1.0f),
			D3DXCOLOR(1,1, 1, 1),
			vector2(0.5f, 0.5f),
			0.0f,
			0.0f,
			1,
			1.0f);

		Graphic::GetInstance().DrawTexture(TEXTURE_ID::MENU_ARROW_RIGHT_TEXTURE,
			vector2(1920 - 50 + (player != true ? 0 : -1920 / 2), pos.y + 6.0f),
			vector2(1.0f, 1.0f)* (select == true ? 1.5f : 1.0f),
			D3DXCOLOR(1, 1, 1, 1),
			vector2(0.5f, 0.5f),
			0.0f,
			0.0f,
			1,
			1.0f);
	}
	else{
		TEXTURE_ID tID;
		if (spiderSelect->ReturnTarantula()){
			if (player){
				tID = TEXTURE_ID::TARENTULA_RED_TEXTURE;
			}
			else{
				tID = TEXTURE_ID::TARENTULA_BLUE_TEXTURE;
			}
		}
		else{
			if (player){
				tID = TEXTURE_ID::NEPHILA_RED_TEXTURE;
			}
			else{
				tID = TEXTURE_ID::NEPHILA_BLUE_TEXTURE;
			}
		}
		Graphic::GetInstance().DrawTexture(tID,
			vector2(pos.x, pos.y),
			vector2(1.0f, 1.0f)* (select == true ? 1.5f : 1.0f),
			D3DXCOLOR(1, 1, 1, 1),
			vector2(0.5f, 0.5f),
			0.0f,
			0.0f,
			1,
			1.0f);
	}
}

void SelectSpider::SetSelect(bool flag){
	select = flag;
}


void SelectSpider::SetTarantula(bool flag){
	tarantula = flag;
}