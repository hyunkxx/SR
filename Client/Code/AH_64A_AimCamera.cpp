#include "stdafx.h"
#include "..\Header\AH_64A_AimCamera.h"

#include "Export_Function.h"
#include "AH_64A.h"
CAH_64A_AimCamera::CAH_64A_AimCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}

CAH_64A_AimCamera::CAH_64A_AimCamera(const CAH_64A_AimCamera & rhs)
	: CCamera(rhs)
{
}

CAH_64A_AimCamera::~CAH_64A_AimCamera()
{
}

HRESULT CAH_64A_AimCamera::Ready_Object(const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFov, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	m_vEye = *pEye;
	//m_PevEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;
	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);
	m_eID = CAMERA_ID::AH_64A_AIMCAMERA;

	return S_OK;
}

_int CAH_64A_AimCamera::Update_Object(const _float & fTimeDelta)
{
	Mouse_Fix();
	Mouse_Move(fTimeDelta);
	Key_Input(fTimeDelta);
	_vec3 PlyerPos;
	_vec3 PlyerLook;
	//static_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"AH_64A", L"Proto_Body_Transform", ID_DYNAMIC))->Get_Info(INFO_POS,&PlyerPos);
	//static_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"AH_64A", L"Proto_Body_Transform", ID_DYNAMIC))->Get_Info(INFO_LOOK, &PlyerLook);

	PlyerPos  = static_cast<CAH_64A*>(Engine::Get_Object(L"GameLogic", L"AH_64A"))->Get_Info(INFO_POS);
	PlyerLook = static_cast<CAH_64A*>(Engine::Get_Object(L"GameLogic", L"AH_64A"))->Get_Info(INFO_LOOK);

	_vec3 a = { 0.f,2.4f,0.f };

	m_vEye = PlyerPos + a;

	m_vAt = m_vEye + PlyerLook;
	_int	iExit = CCamera::Update_Object(fTimeDelta);

	return iExit;
}

void CAH_64A_AimCamera::LateUpdate_Object(void)
{
	CCamera::LateUpdate_Object();
}

void CAH_64A_AimCamera::Mouse_Move(const _float & fTimeDelta)
{

}

void CAH_64A_AimCamera::Mouse_Fix(void)
{
	POINT	pt{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y + 5);
}

void CAH_64A_AimCamera::Key_Input(const _float & fTimeDelta)
{
	if (Get_DIKeyState_Custom(DIK_1) == KEY_STATE::HOLD && D3DXToRadian(30.f) <= m_fFov)
	{
		m_fFov -= D3DXToRadian(30.f * fTimeDelta);
		m_bSetProj = true;
	}
	if (Get_DIKeyState_Custom(DIK_2) == KEY_STATE::HOLD && D3DXToRadian(60.f) >= m_fFov)
	{
		m_fFov += D3DXToRadian(30.f * fTimeDelta);
		m_bSetProj = true;
	}
}

void CAH_64A_AimCamera::Camera_Setting(_vec3 Target_Pos)
{
}

CAH_64A_AimCamera * CAH_64A_AimCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFov, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CAH_64A_AimCamera*	pInstance = new CAH_64A_AimCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		MSG_BOX("CAH_64A_AimCamera Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CAH_64A_AimCamera::Free(void)
{
	CGameObject::Free();
}
