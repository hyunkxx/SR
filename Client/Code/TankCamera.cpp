#include "stdafx.h"
#include "..\Header\TankCamera.h"

#include "Export_Function.h"
#include "BoomEffect.h"

CTankCamera::CTankCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}

CTankCamera::CTankCamera(const CTankCamera & rhs)
	: CCamera(rhs)
{
}

CTankCamera::~CTankCamera()
{
}

HRESULT CTankCamera::Ready_Object(const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFov, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;
	m_vLook = { 0.f,10.f,1.f };
	m_fShake_Power = 50.f;
	m_fShake_Time = 1.f;
	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);
	m_eID = CAMERA_ID::TANK_CAMERA;
	return S_OK;
}

_int CTankCamera::Update_Object(const _float & fTimeDelta)
{
	Key_Input(fTimeDelta);

	CTransform* pPlayerTrans = static_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"PlayerVehicle", L"Proto_TransformBody", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTrans, E_FAIL);

	_vec3	PlyerPos;
	pPlayerTrans->Get_Info(INFO_POS, &PlyerPos);

	_vec3		vUp{ 0.f, 1.f, 0.f };

	m_vAt = PlyerPos + m_vLook;

	_vec3	vInserveLook = -(m_vAt - PlyerPos);

	vInserveLook.y = 0.7f;
	m_vEye = PlyerPos + (vInserveLook * 20.f);
	Camera_Shake(fTimeDelta);
	_int	iExit = CCamera::Update_Object(fTimeDelta);
	if (false == m_bFix)
	{
		Mouse_Move(fTimeDelta);
		Mouse_Fix();
	}
	return iExit;
}

void CTankCamera::LateUpdate_Object(void)
{



	CCamera::LateUpdate_Object();
}

void CTankCamera::Key_Input(const _float & fTimeDelta)
{

	if (Get_DIKeyState(DIK_TAB) & 0x80)
	{
		if (m_bCheck)
			return;

		m_bCheck = true;

		if (m_bFix)
			m_bFix = false;
		else
			m_bFix = true;
	}
	else
		m_bCheck = false;

	if (false == m_bFix)
		return;

}

void CTankCamera::Mouse_Move(const _float & fTimeDelta)
{
	if (m_bRock)
		return;
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, nullptr, &m_matView);

	_long		dwMouseMove = 0;

	if (dwMouseMove = Get_DIMouseMove(DIMS_X))
	{
		_vec3		vUp{ 0.f, 1.f, 0.f };

		//_vec3	vLook = m_vAt - m_vEye;

		_matrix	matRot;
		D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 15.f));
		D3DXVec3TransformNormal(&m_vLook, &m_vLook, &matRot);
		m_vAt = m_vEye + m_vLook;;
		/*_vec3 vAt = m_vEye + m_vLook;

		m_vAt.x = Get_Linear(m_vAt.x, vAt.x, fTimeDelta);
		m_vAt.y = Get_Linear(m_vAt.y, vAt.y, fTimeDelta);
		m_vAt.z = Get_Linear(m_vAt.z, vAt.z, fTimeDelta);*/
	}
}

void CTankCamera::Mouse_Fix(void)
{
	POINT	pt{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

void CTankCamera::Camera_Setting(_vec3 Target_Pos)
{
	CGameObject* pObject = Engine::Get_Object(L"GameLogic", L"BoomEffect");
	pObject->Set_Dead(false);
	static_cast<CBoomEffect*>(pObject)->Set_Pos(Target_Pos);

	Target_Pos.y = 0.f;
	Target_Pos -= m_vEye;
	D3DXVec3Normalize(&Target_Pos, &Target_Pos);
	m_vLook.x = Target_Pos.x;
	m_vLook.z = Target_Pos.z;

	m_vAt = m_vEye + m_vLook;;
}

CTankCamera * CTankCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFov, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CTankCamera*	pInstance = new CTankCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		MSG_BOX("CTankCamera Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CTankCamera::Free(void)
{
	CGameObject::Free();
}
