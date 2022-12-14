#pragma once
#include "GameObject.h"
#include "Collisionable.h"
BEGIN(Engine)

class CTransform;
class CRcTex;
class CTexture;
class CCalculator;
class CLocation;
class CTank_Body;
class CTank_Head;
class CTankPosin;
class CCalculator;
class CVoxel;
END
class CBottomDirEnermy :public CGameObject, public ICollisionable
{

private:
	explicit CBottomDirEnermy(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBottomDirEnermy(const CBottomDirEnermy& rhs);
	virtual ~CBottomDirEnermy();

public:
	virtual HRESULT Ready_Object(void) override;
	HRESULT Ready_Object(void* pArg);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;


	virtual const   _vec3		Get_Info(void)			override;
	virtual			void		Move_Info(_vec3 _Info)	override;
	virtual			void		OBB_Collision_EX(void)	override;
	virtual			void		Update_OBB(void)		override;
	virtual			OBB*        Get_OBB(void)			override;
public:


	//로케이션에 줘야하는 함수 시간나면 이넘값으로 수정
	void	StateCheck();
	_bool Get_RightLocation() { return m_bRightLocationCount; }
	_bool Get_LeftLocation() { return m_bLeftLocationCount; }
	_bool Get_RightTopLocation() { return m_bRightTopLocationCount; }
	_bool Get_LeftTopLocation() { return m_bLeftTopLocationCount; }


	void  Set_SuccessOccupation(_int RecentSO) { m_iSuccessOccupation = RecentSO; }
	_int  Get_LocationCheck() { return m_iLocationCheck; }
	void  Set_LocationCheck(_int _Check) { m_iLocationCheck = _Check; }
	void  Set_Action(_int _action) { m_iAction = _action; }
	void  Set_PastLocation(_int _Past) { m_PastLocation = _Past; }
	void  Set_DisCountLocation();
	float  Get_Hp() { return fCurHp; };
	void Minus_HP_UI(_float HP_minus) { fCurHp -= HP_minus; }
	void	Plus_HP_UI(_float HP_plus) { fCurHp += HP_plus; }
	_float GetHp(void) { return fCurHp; }
	void    Set_Count(_int _Count) { m_iCount += _Count; }
	void Run(_float fTimeDelta);
	_bool Get_CreateCheck(void) { return CreateCheck; }
	void Dead_Motion(const _float & fTimeDelta); 
	void Set_CreateCheck(_bool _true) { CreateCheck = _true; }
public:
	//에너미에서 사용하는 함수
	void Basic(_float fTimeDelta);
	void Occupation(_float fTimeDelta);
	void Wait(_float fTimeDelta);
	void Enermy_In_Area(_float fTimeDelta);
	void Detect(_float fTimeDelta);
	_bool  Left_RightCheck(_vec3 _vDir, _vec3 _vLook);
	_float Dist(CTransform* _Target);
	void		ObjectCol(_bool m_Left);
	void		ColObject(_float fTimeDelta);
	void  Set_DeadMotionPlay() { Deadtest = true; }
	TANKTYPE Get_Type() { return TTYPE; }
private:
	HRESULT		Add_Component(void);

private:
	CVoxel*				m_pBody = nullptr;
	CVoxel*				m_pHead = nullptr;
	CVoxel*				m_pPosin = nullptr;
	//탱크형태
	CTank_Body*			m_pBodyBuffer = nullptr;
	CTank_Head*			m_pHeadBuffer = nullptr;
	//탱크 움직임
	CTransform*			m_pTransformHead = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTransform*			m_pTransformPosin = nullptr;

	EData*				m_EData;
	CCalculator*		m_pCalculatorCom = nullptr;
private:
	BULLET_ID Temp;
	_int  m_iLocationState;
	_int  m_iAction = AIACTION::AIACTION_END;
	_bool m_bRightLocationCount = false
		, m_bRightTopLocationCount = false
		, m_bLeftLocationCount = false
		, m_bLeftTopLocationCount = false;
	_int m_iSuccessOccupation = LOCATIONCHECK::LOCATIONCHECK_END;
	_int m_iLocationCheck = false;
	_int m_PastLocation;
	_vec3   m_vPatrol = {};
	_bool LeftCheck = false;
	_int Targeted = 0;
	_vec3 vPatrolRange = {};
	_bool m_bPatrol = false;
	_float  m_fPreHp = 0.f;
	_bool ColBuild = false;
	_int  ColBuildCount = 0;
	_int m_iCount = 0;
	_float     Range = 0.f;
	_float PreDist = 0.f;
	_bool m_bOcne = false;
	_float PreHp = 0.f;
	_float RuleTime = 0.f;
	_bool m_bTest;
	_bool bLeft;
	_float re = 0.f;
	_bool CreateCheck = false;
	_bool DeadMotionCheck = false;
	_int m_iMotionCount = 0;
	_float fAccum = 0.f;
	_bool   Deadtest = false;
	TANKTYPE TTYPE = MAX;
	//탱크 정보
	_float m_fMaxHp, fCurHp, fAccel_top_speed, RotSpped, fPosinDist;
	_float m_fReloadTime, m_fReload;
	_int   m_iCannonSpeed, TempBullet;
public:
	static CBottomDirEnermy*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CBottomDirEnermy*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg);

private:
	virtual void Free(void) override;

	// HP UI
	_matrix				UI_matViewF, m_UI_ProjMatrix;
	CRcTex*				m_pRcTexF = nullptr;
	CTexture*			m_pTextureF = nullptr;
	CTransform*		m_pTransformHP_UI = nullptr;

	_float					UI_fScaleX, UI_fScaleY, UI_fScaleZ;
	_float					UI_fOrgin_ScaleX;


	// Minimap UI
	_matrix				UI_Minimap_matProj;
	CRcTex*				m_pMinimap_RcTex = nullptr;
	CTexture*			m_pMinimap_Texure = nullptr;
	CTransform*		m_pMinimap_Transform = nullptr;
	_float					m_fMinimap[TRANSFORM_FLOAT_END];

	// Shot_Sound
	CGameObject* pPlayerTank = nullptr;
	_bool				m_bSoundOn = false;
	_float				m_fSoundSize = 0.f;

public:
	void						 Update_Shot_Sound(void);
	void						 Update_UI(void);
	void						 Update_Minimap(void);
private:
	/* Gold Text */
	void TextUpdate(const float& fDeltaTime);
	void TextRender();
	/* Gold Text */
	bool m_bTextUpdate = false;
	float m_fTimer = 0.f;
	const float m_fTimeOut = 4.f;
	int m_nGold = 0;
	_vec2 vTextPos{ 0,0 };
};

