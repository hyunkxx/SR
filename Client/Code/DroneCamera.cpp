#include "stdafx.h"
#include "..\Header\DroneCamera.h"

#include "Export_Function.h"

CDroneCamera::CDroneCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}

CDroneCamera::CDroneCamera(const CCamera & rhs)
	: CCamera(rhs)
{
}

CDroneCamera::~CDroneCamera()
{
}

HRESULT CDroneCamera::Ready_Object(const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFov, const _float & fAspect, const _float & fNear, const _float & fFar)
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
	m_eID = CAMERA_ID::DRONE_CAMERA;

	return S_OK;
}

_int CDroneCamera::Update_Object(const _float & fTimeDelta)
{
	_vec3 TargetPos;
	CGameObject* pTarget = (Engine::Get_Object(L"GameLogic", L"Boom_Support"));
	NULL_CHECK_RETURN(pTarget, E_FAIL);
	static_cast<CTransform*>(pTarget->Get_Component(L"Proto_Transform", ID_DYNAMIC))->Get_Info(INFO_POS, &TargetPos);
	m_vEye.x = TargetPos.x;
	m_vEye.z = TargetPos.z - 5;
	m_vAt = TargetPos;
	if (80.f > m_vEye.y)
		m_vEye.y += 50 * fTimeDelta;

	_int	iExit = CCamera::Update_Object(fTimeDelta);

	return iExit;
}

void CDroneCamera::LateUpdate_Object(void)
{
	CCamera::LateUpdate_Object();
}

void CDroneCamera::Reset_Pos(void)
{
	Engine::StopSound(DRONE_SOUND);
	Engine::PlaySound_SR(L"Drone.mp3", DRONE_SOUND, 0.2f);
	
	m_vEye.y = 0.f;
}

CDroneCamera * CDroneCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFov, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CDroneCamera*	pInstance = new CDroneCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		MSG_BOX("CDroneCamera Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CDroneCamera::Free(void)
{
	CGameObject::Free();
}
