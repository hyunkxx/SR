#include "stdafx.h"
#include "..\Header\Cannonball.h"

#include "Export_Function.h"

CCannonball::CCannonball(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBullet(pGraphicDev)
{
	m_eID = BULLET_ID::CANNONBALL;
}

CCannonball::CCannonball(const CCannonball & rhs)
	:CBullet(rhs)
{
}

CCannonball::~CCannonball()
{
	Free();
}

HRESULT CCannonball::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	return S_OK;
}

_int CCannonball::Update_Object(const _float & fTimeDelta)
{
	m_fAccum += fTimeDelta;
	m_pTransform->Set_Scale(10.f, 10.f, 10.f);

	_vec3 Move, Dir1, Dir2;
	Move.x = m_vDir.x * m_fSpeed * fTimeDelta * cosf(-m_fAngleX);
	Move.z = m_vDir.z * m_fSpeed * fTimeDelta * cosf(-m_fAngleX);
	Move.y = ((m_vDir.y * m_fSpeed * fTimeDelta) * sinf(-m_fAngleX) - (0.5f * 9.8f * (m_fAccum * m_fAccum)));

	Dir2 = Dir1 = Move;
	Dir1.y = 0.f;
	D3DXVec3Normalize(&Dir1, &Dir1);
	D3DXVec3Normalize(&Dir2, &Dir2);
	_float Scalar = D3DXVec3Dot(&Dir2, &Dir1);
	Scalar = acosf(Scalar);
	if (Dir2.y < Dir1.y)
		Scalar *= -1;

	m_pTransform->Rotation(ROT_X, -(m_pTransform->Get_Angle(ROT_X)) - Scalar);

	m_pTransform->Move_Pos(&Move);
	//m_pTransform->Move_Pos();
	CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}
 
void CCannonball::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();

	m_pTransform->Get_Info(INFO_POS, &m_vPos);

	if (m_vPos.y < -1.f)
		m_bDead = true;

}

void CCannonball::Render_Object(void)
{
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());
	m_pBUlletBuffer->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

HRESULT CCannonball::Add_Component(void)
{

	CComponent*		pComponent = nullptr;

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Bullet_Transform", pComponent });

	pComponent = m_pBUlletBuffer = static_cast<CVIBullet*>(Clone_Prototype(L"Proto_VIBullet"));
	NULL_CHECK_RETURN(m_pBUlletBuffer, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_VIBullet", pComponent });

	return S_OK;
}

CCannonball * CCannonball::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCannonball*	pInstance = new CCannonball(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CCannonball Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CCannonball::Bullet_Setting(_vec3 vPos, _vec3 vDir, const _float fSpeed, _float fAngleX, _float fAngleY)
{
	m_pTransform->Reset_Trans();
	m_vPos = vPos;
	m_vDir = vDir;
	m_fSpeed = fSpeed;
	m_fAccum = 0.f;
	m_bDead = false;
	m_fAngleX = fAngleX;
	m_fAngleY = fAngleY;
	m_pTransform->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);
	m_pTransform->Rotation(ROT_Y, fAngleY);
}
void CCannonball::Free(void)
{
	__super::Free();
}
