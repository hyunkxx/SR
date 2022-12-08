#ifndef Enum_h__
#define Enum_h__

enum WINMODE { MODE_FULL, MODE_WIN };

namespace Engine
{
	// ID_DYNAMIC : 매 프레임마다 갱신을 해야하는 컴포넌트 집단
	// ID_STATIC : 한 번 만들면 그대로 사용할 컴포넌트 집단

	enum LOCATIONSTATE { STATE_ALLY, STATE_ENERMY, STATE_NEUTRALITY, STATE_ALLYHQ, STATE_ENERMYHQ, STATE_END };

	enum OBJID { OBJID_PLAYER, OBJID_DEFAULT_ENERMY, OBJID_DEFAULT_ALLY, OBJID_BDALLY, OBJID_BDENERMY, OBJID_END };

	enum AIACTION { AIACTION_OCCOPATION, AIACTION_ENERMY_IN_AREA, AIACTION_BATTLE, AIACTION_DEFENSE, AIACTION_WAIT, AIACTION_END };

	enum LOCATIONCHECK { LOCATIONCHECK_LEFT, LOCATIONCHECK_LEFTTOP, LOCATIONCHECK_RIGHT, LOCATIONCHECK_RIGHTTOP, LOCATIONCHECK_END };

	enum  TANKTYPE { TANK1, TANK2, TANK3, GBC, M3, HUMVEE, PANZER, TANKTYPE_END };

	enum COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };

	enum INFO { INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };

	enum ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_END };

	enum TEXTUREID { TEX_NORMAL, TEX_CUBE, TEX_END };

	enum RENDERID { RENDER_PRIORITY, RENDER_ALPHA, RENDER_NONALPHA, RENDER_WORLD_UI, RENDER_UI, RENDER_END };

	enum LOADINGID { LOADING_STAGE, LOADING_BOSS, LOADING_END };

	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };

	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	enum SoundType {
		LOGO_SOUND, SELECT_MENU_BGM, STAGE_SOUND, 
		SELECT_SOUND, PASS_SOUND,
		PLAYER_SHOT_M3_FIRE, PLAYER_MOVE_SOUND1, PLAYER_SHOT_SOUND1, BOOM1, SOUND_END
	};

	//HOLD : 키 누르고 있는 시점(전 프레임에도 눌렀고 지금도 눌렀다)
	// TAP : 키 처음 한번 누른 시점(전 프레임에는 안눌렀고 지금 눌렀다)
	// AWAY : 키 눌렀다가 뗀 시점(전프레임 눌렀는데 지금 안눌렀다)
	// NONE : 아무것도 안한 상태(전프레임에도 안눌렀고 이번프레임에도 안눌렀다)
	enum KEY_STATE { HOLD, TAP, AWAY, NONE };

	enum BULLET_ID { MASHINE_BULLET, CANNONBALL, MASHINE_BULLET_RELOAD, CANNONBALL_RELOAD, BULLET_END };

	enum class VEHICLE
	{
		HUMVEE, SMALL_TANK, MIDDLE_TANK, BIG_TANK, LONG_TANK, MAX //GBC, M3, PANZER, MAX 
	};

	enum OBB_ID { x, y, z, OBB_END };
}

#endif // Enum_h__
