#include "stdafx.h"
#include "..\Header\BoomCamera.h"

#include "Export_Function.h"
#include "Bomber.h"

CBoomCamera::CBoomCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCamera(pGraphicDev)
{
}

CBoomCamera::CBoomCamera(const CBoomCamera & rhs)
	: CCamera(rhs)
{
}

CBoomCamera::~CBoomCamera()
{
}

HRESULT CBoomCamera::Ready_Object(const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFov, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;
	m_vLook;
	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);
	m_eID = CAMERA_ID::BOOM_CAMERA;
	return S_OK;
}

_int CBoomCamera::Update_Object(const _float & fTimeDelta)
{
	Mouse_Fix();
	Target_Renewal(fTimeDelta);
	return  CCamera::Update_Object(fTimeDelta);
}

void CBoomCamera::LateUpdate_Object(void)
{
	CCamera::LateUpdate_Object();
}

void CBoomCamera::Set_Target(CTransform * Target)
{
	Target_Trans = Target;
}

void CBoomCamera::Mouse_Fix(void)
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

void CBoomCamera::Target_Renewal(const _float & fTimeDelta)
{
	_vec3 TargetPos, TargetLook;
	if (Target_Trans)
	{

		Target_Trans->Get_Info(INFO_POS, &TargetPos);
		Target_Trans->Get_Info(INFO_LOOK, &TargetLook);
	}
	//TargetPos -= (TargetLook * 5);
	//TargetPos.y;
	m_vPos.x = Get_Linear(m_vPos.x, TargetPos.x, fTimeDelta);
	m_vPos.y = Get_Linear(m_vPos.y, TargetPos.y, fTimeDelta);
	m_vPos.z = Get_Linear(m_vPos.z, TargetPos.z, fTimeDelta);

	m_vAt = TargetPos;
	m_vEye = m_vPos;
//	m_vEye.y += 10.f;
}

CBoomCamera * CBoomCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFov, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CBoomCamera* pInstance = new CBoomCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		MSG_BOX("CBoomCamera Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBoomCamera::Free(void)
{
	CGameObject::Free();
}
