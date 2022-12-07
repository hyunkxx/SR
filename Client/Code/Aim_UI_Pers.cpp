#include "stdafx.h"
#include "..\Header\Aim_UI_Pers.h"

#include "Export_Function.h"

CAim_UI_Pers::CAim_UI_Pers(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CAim_UI_Pers::CAim_UI_Pers(const CAim_UI_Pers & rhs)
	: CGameObject(rhs)
{
}

CAim_UI_Pers::~CAim_UI_Pers()
{
}

HRESULT CAim_UI_Pers::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	CGameObject::Ready_Object();
	m_fScale = 10.f;
	m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale);
	return S_OK;
}

_int CAim_UI_Pers::Update_Object(const _float & fTimeDelta)
{
	Key_Input();

	return __super::Update_Object(fTimeDelta);
}

void CAim_UI_Pers::LateUpdate_Object(void)
{
	_matrix matWorld, view, matBill;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &view);
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = view._11;
	matBill._13 = view._13;
	matBill._31 = view._31;
	matBill._33 = view._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));

	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_NONALPHA, this);
}

void CAim_UI_Pers::Render_Object(void)
{

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	if (m_bTexture[0])
		m_pTextureCom_0->Set_Texture(0);
	else if (m_bTexture[1])
		m_pTextureCom_1->Set_Texture(0);
	else if (m_bTexture[2])
		m_pTextureCom_2->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

HRESULT CAim_UI_Pers::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom_0 = dynamic_cast<CTexture*>(Clone_Prototype(L"target_0"));
	NULL_CHECK_RETURN(m_pTextureCom_0, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"target_0", pComponent });


	pComponent = m_pTextureCom_1 = dynamic_cast<CTexture*>(Clone_Prototype(L"target_1"));
	NULL_CHECK_RETURN(m_pTextureCom_1, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"target_1", pComponent });

	pComponent = m_pTextureCom_2 = dynamic_cast<CTexture*>(Clone_Prototype(L"target_2"));
	NULL_CHECK_RETURN(m_pTextureCom_2, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"target_2", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CAim_UI_Pers::Key_Input(void)
{
	if (Engine::Get_DIKeyState_Custom(DIK_T) == KEY_STATE::TAP)
	{
		if (m_bTexture[0])
		{
			m_bTexture[0] = false;
			m_bTexture[1] = true;
		}
		else if (m_bTexture[1])
		{
			m_bTexture[1] = false;
			m_bTexture[2] = true;
		}
		else if (m_bTexture[2])
		{
			m_bTexture[2] = false;
			m_bTexture[0] = true;
		}
	}
}

CAim_UI_Pers * CAim_UI_Pers::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAim_UI_Pers*		pInstance = new CAim_UI_Pers(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CAimUI Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAim_UI_Pers::Free(void)
{
	__super::Free();
}
