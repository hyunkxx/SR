#include "stdafx.h"
#include "..\Header\BoomEffect.h"

#include "Export_Function.h"
#include "TankSet.h"
CBoomEffect::CBoomEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CBoomEffect::CBoomEffect(const CBoomEffect & rhs)
	: CGameObject(rhs)
{
}

CBoomEffect::~CBoomEffect()
{
}

HRESULT CBoomEffect::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_bDead = true;
	m_fScale = 70.f;
	m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale);
	return S_OK;
	Get_Dead();
}

_int CBoomEffect::Update_Object(const _float & fTimeDelta)
{

	if (m_bDead)
		return OBJ_NOEVENT;

	m_pTransformCom->Set_Pos(m_vPos.x, (m_vPos.y + 30), m_vPos.z);

	m_fFrame += 12.f * fTimeDelta;

	if (m_fFrame > 33.f)
	{
		static_cast<CTankSet*>(Engine::Get_Object(L"GameLogic", L"PlayerVehicle"))->Set_Rock(false);
		m_bDead = true;
		m_fFrame = 0.f;
	}

	__super::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_ALPHA, this);

	return OBJ_NOEVENT;
}

void CBoomEffect::LateUpdate_Object(void)
{
	if (m_bDead)
	{
		Engine::Get_Camera()->Set_Rock(false);
		return;
	}
	Engine::Get_Camera()->Set_Rock(true);
	
	_vec3	vTemp;
	m_pTransformCom->Get_Info(INFO_POS, &vTemp);

	__super::Compute_ViewZ(&vTemp);

	_matrix		matWorld, matView, matBill;
	m_pTransformCom->Get_WorldMatrix(&matWorld);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));

	__super::LateUpdate_Object();
}

void CBoomEffect::Render_Object(void)
{
	if (m_bDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(_ulong(m_fFrame));
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CBoomEffect::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Prototype(L"Proto_Boom"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Boom2", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

CBoomEffect * CBoomEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoomEffect*		pInstance = new CBoomEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CBoomEffect Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoomEffect::Free(void)
{
	__super::Free();
}