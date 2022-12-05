#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera :	public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
protected:
	void		Camera_Shake(const _float& fTimeDelta);
public:
	void		Shake_On(void)
	{
		if (!m_bShake)
			m_bShake = true;
		else
			m_fAccum = 0.f;
	}

public:
	_float Get_Linear(_float a, _float b, _float Alpha)
	{
		return a * (1 - Alpha) + b * Alpha;
	}
	virtual void	Set_Camera(_bool CameraOn) { m_bCameraOn = CameraOn; }
	const	_bool	Get_CameraOn(void) const { return m_bCameraOn; }
	const _vec3		Get_Look(void) { return *D3DXVec3Normalize(&_vec3(), &_vec3(m_vAt - m_vEye)); }

protected:
	_vec3				m_vEye, m_vAt, m_vUp;
	_float				m_fAspect, m_fFov, m_fNear, m_fFar;
	_matrix				m_matView, m_matProj;
	//카메라 쉐이킹용 누적 시간
	_bool		m_bShake = false;
	_bool		m_bxRand = false;
	_float		m_fAccum = 0.f;
	_bool		m_bCameraOn = false;
	_bool		m_bSetProj = false;
protected:
	virtual void		Free(void);
};

END
