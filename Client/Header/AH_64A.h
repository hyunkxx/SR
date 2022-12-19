#pragma once
#include "GameObject.h"
#include "Voxel.h"

BEGIN(Engine)
class CTransform;
END

class CAH_64A :
	public CGameObject
{
public:
	explicit CAH_64A(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAH_64A(const CAH_64A & rhs);
	virtual ~CAH_64A();

public:
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;
	virtual void		RenderGUI(void)override;

	void				Key_Input(const _float& fTimeDleta);
private:	
	_float				Plus_Advance_AccelSpeed(const _float & fTimeDelta);
	_float				Minus_Advance_AccelSpeed(const _float & fTimeDelta);

	_float				Plus_Right_AccelSpeed(const _float & fTimeDelta);
	_float				Minus_Right_AccelSpeed(const _float & fTimeDelta);

public:
	void				Start_AH_64A(void);
	_vec3				Get_Info(INFO Info) { return m_vInfo[Info]; }
	_vec3				Get_vRight_Gun_Pos(void) { return m_vInfo[INFO_POS] + 3 * m_vInfo[INFO_RIGHT]; }
	_vec3				Get_vLeft_Gun_Pos(void) { return m_vInfo[INFO_POS] - 3 * m_vInfo[INFO_RIGHT]; }

private:
	void				Set_Transform(const _float& fTimeDelta);
private:
	virtual	HRESULT		Add_Component(void);
	virtual HRESULT		Ready_Object(void);

	CTransform*			m_pTransformBody = nullptr;
	CTransform*			m_pTransformHead = nullptr;
	CTransform*			m_pTransformTail = nullptr;

	CVoxel*				m_pBody = nullptr;
	CVoxel*				m_pHead = nullptr;
	CVoxel*				m_pTail = nullptr;

	_float	m_fAdAccum = 0.f;
	_float	m_fRightAccum = 0.f;

	_vec3 m_vInfo[INFO_END];

	_float m_fAccum = 0.f;

	
	_float m_fRot[ROT_END];

	_bool m_bLock = false;
	_bool m_bRight_Shoot = true;

	_float m_fAdSpeed  = 0.f;
	_float m_fRightSpeed = 0.f;
	_float m_fReloadTime = 0.f;

	_float m_fPlayCount = 0.f;

	_float m_fSoundCount = 30.f;
	_float m_fInSoundCount = 0.f;
	_bool m_In_Sound = false;

	_float m_fSound;

	_bool m_bAppear = false;
public:
	static CAH_64A* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

};

