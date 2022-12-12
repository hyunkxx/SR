#include "stdafx.h"
#include "..\Header\MoonWalkCamera.h"

#include "Export_Function.h"

CMoonWalkCamera::CMoonWalkCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCamera(pGraphicDev)
{
}

CMoonWalkCamera::CMoonWalkCamera(const CMoonWalkCamera & rhs)
	: CCamera(rhs)
{
}

CMoonWalkCamera::~CMoonWalkCamera()
{
}

HRESULT CMoonWalkCamera::Ready_Object(const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFov, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;
	m_vLook;
	m_fShake_Power = 50.f;
	m_fShake_Time = 1.f;
	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);
	m_eID = CAMERA_ID::MOONWALK_CAMERA;
	return S_OK;
}

_int CMoonWalkCamera::Update_Object(const _float & fTimeDelta)
{
	Target_Renewal(fTimeDelta);
	Camera_Shake(fTimeDelta);
	Mouse_Fix();
	return CCamera::Update_Object(fTimeDelta);
}

void CMoonWalkCamera::LateUpdate_Object(void)
{
	CCamera::LateUpdate_Object();
}

void CMoonWalkCamera::Camera_Setting(_vec3 _Target_Pos)
{

	CComponent* pComponent = Engine::Get_Component(L"GameLogic", L"PlayerVehicle", L"Proto_TransformHead", ID_DYNAMIC);
	_vec3 Move_Pos;
	static_cast<CTransform*>(pComponent)->Get_Info(INFO_POS, &Move_Pos);
	m_vDir = Move_Pos - _Target_Pos;
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	Target_Pos = _Target_Pos;
	Target_Pos.y += 1.f;
	m_vEye = (_Target_Pos + m_vDir * 3.f);
	Target_Pos.y += 1.f;
	m_bShake = true;


}

void CMoonWalkCamera::Mouse_Fix(void)
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

void CMoonWalkCamera::Target_Renewal(const _float & fTimeDelta)
{

	CComponent* pComponent = Engine::Get_Component(L"GameLogic", L"PlayerVehicle", L"Proto_TransformHead", ID_DYNAMIC);
	_vec3 Move_Pos;
	static_cast<CTransform*>(pComponent)->Get_Info(INFO_POS, &Move_Pos);
	//TargetPos -= (TargetLook * 5);
	//TargetPos.y;
	_vec3 Inst = Move_Pos - m_vEye;
	D3DXVec3Normalize(&m_vDir, &Inst);

	m_vEye.x += m_vDir.x * 50.f * fTimeDelta;
	m_vEye.z += m_vDir.z * 50.f * fTimeDelta;

	if (1.f >= sqrtf((Inst.x * Inst.x) + (Inst.z * Inst.z)))
	{
		Engine::Camera_Change(L"TankCamera");
	}

	m_vAt = Target_Pos;

}

CMoonWalkCamera * CMoonWalkCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFov, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CMoonWalkCamera* pInstance = new CMoonWalkCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		MSG_BOX("CMoonWalkCamera Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CMoonWalkCamera::Free(void)
{
	CGameObject::Free();
}
