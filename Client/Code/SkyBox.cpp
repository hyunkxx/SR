#include "stdafx.h"
#include "..\Header\SkyBox.h"

#include "Export_Function.h"

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CSkyBox::CSkyBox(const CSkyBox& rhs)
	: Engine::CGameObject(rhs)
{

}

CSkyBox::~CSkyBox()
{
}

HRESULT CSkyBox::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(300.f, 100.f, 300.f);

	return S_OK;
}

_int CSkyBox::Update_Object(const _float& fTimeDelta)
{
	__super::Update_Object(fTimeDelta);

	_matrix	matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matView, NULL, &matView);
	m_pTransformCom->Set_Pos(matView._41, matView._42, matView._43);


	Add_RenderGroup(RENDER_PRIORITY, this);

	return OBJ_NOEVENT;
}

void CSkyBox::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CSkyBox::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CSkyBox::Free(void)
{
	__super::Free();
}

CSkyBox* CSkyBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkyBox*		pInstance = new CSkyBox(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("SkyBox Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CSkyBox::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Clone_Prototype(L"Proto_CubeTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Prototype(L"Proto_CubeTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTexture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}


