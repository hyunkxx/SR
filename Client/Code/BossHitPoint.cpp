#include "stdafx.h"
#include "..\Header\BossHitPoint.h"

#include "Export_Function.h"

CBossHitPoint::CBossHitPoint(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CBossHitPoint::CBossHitPoint(const CBossHitPoint & rhs)
	:CGameObject(rhs)
{
}

CBossHitPoint::~CBossHitPoint()
{
}

HRESULT CBossHitPoint::Ready_Object(void)
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformHitPoint->Set_Scale(20.f, 20.f, 20.f);
	m_pTransformHitPoint->Rotation(ROT_X, D3DXToRadian(90.f));

	return S_OK;
}

_int CBossHitPoint::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_pRender_Count += fTimeDelta;

	if (m_pRender_Time < m_pRender_Count)
		m_bDead = true;

	m_pTransformHitPoint->Set_Scale(20.f, 20.f, 20.f);

	if (D3DXToRadian(720.f) <= m_pTransformHitPoint->Get_Angle(ROT_Y))
		m_pTransformHitPoint->Rotation(ROT_Y, -m_pTransformHitPoint->Get_Angle(ROT_Y));

	m_pTransformHitPoint->Rotation(ROT_Y,D3DXToRadian(90.f * fTimeDelta));

	Add_RenderGroup(RENDER_ALPHA, this);
	return __super::Update_Object(fTimeDelta);

}

void CBossHitPoint::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CBossHitPoint::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformHitPoint->Get_WorldMatrix());
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();
}

void CBossHitPoint::Hit_Point_Set(_vec3 Pos, _float Time)
{
	m_pTransformHitPoint->Set_Pos(Pos.x, Pos.y, Pos.z);
	m_pRender_Time = Time;
	m_pRender_Count = 0.f;
	m_bDead = false;
}

HRESULT CBossHitPoint::Add_Component(void)
{
	//HitPoint
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformHitPoint = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformHitPoint, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformHitPoint", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(Clone_Prototype(L"target_0"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"target_0", pComponent });

	pComponent = m_pBufferCom = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_RcTex", pComponent });

	return S_OK;
}


CBossHitPoint * CBossHitPoint::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossHitPoint*		pInstance = new CBossHitPoint(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("BossHitPoint Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CBossHitPoint::Free(void)
{
	CGameObject::Free();
}
