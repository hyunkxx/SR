#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void		 Camera_Setting(_vec3	Target_Pos)PURE;
protected:
	void		Camera_Shake(const _float& fTimeDelta);
public:
	_matrix		Get_View(void) { return m_matView; }
public:
	_bool		Get_MouseFix() { return m_bFix; }
	void		Set_MouseFix(bool bValue) { ShowCursor(bValue); m_bFix = bValue; }
	void		Shake_On(void)
	{
		if (!m_bShake)
			m_bShake = true;
		else
			m_fAccum = 0.f;
	}
	_vec3	Get_Eye(void) { return m_vEye; }
	void		Set_Fov(_float Fov) { m_bFovSet = true; m_fSetFov = Fov; }
	void		Fov_Setting(const _float& fTimeDelta);
	void		Set_Lock(_bool Lock) { m_bSeeLock = Lock; }
	_bool		Get_Lock(void) { return m_bSeeLock; }
public:
	_float Get_Linear(_float a, _float b, _float Alpha)
	{
		return a * (1 - Alpha) + b * Alpha;
	}
	const _vec3		Get_Look(void) { return *D3DXVec3Normalize(&_vec3(), &_vec3(m_vAt - m_vEye)); }
	CAMERA_ID		Get_ID(void) { return m_eID; }
	void			Set_Rock(_bool Rock) { m_bRock = Rock; }
protected:
	_bool		m_bFix = false;
	_vec3		m_vEye, m_vAt, m_vUp;
	_float		m_fAspect, m_fFov, m_fNear, m_fFar;
	_matrix		m_matView, m_matProj;
	//카메라 쉐이킹용 누적 시간
	_bool		m_bShake = false;
	_bool		m_bxRand = false;
	_float		m_fAccum = 0.f;
	_bool		m_bSetProj = false;
	CAMERA_ID	m_eID;

	_float		m_fShake_Power = 0.f;
	_float		m_fShake_Time = 0.f;
	_bool		m_bRock = false;
	_bool		m_bFovSet = false;
	_float		m_fSetFov = 60.f;

	_bool		m_bSeeLock = true;
protected:
	virtual void		Free(void);
private:
};

END
