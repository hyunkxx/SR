#include "stdafx.h"
#include "..\Header\AH_64A_EndCamera.h"

#include "Export_Function.h"
#include "TankSet.h"

CAH_64A_EndCamera::CAH_64A_EndCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCamera(pGraphicDev)
{
}

CAH_64A_EndCamera::CAH_64A_EndCamera(const CAH_64A_EndCamera & rhs)
	: CCamera(rhs)
{
}

CAH_64A_EndCamera::~CAH_64A_EndCamera()
{
}

HRESULT CAH_64A_EndCamera::Ready_Object(const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFov, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);
	m_eID = CAMERA_ID::AH_64A_END_CAMERA;
	return S_OK;
}

_int CAH_64A_EndCamera::Update_Object(const _float & fTimeDelta)
{
	if (3.f < m_fShake_Power)
		m_fShake_Power += fTimeDelta;

	Mouse_Fix();
	Camera_Shake(fTimeDelta);
	Target_Renewal(fTimeDelta);

	_int	iExit = CCamera::Update_Object(fTimeDelta);
	return iExit;
}

void CAH_64A_EndCamera::LateUpdate_Object(void)
{
	CCamera::LateUpdate_Object();
}

void CAH_64A_EndCamera::Camera_Setting(_vec3 Target_Pos)
{
	m_AH_64A_Pos = Target_Pos;
	m_vPos = Target_Pos;
	m_bFix = true;
	m_fShake_Power = 0.f;
	m_fShake_Time = 3.f;
}

void CAH_64A_EndCamera::Mouse_Fix(void)
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

void CAH_64A_EndCamera::Target_Renewal(const _float & fTimeDelta)
{
	_vec3 UserPos;
	UserPos = Engine::Get_Camera(L"TankCamera")->Get_Eye();
	UserPos += _vec3(10.f, 10.f, 10.f);
	m_vPos.x = Get_Linear(m_vPos.x, UserPos.x, fTimeDelta);
	m_vPos.y = Get_Linear(m_vPos.y, UserPos.y, fTimeDelta);
	m_vPos.z = Get_Linear(m_vPos.z, UserPos.z, fTimeDelta);

	m_vEye = m_vPos;

	_vec3 vDist = UserPos - m_vPos;

	_float fDist = (sqrtf(vDist.x * vDist.x + vDist.z * vDist.z) * sqrtf(vDist.x * vDist.x + vDist.z * vDist.z));

	m_vAt = m_AH_64A_Pos;

	if (300.f > fDist)
	{
		Engine::Camera_Change(L"TankCamera");
		static_cast<CTankSet*>(Engine::Get_Object(L"GameLogic", L"PlayerVehicle"))->Set_Rock(false);
	}
}

CAH_64A_EndCamera * CAH_64A_EndCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFov, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CAH_64A_EndCamera*	pInstance = new CAH_64A_EndCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		MSG_BOX("AH_64A_EndCamera Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CAH_64A_EndCamera::Free(void)
{
	CGameObject::Free();
}
