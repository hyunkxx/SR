#include "stdafx.h"
#include "..\Header\StaticCamera.h"

#include "Export_Function.h"

CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}

CStaticCamera::CStaticCamera(const CStaticCamera& rhs)
	: CCamera(rhs)
	, m_fDistance(rhs.m_fDistance)
	, m_fSpeed(rhs.m_fSpeed)
	, m_fAngle(rhs.m_fAngle)

{

}

CStaticCamera::~CStaticCamera()
{
}

HRESULT CStaticCamera::Ready_Object(const _vec3* pEye,
										const _vec3* pAt,
										const _vec3* pUp, 
										const _float& fFov, 
										const _float& fAspect, 
										const _float& fNear, 
										const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt  = *pAt;
	m_vUp  = *pUp;

	m_fFov		= fFov;
	m_fAspect	= fAspect;
	m_fNear		= fNear;
	m_fFar		= fFar;


	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);
	return S_OK;
}

_int CStaticCamera::Update_Object(const _float& fTimeDelta)
{
	if (!m_bCameraOn)
		return 0;

	//Engine::Update_KeyMgr();
	Key_Input(fTimeDelta);
	
	Target_Renewal(fTimeDelta);
	Camera_Shake(fTimeDelta);

	_int	iExit = CCamera::Update_Object(fTimeDelta);
	return iExit;
}

void CStaticCamera::LateUpdate_Object(void)
{
	CCamera::LateUpdate_Object();
}

void CStaticCamera::Key_Input(const _float& fTimeDelta)
{
	if (Engine::Get_DIKeyState_Custom(DIK_Q) == KEY_STATE::HOLD)
		m_fDistance -= m_fSpeed * fTimeDelta;

	if (Engine::Get_DIKeyState_Custom(DIK_E) == KEY_STATE::HOLD)
		m_fDistance += m_fSpeed * fTimeDelta;


	if (Engine::Get_DIKeyState_Custom(DIK_W) == KEY_STATE::HOLD )
		m_fAngle += D3DXToRadian(180.f) * fTimeDelta;

	if (Engine::Get_DIKeyState_Custom(DIK_S) == KEY_STATE::HOLD)
		m_fAngle -= D3DXToRadian(180.f) * fTimeDelta;	
}

void CStaticCamera::Target_Renewal(const _float& fTimeDelta)
{
	CTransform*		pPlayerTransform = static_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"PlayerVehicle", L"Proto_TransformBody", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransform);

	/*_vec3	vLook;
	pPlayerTransform->Get_Info(INFO_LOOK, &vLook);

	m_vEye = vLook * -1.f;
	D3DXVec3Normalize(&m_vEye, &m_vEye);

	m_vEye.y = 1.f;
	m_vEye *= m_fDistance;

	_vec3		vRight;
	memcpy(&vRight, &pPlayerTransform->m_matWorld.m[0][0], sizeof(_vec3));

	_matrix			matRot;
	D3DXMatrixRotationAxis(&matRot, &vRight, m_fAngle);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRot);

	m_vEye += pPlayerTransform->m_vInfo[INFO_POS];	// À§Ä¡
	m_vAt   = pPlayerTransform->m_vInfo[INFO_POS];*/


	_vec3 m_Plus = { 0.f,40.f, -30.f };
	_vec3 UserPos;
	pPlayerTransform->Get_Info(INFO_POS, &UserPos);

	m_vPos.x = Get_Linear(m_vPos.x , UserPos.x , fTimeDelta);
	m_vPos.y = Get_Linear(m_vPos.y , UserPos.y , fTimeDelta);
	m_vPos.z = Get_Linear(m_vPos.z , UserPos.z , fTimeDelta);

	m_vEye = m_vPos + m_Plus;

	_matrix matCam;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCam);
	D3DXMatrixInverse(&matCam, 0, &matCam);
	_vec3 camLook = { matCam._31,matCam._32 ,matCam._33 };

	m_vAt = UserPos; //m_vPos + camLook;
}

CStaticCamera* CStaticCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov /*= D3DXToRadian(60.f)*/, const _float& fAspect /*= (_float)(WINCX) / WINCY*/, const _float& fNear /*= 0.1f*/, const _float& fFar /*= 1000.f*/)
{
	CStaticCamera*	pInstance = new CStaticCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{ 
		Safe_Release(pInstance);
		MSG_BOX("StaticCamera Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CStaticCamera::Free(void)
{
	CGameObject::Free();
}
