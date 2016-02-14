#pragma once

enum TEXTURE_ID
{
	WOOD_TEXTURE,
	FLOOR_TEXTURE,
	MOUSE_TEXTURE,
	CURSOR_TEXTURE,
	CURSOR2_TEXTURE,
	ARROW_TEXTURE,
	POINT_TEXTURE,
	GAUGE_TEXTURE,
	GAUGE_CENTER_TEXTURE,
	DAMAGE_TEXTURE,
	//TitleScene
	TITLE_BACK_TEXTURE,
	MENU_BLACK_TEXTURE,
	//DemoScene
	TITLE_LOGO_TEXTURE,
	PRESS_START_TEXTURE,
	BLACK_TEXTURE,
	//Option
	MANUAL_TEXTURE,
	LINE_WHITE_TEXTURE,
	LINE_BLACK_TEXTURE,
	//teamSelect
	NEPHILA_WHITE_TEXTURE,
	NEPHILA_RED_TEXTURE,
	NEPHILA_BLUE_TEXTURE,
	TARENTULA_WHITE_TEXTURE,
	TARENTULA_RED_TEXTURE,
	TARENTULA_BLUE_TEXTURE,
	TEAM_TEXTURE,
	THREAD_BACK_TEXTURE,
	WHITE_BAR_TEXTURE,
	MENU_ARROW_LEFT_TEXTURE,
	MENU_ARROW_RIGHT_TEXTURE,
	//ResultScene
	WHITE_TEXTURE,
};

enum FONT_ID{
	TEST_FONT
};

enum MODEL_ID{
	NEPHILA_MODEL,
	NEPHILA2_MODEL,
	TARENTULE_MODEL,
	TARENTULE2_MODEL,
	CUBE_MODEL,
	SPHERE_MODEL,
	STAGE_MODEL,
	THREAD_EFFECT_MODEL,
	CRYSTAL_CENTER_MODEL,
	CRYSTAL_PLAYERSIDE_MODEL,
	CRYSTAL_ENEMYSIDE_MODEL,
};

enum OCT_ID{
	NULL_OCT,
	STAGE_OCT,
	ENEMY_OCT,
};

enum BGM_ID{
	TITLE_BGM,
	GAME_BGM,
};

enum SE_ID{
	CRYSTALGET_SE,
	CRYSTALNORMAL_SE,
	DANGLE_SE,
	JUMP_SE,
	ROUND_SE,
	STEP_SE,
	THREAD_SHOT_SE,
	WALK_SE,
	WIND_SE,
	BACK_SE,
	ENTER_SE,
	RESTRAINT_SE,
	SWITCH_SE,
	LANDING_SE,
	ANNBIENNT_SE,
};

enum ANIM_ID{
	NULL_ANIM,
	NEPHILA_WAIT_ANIM,
	NEPHILA_WALKFRONT_ANIM,
	NEPHILA_WAITLEFT_ANIM,
	NEPHILA_WALKBACK_ANIM,
	NEPHILA_WALKRIGHT_ANIM,
	NEPHILA_JUMP_ANIM,
	NEPHILA_TURNLEFT_ANIM,
	NEPHILA_STEPFRONT_ANIM,
	NEPHILA_STEPBACK_ANIM,
	NEPHILA_STEPLEFT_ANIM,
	NEPHILA_STEPRIGHT_ANIM,
	NEPHILA_ROUND_ANIM,
	NEPHILA_THREADFRONT_ANIM,
	NEPHILA_THREADLEFT_ANIM,
	NEPHILA_REWIND_ANIM,
	NEPHILA_DEAD_ANIM,
	CENTER,
	TARENTULE_WAIT_ANIM,
	TARENTULE_WALKFRONT_ANIM,
	TARENTULE_WAITLEFT_ANIM,
	TARENTULE_WALKBACK_ANIM,
	TARENTULE_WALKRIGHT_ANIM,
	TARENTULE_JUMP_ANIM,
	TARENTULE_TURNLEFT_ANIM,
	TARENTULE_STEPFRONT_ANIM,
	TARENTULE_STEPBACK_ANIM,
	TARENTULE_STEPLEFT_ANIM,
	TARENTULE_STEPRIGHT_ANIM,
	TARENTULE_ROUND_ANIM,
	TARENTULE_THREADFRONT_ANIM,
	TARENTULE_THREADLEFT_ANIM,
	TARENTULE_REWIND_ANIM,
	TARENTULE_DEAD_ANIM,
};

