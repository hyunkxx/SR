#include "stdafx.h"
#include "..\Header\ShootSmoke.h"

#include "Export_Function.h"

CShootSmoke::CShootSmoke(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CShootSmoke::CShootSmoke(const CShootSmoke & rhs)
	: CGameObject(rhs)
{
}

CShootSmoke::~CShootSmoke()
{
}

HRESULT CShootSmoke::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_bDead = true;
	m_fScale = 10.f;
	m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale);
	return S_OK;
}

_int CShootSmoke::Update_Object(const _float & fTimeDelta)
{
	m_fDeadTime += fTimeDelta;
	m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale);
	m_pTransformCom->Set_Pos(m_vPos.x, m_vPos.y + 8.f, m_vPos.z);

	if(!m_bFrameRepeat)
		m_fFrame += 10.f * fTimeDelta;

	else
		m_fFrame -= 10.f * fTimeDelta;

	if (3.f < m_fFrame)
	{
		m_fFrame -= 10.f * fTimeDelta;
		m_bFrameRepeat = true;
	}
	else if (0.f > m_fFrame)
	{
		m_fFrame += 10.f * fTimeDelta;
		m_bFrameRepeat = false;
	}

	if (5.f < m_fDeadTime)
		m_bDead = true;

	__super::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_ALPHA, this);

	return OBJ_NOEVENT;
}

void CShootSmoke::LateUpdate_Object(void)
{
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

void CShootSmoke::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(_ulong(m_fFrame));
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CShootSmoke::Effect_Setting(_vec3 Pos)
{
	m_pTransformCom->Reset_Trans();
	m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale);
	m_vPos = Pos;
	m_fFrame = 0.f;
	m_bDead = false;
	m_bFrameRepeat = false;

}

HRESULT CShootSmoke::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Prototype(L"Posin_Smoke"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"PosinSmoke", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;

}

CShootSmoke * CShootSmoke::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShootSmoke*		pInstance = new CShootSmoke(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CShootSmoke Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShootSmoke::Free(void)
{
	__super::Free();
}
