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
class CVoxel;
END
class CDefault_Enermy :public CGameObject, public ICollisionable
{

private:
	explicit CDefault_Enermy(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDefault_Enermy(const CDefault_Enermy& rhs);
	virtual ~CDefault_Enermy();

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
	_int Get_Action() { return m_iAction; }
	void  Set_DisCountLocation();
	_bool Get_CreateCheck(void) { return CreateCheck; }
	void Set_CreateCheck(_bool _true) { CreateCheck = _true; }
	void AiMove(_float fTimeDelta);
public:
	//에너미에서 사용하는 함수
	void Basic(_float fTimeDelta);
	void Occupation(_float fTimeDelta);
	void Wait(_float fTimeDelta);
	void Enermy_In_Area(_float fTimeDelta);
	void Detect(_float fTimeDelta);
	_bool Left_RightCheck(_vec3 _vDir, _vec3 _vLook);
	_float Dist(CTransform* _Target);
	void ObjectCol(_bool m_Left);
	void Run(_float fTimeDelta);
	void Minus_HP_UI(_float HP_minus) { UI_fHP -= HP_minus; }
	void Plus_HP_UI(_float HP_plus) { UI_fHP += HP_plus; }
	_float GetHp(void) { return UI_fHP; }
	void 	ColObject(_float fTimeDelta);
private:
	HRESULT		Add_Component(void);
	EData*      m_EData;
private:
	CCalculator*		m_pCalculatorCom = nullptr;
	//탱크형태
	CVoxel*				m_pBody = nullptr;
	CVoxel*				m_pHead = nullptr;
	CVoxel*				m_pPosin = nullptr;

	//탱크 움직임
	CTransform*			m_pTransformHead = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTransform*			m_pTransformPosin = nullptr;

private:
	_int  m_iLocationState;
	_int  m_iAction = AIACTION::AIACTION_END;
	_float PreHp = 0.f;
	_bool m_bRightLocationCount = false
		, m_bRightTopLocationCount = false
		, m_bLeftLocationCount = false
		, m_bLeftTopLocationCount = false;
	_int m_iSuccessOccupation = LOCATIONCHECK::LOCATIONCHECK_END;
	_int m_iLocationCheck = false;
	_int m_PastLocation;
	_vec3   m_vPatrol = {};
	_bool LeftCheck = false;
	_bool m_bDef = false;
	_int Targeted = 0;
	_float Range = 0.f;
	_vec3 vPatrolRange = {};
	_bool m_bPatrol = false;
	_bool ColBuild = false;
	_int  ColBuildCount = 0;
	_int  BulletCount = 0;
	_bool m_bTest;
	_bool bLeft;
	_bool CreateCheck = false;
	_float re = 0.f;
	//탱크 정보
	_float m_fMaxHp, fCurHp, fAccel_top_speed, RotSpped, fPosinDist;
	_float m_fReloadTime, m_fReload;
	_int   m_iCannonSpeed, TempBullet;

public:
	static CDefault_Enermy*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CDefault_Enermy*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg);
private:
	virtual void Free(void) override;

	// HP UI
	_matrix				UI_matViewF, m_UI_ProjMatrix;
	CRcTex*				m_pRcTexF = nullptr;
	CTexture*			m_pTextureF = nullptr;
	CTransform*		m_pTransformHP_UI = nullptr;
	_float					UI_Orgin_HP, UI_fHP;
	_float					UI_fScaleX, UI_fScaleY, UI_fScaleZ;
	_float					UI_fOrgin_ScaleX;


	// Minimap UI
	_matrix				UI_Minimap_matProj;
	CRcTex*				m_pMinimap_RcTex = nullptr;
	CTexture*			m_pMinimap_Texure = nullptr;
	CTransform*		m_pMinimap_Transform = nullptr;
	_float					m_fMinimap[TRANSFORM_FLOAT_END];

public:
	void						 Update_UI(void);
	void						 Update_Minimap(void);

};

