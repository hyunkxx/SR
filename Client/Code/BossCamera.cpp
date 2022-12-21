#include "stdafx.h"
#include "..\Header\BossCamera.h"

#include "Export_Function.h"

CBossCamera::CBossCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCamera(pGraphicDev)
{
}

CBossCamera::CBossCamera(const CBossCamera & rhs)
	: CCamera(rhs)
{
}

CBossCamera::~CBossCamera()
{
}

HRESULT CBossCamera::Ready_Object(const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFov, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);
	//m_eID = CAMERA_ID::BOSS_APPEAR_CAMERA;
	return S_OK;
}

_int CBossCamera::Update_Object(const _float & fTimeDelta)
{
	
	m_fCamera_Count += fTimeDelta;
	
	if (m_bUp)
	{
		m_vEye.y = 30.f;

		if (D3DXToRadian(80.f) > m_fFov)
		{
			m_fFov += D3DXToRadian(5.f * fTimeDelta);
			if (D3DXToRadian(80.f) < m_fFov)
				m_bUp = false;
			_vec3 Dir = m_vAt - m_vEye;
			D3DXVec3Normalize(&Dir, &Dir);
			m_vEye += Dir * 35.f * fTimeDelta;
		}
	}
	else
	{
		if (D3DXToRadian(40.f) < m_fFov)
		{
			m_fFov -= D3DXToRadian(10.f * fTimeDelta);
			_vec3 Dir  = m_vAt - m_vEye;
			D3DXVec3Normalize(&Dir, &Dir);
			m_vEye -= Dir * 80.f * fTimeDelta;
			m_vEye.y += 10.f * fTimeDelta;
		}
	}

	if (8.f < m_fCamera_Count)
		Engine::Camera_Change(L"TankCamera");

	Mouse_Fix();
	Camera_Shake(fTimeDelta);
	Target_Renewal(fTimeDelta);


	_int	iExit = CCamera::Update_Object(fTimeDelta);
	return iExit;
}

void CBossCamera::LateUpdate_Object(void)
{
	CCamera::LateUpdate_Object();
}

void CBossCamera::Camera_Setting(_vec3 Target_Pos)
{
	m_vPos = { 0.f,0.f ,0.f };
	m_vEye = {0.f,0.f ,0.f };
	m_vAt = Target_Pos;
	m_vEye = Target_Pos;
	m_vEye.z -= 400.f;
	m_bFix = true;
	m_bUp = true;
	
	__super::Shake_On();

}

void CBossCamera::Mouse_Fix(void)
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

void CBossCamera::Target_Renewal(const _float & fTimeDelta)
{
}

CBossCamera * CBossCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFov, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CBossCamera*	pInstance = new CBossCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		MSG_BOX("BossCamera Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBossCamera::Free(void)
{
	CGameObject::Free();
}
