#include "stdafx.h"
#include "..\Header\AH_64A_Camera.h"

#include "Export_Function.h"

CAH_64A_Camera::CAH_64A_Camera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCamera(pGraphicDev)
{
}

CAH_64A_Camera::CAH_64A_Camera(const CAH_64A_Camera & rhs)
	: CCamera(rhs)
{
}


CAH_64A_Camera::~CAH_64A_Camera()
{
}

HRESULT CAH_64A_Camera::Ready_Object(const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFov, const _float & fAspect, const _float & fNear, const _float & fFar)
{

	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);
	m_eID = CAMERA_ID::AH_64A_CAMERA;
	return S_OK;
}

_int CAH_64A_Camera::Update_Object(const _float & fTimeDelta)
{
	if(3.f < m_fShake_Power)
		m_fShake_Power += fTimeDelta;

	Mouse_Fix();
	Camera_Shake(fTimeDelta);
	Target_Renewal(fTimeDelta);

	_int	iExit = CCamera::Update_Object(fTimeDelta);
	return iExit;
}

void CAH_64A_Camera::LateUpdate_Object(void)
{
	CCamera::LateUpdate_Object();
}

void CAH_64A_Camera::Camera_Setting(_vec3 Target_Pos)
{
	m_vPos = Target_Pos;
	m_bFix = true;
	m_fShake_Power = 0.f;
	m_fShake_Time = 3.f;
}

void CAH_64A_Camera::Set_Target(CTransform * Target)
{
	m_pTarget_Trans = Target;
	
}

void CAH_64A_Camera::Mouse_Fix(void)
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

void CAH_64A_Camera::Target_Renewal(const _float & fTimeDelta)
{
	_vec3 UserPos, UserLook;

	m_pTarget_Trans ->Get_Info(INFO_POS, &UserPos);
	m_pTarget_Trans->Get_Info(INFO_LOOK, &UserLook);

	m_vPos.x = Get_Linear(m_vPos.x, UserPos.x, fTimeDelta);
	m_vPos.y = Get_Linear(m_vPos.y, UserPos.y, fTimeDelta);
	m_vPos.z = Get_Linear(m_vPos.z, UserPos.z, fTimeDelta);

	_vec3 Plus = (-UserLook * 30.f);
	Plus.y += 10.f;
	m_vEye = m_vPos + Plus;

	_matrix matCam;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCam);
	D3DXMatrixInverse(&matCam, 0, &matCam);
	_vec3 camLook = { matCam._31,matCam._32 ,matCam._33 };

	m_vAt = UserPos;
	
}

CAH_64A_Camera * CAH_64A_Camera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFov, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CAH_64A_Camera*	pInstance = new CAH_64A_Camera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		MSG_BOX("CAH_64A_Camera Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CAH_64A_Camera::Free(void)
{
	CGameObject::Free();
}
