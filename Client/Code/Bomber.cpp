#include "stdafx.h"
#include "..\Header\Bomber.h"

#include "Export_Function.h"

CBomber::CBomber(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CBomber::CBomber(const CBomber & rhs)
	:CGameObject(rhs)
{
}

CBomber::~CBomber()
{
	
}

_int CBomber::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return 0;

	m_vMileage += m_vDir * m_fSpeed * fTimeDelta;
	m_pTransformBody->Move_Pos(&(m_vDir * m_fSpeed * fTimeDelta));
	if (sqrtf((m_vMileage.x*m_vMileage.x) + (m_vMileage.z*m_vMileage.z)) > 2000.f)
		m_bDead = true;

	Add_RenderGroup(RENDER_NONALPHA, this);

	return CGameObject::Update_Object(fTimeDelta);
}

void CBomber::LateUpdate_Object(void)
{
	if (m_bDead)
		return;

	return CGameObject::LateUpdate_Object();
}

void CBomber::Render_Object(void)
{
	if (m_bDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformBody->Get_WorldMatrix());
	m_pBody->Render(m_pTransformBody->Get_WorldMatrix());

}

void CBomber::RenderGUI(void)
{
}

HRESULT CBomber::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pTransformBody = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformBody, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Bullet_Transform", pComponent });

	pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"air_ally_body");
	NULL_CHECK_RETURN(m_pBody, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelBody", pComponent });

	return S_OK;
}

HRESULT CBomber::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

CBomber * CBomber::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBomber* pInstance = new CBomber(pGraphicDev);
	
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CBomber::Free(void)
{
	CGameObject::Free();
}

void CBomber::Strike(_vec3 _Strike_Pos)
{
	m_vStrike_Pos = _Strike_Pos;
	m_pTransformBody->Reset_Trans();
	m_pTransformBody->Set_Pos(-100.f, 1.f, -100.f);
	m_vDir = _Strike_Pos - _vec3(-100.f, 1.f, -100.f);
	m_vDir.y = 0.f;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vMileage = _vec3(0.f, 0.f, 0.f);
	_vec3 Right(1.f, 0.f, 0.f);
	_float Scalar = D3DXVec3Dot(&m_vDir, &Right);
	
	Scalar = acosf(Scalar);

	if (m_vDir.z < Right.z)
		Scalar *= -1;

	m_pTransformBody->Rotation(ROT_Y,Scalar);
	m_bDead = false;
	m_fScale = 5.f;
	m_pTransformBody->Set_Scale(m_fScale, m_fScale, m_fScale);
}
