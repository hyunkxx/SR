#include "stdafx.h"
#include "..\Header\Gun_Shoot_Effect.h"

#include "Export_Function.h"
CGun_Shoot_Effect::CGun_Shoot_Effect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CGun_Shoot_Effect::CGun_Shoot_Effect(const CGun_Shoot_Effect & rhs)
	:CGameObject(rhs)
{
}

CGun_Shoot_Effect::~CGun_Shoot_Effect()
{
}

HRESULT CGun_Shoot_Effect::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_bDead = true;
	m_fScale = 5.f;
	m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale);
	return S_OK;
	Get_Dead();
}

_int CGun_Shoot_Effect::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
	{
		CTransform*		pPlayerTransform = static_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"PlayerVehicle", L"Proto_TransformPosin", ID_DYNAMIC));
		_vec3 Pos, Look;

		pPlayerTransform->Get_Info(INFO_POS, &Pos);
		pPlayerTransform->Get_Info(INFO_LOOK, &Look);
		D3DXVec3Normalize(&Look, &Look);
		Pos.y += 2.f;
		Pos += Look * 4.f;
		m_pTransformCom->Set_Pos(Pos.x, Pos.y, Pos.z);
		return OBJ_NOEVENT;
	}



	m_fFrame += 90.f * fTimeDelta;

	if (m_fFrame > 8.f)
	{
		m_bDead = true;
		m_fFrame = 0.f;
	}


	__super::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_ALPHA, this);

	return OBJ_NOEVENT;
}

void CGun_Shoot_Effect::LateUpdate_Object(void)
{
	if (m_bDead)
		return;

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

void CGun_Shoot_Effect::Render_Object(void)
{
	if (m_bDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(_ulong(m_fFrame));
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CGun_Shoot_Effect::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Prototype(L"Proto_Gun_ShootEffectTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Gun_ShootEffectTexture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

CGun_Shoot_Effect * CGun_Shoot_Effect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGun_Shoot_Effect*		pInstance = new CGun_Shoot_Effect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Effect Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGun_Shoot_Effect::Free(void)
{
	__super::Free();
}
