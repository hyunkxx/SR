#pragma once
#include "GameObject.h"

#include "Collisionable.h"
BEGIN(Engine)
class CRcTex;
class CTexture;
class CTransform;
class CCalculator;
class CVoxel;
END

class CRushTank :
	public CGameObject ,  public ICollisionable
{
private:
	explicit CRushTank(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRushTank(const CRushTank& rhs);
	virtual ~CRushTank();

public:
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;


	virtual const _vec3	Get_Info(void)override;
	virtual	void		Move_Info(_vec3 _Info)override;
	virtual			void		RenderGUI(void)override {}
	virtual			void		OBB_Collision_EX(void);
	virtual			void		Update_OBB(void);
	virtual			OBB*        Get_OBB(void) { return &m_stBody; }

private:
	virtual	HRESULT		Add_Component(void);
	virtual HRESULT		Ready_Object(void);
	virtual void		Sound_Setting(const _float & fTimeDelta);
	virtual void        Shoot_Bullet(BULLET_ID eID);
	void				Posin_Setting(const _vec3& _SetPos);
	void				Head_Setting(const _vec3& _SetPos);
	void				Body_Setting(const _vec3& _SetPos);

	virtual void		Dead_Motion(const _float& fTimeDelta);

public:
	void		Key_Input(const _float& fTimeDelta);
	void		SetUp_OnTerrain(void);
	void		Head_Spin(const _float& fTimeDelta);

private:
	void		Expect_Hit_Point(const _float& fTimeDelta);
	_vec3		PickUp_OnTerrain(void);
	void		Camera_Change(void);
	void		Posin_Shake(const _float& fTimeDelta);

private:
	void				Reset_Speed(void) { m_stInfo.fAccum = 0.f; }

public:
	void				Rotation_Body(ROTATION eID, _float fAngle);
	void				Rotation_Head(ROTATION eID, _float fAngle);
	void				Rotation_Posin(ROTATION eID, _float fAngle);

	_float				Plus_Advance_AccelSpeed(const _float& fTimeDelta);
	_float				Minus_Advance_AccelSpeed(const _float& fTimeDelta);

	_float				Plus_Back_AccelSpeed(const _float& fTimeDelta);
	_float				Minus_Back_AccelSpeed(const _float& fTimeDelta);

	const _vec3				Get_HitPos(void) { return m_fHitPos; }
	void				Set_Rock(_bool Rock) { m_bRock = Rock; }

	const Tank_State Get_TankInfo(void) { return  m_stInfo; }
	_bool				Get_God_Mode(void) { return m_bGodMode; }
private:

	CTransform*			m_pTransformBody = nullptr;
	CTransform*			m_pTransformHead = nullptr;
	CTransform*			m_pTransformPosin = nullptr;

	CCalculator*		m_pCalculatorCom = nullptr;

	Tank_State			m_stInfo;

	_bool				m_bStart = false;
	_bool				m_bRock = false;
	_bool				m_bMove = false;
	_bool				m_bReLoad = true;
	_bool				m_bPosinShake = false;
	_float				m_fPosinAccum = 0.f;
	
	_float				m_fMoveCount = 0.f;
	_float				m_fEngineCount = 0;
	_int				m_iSound_Count = 0;
	//Voxel Ãß°¡
	CVoxel*				m_pBody = nullptr;
	CVoxel*				m_pHead = nullptr;
	CVoxel*				m_pPosin = nullptr;

	_float				m_fDT = 0.f;

	_vec3				m_fHitPos;
	//
	_int				m_iMotionCount = 0;

	_bool				m_bGodMode = false;
	_float				m_fGodCount = 0.f;
public:
	virtual void Free(void) override;
	static CRushTank* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

