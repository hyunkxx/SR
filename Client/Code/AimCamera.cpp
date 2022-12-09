#include "stdafx.h"
#include "..\Header\AimCamera.h"

#include "Export_Function.h"
#include "TankSet.h"
CAimCamera::CAimCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}

CAimCamera::CAimCamera(const CAimCamera & rhs)
	: CCamera(rhs)
{
}

CAimCamera::~CAimCamera()
{
}

HRESULT CAimCamera::Ready_Object(const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFov, const _float & fAspect, const _float & fNear, const _float & fFar)
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
	m_eID = CAMERA_ID::AIM_CAMERA;

	return S_OK;
}

_int CAimCamera::Update_Object(const _float & fTimeDelta)
{

	CGameObject* pPlayer = (Engine::Get_Object(L"GameLogic", L"PlayerVehicle"));
	NULL_CHECK_RETURN(pPlayer, E_FAIL);
	CTransform* pPlayerTrans = static_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"PlayerVehicle", L"Proto_TransformBody", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTrans, E_FAIL);

	_vec3 HitPos = static_cast<CTankSet*>(pPlayer)->Get_HitPos();
	_vec3 PlyerPos;
	pPlayerTrans->Get_Info(INFO_POS, &PlyerPos);
	HitPos.y = PlyerPos.y = 2.4f;
	_vec3 vLook = HitPos - PlyerPos;
	m_vLook = *D3DXVec3Normalize(&vLook, &vLook);
	m_vAt = PlyerPos + m_vLook;

	Mouse_Fix();
	Mouse_Move(fTimeDelta);
	Key_Input(fTimeDelta);

	NULL_CHECK_RETURN(pPlayerTrans, E_FAIL);

	_vec3 a = { 0.f,2.4f,0.f };
	m_vEye = PlyerPos + a;
	D3DXVec3Normalize(&m_vLook,&m_vLook);
	pPlayerTrans->Get_Info(INFO_POS, &PlyerPos);

	
	m_vEye = PlyerPos + a;
	m_vAt = m_vEye + m_vLook;
	_int	iExit = CCamera::Update_Object(fTimeDelta);

	return iExit;
}

void CAimCamera::LateUpdate_Object(void)
{
	CCamera::LateUpdate_Object();
}

void CAimCamera::Mouse_Move(const _float & fTimeDelta)
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, nullptr, &m_matView);

	_long		dwMouseMove = 0;

	if (dwMouseMove = Get_DIMouseMove(DIMS_X))
	{
		_vec3		vUp{ 0.f, 1.f, 0.f };

		_matrix	matRot;
		D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 10.f));
		D3DXVec3TransformNormal(&m_vLook, &m_vLook, &matRot);

	}

}

void CAimCamera::Mouse_Fix(void)
{
	POINT	pt{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y + 5);
}

void CAimCamera::Key_Input(const _float & fTimeDelta)
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

CAimCamera * CAimCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFov, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CAimCamera*	pInstance = new CAimCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		MSG_BOX("CAimCamera Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CAimCamera::Free(void)
{
	CGameObject::Free();
}
