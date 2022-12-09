#include "stdafx.h"
#include "..\Header\SimpleCamera.h"


CSimpleCamera* CSimpleCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vEye, const _vec3* vLook, const _vec3* vUp)
{
	CSimpleCamera* pInstance = new CSimpleCamera(pGraphicDev, vEye, vLook, vUp);
	if (FAILED(pInstance->Ready_Object(vEye, vLook, vUp, D3DXToRadian(65), WINCX / WINCY, 0.1f, 1000.f)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

CSimpleCamera::CSimpleCamera(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vEye, const _vec3* vLook, const _vec3* vUp)
	: CGameObject(pGraphicDev)
{
}

CSimpleCamera::CSimpleCamera(const CSimpleCamera& rhs)
	: CGameObject(rhs)
{
}

CSimpleCamera::~CSimpleCamera()
{
}

HRESULT CSimpleCamera::Ready_Object(
	const _vec3* pEye,
	const _vec3* pAt,
	const _vec3* pUp,
	const float& fFOV,
	const float& fAspect,
	const float& fNear,
	const float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFOV = fFOV;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFOV, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_eID = CAMERA_ID::SIMPLE_CAMERA;
	
	return S_OK;
}

_int CSimpleCamera::Update_Object(const _float & fTimeDelta)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFOV, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return _int();
}

void CSimpleCamera::LateUpdate_Object(void)
{
}

void CSimpleCamera::Render_Object(void)
{
}

void CSimpleCamera::RenderGUI(void)
{
}

void CSimpleCamera::Free()
{
	__super::Free();
}