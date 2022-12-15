#include "stdafx.h"
#include "..\Header\ShipCamera.h"

#include "Export_Function.h"
CShipCamera::CShipCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCamera(pGraphicDev)
{
}

CShipCamera::CShipCamera(const CShipCamera & rhs)
	: CCamera(rhs)
{
}

CShipCamera::~CShipCamera()
{
}

HRESULT CShipCamera::Ready_Object(const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFov, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;
	m_vLook;
	m_fShake_Power = 20.f;
	m_fAccum = 0.f;
	m_fShake_Time = 3.f;
	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);
	m_eID = CAMERA_ID::BOOM_CAMERA;
	return S_OK;
}

_int CShipCamera::Update_Object(const _float & fTimeDelta)
{
	Mouse_Fix();
	Target_Renewal(fTimeDelta);
	Camera_Shake(fTimeDelta);
	return  CCamera::Update_Object(fTimeDelta);
}

void CShipCamera::LateUpdate_Object(void)
{
	CCamera::LateUpdate_Object();
}

void CShipCamera::Set_Target(CTransform * Target)
{
	Target_Trans = Target;
}

void CShipCamera::Mouse_Fix(void)
{
	if (Engine::Get_DIKeyState_Custom(DIK_TAB) == KEY_STATE::TAP)
	{
		if (m_bFix)
			m_bFix = false;
		else
			m_bFix = true;
	}
	if (m_bFix)
	{
		POINT	pt{ WINCX >> 1, WINCY >> 1 };

		ClientToScreen(g_hWnd, &pt);
		SetCursorPos(pt.x, pt.y);
	}
}

void CShipCamera::Target_Renewal(const _float & fTimeDelta)
{
	_vec3 Pos, TargetPos, TargetLook;
	if (Target_Trans)
	{
		Target_Trans->Get_Info(INFO_POS, &TargetPos);
		Target_Trans->Get_Info(INFO_LOOK, &TargetLook);
		Pos = TargetPos;
		Pos.x += m_fDist;
		Pos.z += m_fDist;
	}
	if (m_fDist > 70.f)
	{
		Pos.y += 20.f;
	}
	m_vPos.x = Get_Linear(m_vPos.x, Pos.x, fTimeDelta);
	m_vPos.y = Get_Linear(m_vPos.y, Pos.y, fTimeDelta);
	m_vPos.z = Get_Linear(m_vPos.z, Pos.z, fTimeDelta);

	m_vAt = TargetPos;
	if(!m_bLock)
		m_vEye = m_vPos;
}

CShipCamera * CShipCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFov, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CShipCamera* pInstance = new CShipCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		MSG_BOX("CShipCamera Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CShipCamera::Free(void)
{
	CGameObject::Free();
}
