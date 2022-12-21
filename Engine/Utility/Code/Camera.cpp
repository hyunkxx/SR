#include "..\..\Header\Camera.h"

#include "Export_Function.h"

USING(Engine)


CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

Engine::CCamera::CCamera(const CCamera& rhs)
	: CGameObject(rhs)
{

}

CCamera::~CCamera()
{
}

HRESULT CCamera::Ready_Object(void)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return S_OK;
}

_int CCamera::Update_Object(const _float& fTimeDelta)
{
	Fov_Setting(fTimeDelta);

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return 0;
}

void CCamera::LateUpdate_Object(void)
{

}

void CCamera::Camera_Shake(const _float & fTimeDelta)
{
	if (!m_bShake)
		return;

	if (m_bxRand)
	{
		m_vEye.x -= sin(m_fAccum*m_fShake_Power) * powf(0.5f, m_fAccum);
		m_vAt.x -= sin(m_fAccum*m_fShake_Power) * powf(0.5f, m_fAccum);
		m_bxRand = false;
	}
	else
	{
		m_vEye.x += sin(m_fAccum*m_fShake_Power) * powf(0.5f, m_fAccum);
		m_vAt.x += sin(m_fAccum*m_fShake_Power) * powf(0.5f, m_fAccum);
		m_bxRand = true;
	}

	m_fAccum += fTimeDelta;
	m_vEye.y += sin(m_fAccum*m_fShake_Power) * powf(0.5f, m_fAccum);
	m_vEye.z += sin(m_fAccum*m_fShake_Power) * powf(0.5f, m_fAccum);

	m_vAt.y += sin(m_fAccum*m_fShake_Power) * powf(0.5f, m_fAccum);
	m_vAt.z += sin(m_fAccum*m_fShake_Power) * powf(0.5f, m_fAccum);

	if (m_fAccum >= m_fShake_Time)
	{
		m_fAccum = 0.f;
		m_bShake = false;
	}
}

void CCamera::Fov_Setting(const _float & fTimeDelta)
{
	if (m_bFovSet)
	{
		_float Angle = m_fSetFov - m_fFov;
		if (D3DXToRadian(1.f)< abs(Angle))
		{
			if (D3DXToRadian(0.f) < Angle)
			{
				m_fFov += D3DXToRadian(5.f * fTimeDelta);
			}
			else
			{
				m_fFov -= D3DXToRadian(5.f * fTimeDelta);
			}
		}
		else
			m_bFovSet = false;
	}
}

void Engine::CCamera::Free(void)
{
	CGameObject::Free();
}