enum SHADER_ID{
	PLAYER_SHADER,
	STAGE_SHADER,
	TEXTURE_SHADER,
	SPHERE_SHADER,
	FONT_SHADER,
	LINE_SHADER,
	CUBE_SHADER,
	CRYSTAL_SHADER,
	THREAD_EFFECT_SHADER,
};

enum ACTOR_ID{
	BEGIN_ACTOR,
	STAGE_ACTOR,
	ENEMY_THREADBULLET_ACTOR,
	PLAYER_THREADBULLET_ACTOR,
	PLAYER_THREAD_ACTOR,
	PLAYER_THREADWEB_ACTOR,
	ENEMY_THREAD_ACTOR,
	ENEMY_THREADWEB_ACTOR,
	PLAYER_ACTOR,
	ENEMY_ACTOR,
	CRYSTAL_CENTER_ACTOR,
	CRYSTAL_PLAYERSIDE_ACTOR,
	CRYSTAL_ENEMYSIDE_ACTOR,
	BASE_ACTOR,
	THREAD_EFFECT_ACTOR,
	UI_ACTOR,
	END_ACTOR,
};

enum COL_ID{
	SPHERE_SPHERE_COLL,
	THREAD_PLAYER_COLL,
	RAY_MODEL_COLL,
	CAPSULE_MODEL_COLL,
	RAY_MODEL_NATURAL_COLL,
	RAY_MODEL_STEP_COLL,
	SPHERE_MODEL_COLL,
	SPHERE_MODEL_NATURAL_COLL,
	SPHERE_MODEL_THREADWEB_COLL,
	SPHERE_MODEL_ONTHREAD_COLL,
	OBB_OBB_COLL,
	SPHERE_MODEL_STEP,
	SPHERE_LINE_COLL,
	LINE_SPHERE_COLL,
	SPHERE_LINE_END_COLL,
	SPHERE_LINE_JUMP_COLL,
	TRIANGLE_LINE_COLL,
	TRIANGLE_SPHERE_COLL,
	TRIANGLE_LINE_T_COLL,
	TRIANGLE_LINE_JUMP_COLL,
	SPHERE_MODEL_MOVE_COLL,
	RAY_MODEL_ONTHREAD_COLL,
	PLAYER_ENEMY_COLL,//プレイヤーから敵
	ENEMY_PLAYER_COLL,//敵からプレイヤー
};

enum CAMERA_ID{
	//PLAYER_CAMERA,
	PLAYER_CAMERA_1P,
	PLAYER_CAMERA_2P,
	PLAYER_CAMERA_3P,
	PLAYER_CAMERA_4P,
	//ENEMY_CAMERA,
	ENEMY_CAMERA_5P,
	ENEMY_CAMERA_6P,
	ENEMY_CAMERA_7P,
	ENEMY_CAMERA_8P,
	GOD_CAMERA,
};

enum ACTION_ID{
	NULL_ACTION,
	NORMAL_ACTION,
	JUMP_ACTION,
	HOVER_ACTION,
	STEP_ACTION,
	CURL_ACTION,
	GROUND_CURL_ACTION,
	AIR_CURL_ACTION,
	JUMP_CURL_ACTION,
	THREAD_ACTION,
	THREAD_WEB_ACTION,
	THREAD_STEP_ACTION,
	DANGLE_ACTION,
	REWIND_ACTION,
};

enum VICTORY_ID{
	PLAYER_WIN,
	ENEMY_WIN,
	DRAW_WIN,
};