#include "stdafx.h"
#include "..\Header\AH_64A_Shoot_Effect.h"

#include "Export_Function.h"
#include "AH_64A.h"
CAH_64A_Shoot_Effect::CAH_64A_Shoot_Effect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CAH_64A_Shoot_Effect::CAH_64A_Shoot_Effect(const CAH_64A_Shoot_Effect & rhs)
	:CGameObject(rhs)
{
}

CAH_64A_Shoot_Effect::~CAH_64A_Shoot_Effect()
{
}

HRESULT CAH_64A_Shoot_Effect::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_bDead = true;
	m_fScale = 5.f;
	m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale);
	return S_OK;
	Get_Dead();
}

_int CAH_64A_Shoot_Effect::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_NOEVENT;

	if (m_bLeft)
	{
		_vec3 vPos = static_cast<CAH_64A*>(Engine::Get_Object(L"GameLogic", L"AH_64A"))->Get_vLeft_Gun_Pos();
		_vec3 vLook = static_cast<CAH_64A*>(Engine::Get_Object(L"GameLogic", L"AH_64A"))->Get_Info(INFO_LOOK);
		_vec3 vUp = static_cast<CAH_64A*>(Engine::Get_Object(L"GameLogic", L"AH_64A"))->Get_Info(INFO_UP);
		vPos.y += 2.f;
		vPos += vLook * 4.f;
		vPos -= vUp * 3.f;
		m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	}
	else
	{
		_vec3 vPos =  static_cast<CAH_64A*>(Engine::Get_Object(L"GameLogic", L"AH_64A"))->Get_vRight_Gun_Pos();
		_vec3 vLook = static_cast<CAH_64A*>(Engine::Get_Object(L"GameLogic", L"AH_64A"))->Get_Info(INFO_LOOK);
		_vec3 vUp = static_cast<CAH_64A*>(Engine::Get_Object(L"GameLogic", L"AH_64A"))->Get_Info(INFO_UP);

		vPos.y += 2.f;
		vPos += vLook * 4.f;
		vPos -= vUp * 3.f;
		m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
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

void CAH_64A_Shoot_Effect::LateUpdate_Object(void)
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

void CAH_64A_Shoot_Effect::Render_Object(void)
{
	if (m_bDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(_ulong(m_fFrame));
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CAH_64A_Shoot_Effect::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Prototype(L"Proto_Gun_ShootEffectTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_AH_64A_ShootEffectTexture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

CAH_64A_Shoot_Effect * CAH_64A_Shoot_Effect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{

	CAH_64A_Shoot_Effect*		pInstance = new CAH_64A_Shoot_Effect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CAH_64A_Shoot_Effect Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAH_64A_Shoot_Effect::Free(void)
{
	__super::Free();
}
