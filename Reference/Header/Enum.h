#ifndef Enum_h__
#define Enum_h__

enum WINMODE { MODE_FULL, MODE_WIN };

namespace Engine
{
	// ID_DYNAMIC : 매 프레임마다 갱신을 해야하는 컴포넌트 집단
	// ID_STATIC : 한 번 만들면 그대로 사용할 컴포넌트 집단

	enum TRANSFORM_FLOAT { SCALEX, SCALEY, SCALEZ, POSX, POSY, POSZ, TRANSFORM_FLOAT_END };

	enum LOCATIONSTATE { STATE_ALLY, STATE_ENERMY, STATE_NEUTRALITY, STATE_ALLYHQ, STATE_ENERMYHQ, STATE_END };

	enum OBJID { OBJID_PLAYER, OBJID_DEFAULT_ENERMY, OBJID_DEFAULT_ALLY, OBJID_BDALLY, OBJID_BDENERMY, OBJID_END };

	enum AIACTION { AIACTION_OCCOPATION, AIACTION_ENERMY_IN_AREA, AIACTION_BATTLE, AIACTION_DEFENSE, AIACTION_WAIT, AIACTION_RUN, AIACTION_MOVE, AIACTION_OBJECTCOL, AIACTION_ENERMYDTECT, AIACTION_END };

	enum LOCATIONCHECK { LOCATIONCHECK_LEFT, LOCATIONCHECK_LEFTTOP, LOCATIONCHECK_RIGHT, LOCATIONCHECK_RIGHTTOP, LOCATIONCHECK_END };

	enum TANKTYPE { HUMVEE, SMALL_TANK, MIDDLE_TANK, BIG_TANK, LONG_TANK, MAX };//GBC, M3, PANZER, MAX 

	enum COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };

	enum INFO { INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };

	enum ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_END };

	enum TEXTUREID { TEX_NORMAL, TEX_CUBE, TEX_END };

	enum RENDERID { RENDER_PRIORITY, RENDER_ALPHA, RENDER_NONALPHA, RENDER_WORLD_UI, RENDER_UI, RENDER_END };

	enum LOADINGID { LOADING_STAGE, LOADING_BOSS, LOADING_END };

	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };

	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	enum SoundType {MASHIN_HIT_SOUND1, SAMLL_HIT_SOUND1, MIDDLE_HIT_SOUND1, BIG_HIT_SOUND1,
		ENEMY_MASHIN_HIT_SOUND1, ENEMY_SAMLL_HIT_SOUND1, ENEMY_MIDDLE_HIT_SOUND1, ENEMY_BIG_HIT_SOUND1,
		TANK_DEAD_SOUND1,TANK_DEAD_SOUND2,ENEMY_TANK_DEAD_SOUND1, ENEMY_TANK_DEAD_SOUND2,
		BOSS_BOMBER_SOUND1, BOSS_BOMBER_SOUND2, BOSS_BOMBER_SOUND3, BOSS_BOMBER_SOUND4, BOSS_BOMBER_SOUND5, BOSS_SIREN_SOUND,
		BOOS_APPEAR_SOUND, BOSS_SHOOT_SOUND, BOSS_ENGINE_SOUND, BOSS_RUSH1_SOUND, BOSS_RUSH2_SOUND, BOSS_BGM,
		METRONOME_SOUND, POSIN_ZOOM, RADIOS_SOUND, RADER_SOUND, UI_BUTTON_SOUND, HIT_SHELL_SUOND, VOLUEM_BUTTON, LOGO_SOUND, SELECT_MENU_BGM, STAGE_SOUND, STAGE_EFFECT,
		SELECT_SOUND, PASS_SOUND, DRONE_SOUND, AH_64A_SOUND, AH_64A_VOICE, AH_64A_VOICE2, AH_64A_VOICE3,
		SHIP_SHOOT_SOUND1, SHIP_SHOOT_SOUND2, SHIP_SHOOT_SOUND3, SHIP_SHOOT_SOUND4, SHIP_ENGINE_SOUND1, SHIP_ENGINE_SOUND2, SHIP_ENGINE_SOUND3, SHIP_ENGINE_SOUND4, SHIP_ENGINE_SOUND5,
		PLAYER_MASHIN_HIT_SOUND, PLAYER_SMALL_HIT_SOUND, PLAYER_MIDDLE_HIT_SOUND, PLAYER_BIG_HIT_SOUND,
		PLAYER_SHOT_M3_FIRE, AIRPLANE_SOUND, PLAYER_ENGINE_BGM, PLAYER_MOVE_BGM, AIMCHANGE_SOUND, CAMERA_CHANGE_SOUND,
		PLAYER_MOVE_SOUND1, PLAYER_MOVE_SOUND2, PLAYER_SHOT_SOUND1, PLAYER_SHOT_SOUND2, PLAYER_BULLET_EXIT_SOUND1, BOOM1,
		POPUP_SOUND, PAPER_SOUND, KILL_LOG_SOUND
		, AI_SHOT_SOUND0 = 50, AI_SHOT_SOUND1, AI_SHOT_SOUND2, AI_SHOT_SOUND3, AI_SHOT_SOUND4, AI_SHOT_SOUND5, AI_SHOT_SOUND6, AI_SHOT_SOUND7, AI_SHOT_SOUND8, AI_SHOT_SOUND9
		, AI_SHOT_SOUND10, AI_SHOT_SOUND11, AI_SHOT_SOUND12, AI_SHOT_SOUND13, AI_SHOT_SOUND14, AI_SHOT_SOUND15, AI_SHOT_SOUND16, AI_SHOT_SOUND17, AI_SHOT_SOUND18, AI_SHOT_SOUND19
		, SOUND_END
	};

	//HOLD : 키 누르고 있는 시점(전 프레임에도 눌렀고 지금도 눌렀다)
	// TAP : 키 처음 한번 누른 시점(전 프레임에는 안눌렀고 지금 눌렀다)
	// AWAY : 키 눌렀다가 뗀 시점(전프레임 눌렀는데 지금 안눌렀다)
	// NONE : 아무것도 안한 상태(전프레임에도 안눌렀고 이번프레임에도 안눌렀다)
	enum KEY_STATE { HOLD, TAP, AWAY, NONE };

	enum BULLET_ID {
		ENEMY_MASHINE_BULLET, ENEMY_CANNONBALL, ENEMY_SMALL_CANNONBALL, ENEMY_MIDDLE_CANNONBALL, ENEMY_BIG_CANNONBALL,
		MASHINE_BULLET, CANNONBALL, SMALL_CANNONBALL,MIDDLE_CANNONBALL,BIG_CANNONBALL, AH_64A_BULLET,
		BOOM_BULLET, SMOKE_BULLET, MASHINE_BULLET_RELOAD, CANNONBALL_RELOAD, SHIP_BULLET, SHIP_REAL_BULLET, BOSS_BULLET, BULLET_END };

	enum EFFECT_ID { AH_64A_EFFECT, SHIP_SMOKE_EFFECT, TANK_SHOOT_SMOKE, BOOMER_BOOM_SMOKE, BATTLESHIP_FIRE_SMOKE, HUMVEE_SHOOT_EFFECT, BATTLEAHIP_POSIN_SMOKE, EFFECT_END };

	enum CAMERA_ID { AH_64A_END_CAMERA , AH_64A_CAMERA, AH_64A_AIMCAMERA, MOONWALK_CAMERA, BOOM_CAMERA, DRONE_CAMERA, SIMPLE_CAMERA, DYNAMIC_CAMERA, TANK_CAMERA, TOPVIEW_CAMERA, AIM_CAMERA, CAMERA_END };

	enum class VEHICLE
	{
		HUMVEE, SMALL_TANK, MIDDLE_TANK, BIG_TANK, LONG_TANK, MAX //GBC, M3, PANZER, MAX 
	};

	enum OBB_ID { x, y, z, OBB_END };

	enum GOD_COLOR { RED, ORANGE, YELLOW, GREEN, BLUE, DEEP_BLUE, PURPLE, GOD_COLOR_END };
}

#endif // Enum_h__
