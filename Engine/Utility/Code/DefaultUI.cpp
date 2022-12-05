#include "..\..\Header\DefaultUI.h"

#include "Export_Function.h"
#include "Transform.h"

USING(Engine)

CDefaultUI* CDefaultUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* strName, const _tchar* strPath)
{
	CDefaultUI*	pInstance = new CDefaultUI(pGraphicDev, strName, strPath);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CDefaultUI Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDefaultUI::Free(void)
{
	__super::Free();
}

CDefaultUI::CDefaultUI(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* strName, const _tchar* strPath)
	: CGameObject(pGraphicDev)
	, m_vPos(0.f, 0.f, 0.01f)
	, m_vScale(100.f, 100.f, 1.f)
	, m_strName(strName)
	, m_strPath(strPath)
{
}

CDefaultUI::~CDefaultUI()
{
}

HRESULT CDefaultUI::Ready_Object(void)
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_pTransform->Set_Scale(m_vScale.x, m_vScale.y, m_vScale.z);
	m_pTransform->Set_Pos(m_vPos.x - (WINCX * 0.5f), (WINCY * 0.5f) - m_vPos.y, m_vPos.z);

	return S_OK;
}

_int CDefaultUI::Update_Object(const _float & fTimeDelta)
{
	__super::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);
	return 0;
}

void CDefaultUI::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CDefaultUI::Render_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pTransform->Set_Scale(m_vScale.x, m_vScale.y, m_vScale.z);
	m_pTransform->Set_Pos(m_vPos.x - (WINCX * 0.5f), (WINCY * 0.5f) - m_vPos.y, m_vPos.z);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());

	_matrix	ViewMatrix;
	D3DXMatrixIdentity(&ViewMatrix);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pTexture->Set_Texture(0);
	m_pRectTexture->Render_Buffer();


	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CDefaultUI::RenderGUI(void)
{
}

HRESULT CDefaultUI::AddComponent()
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRectTexture = CRcTex::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pRectTexture, E_FAIL);

	wstring RectTextureKey = m_strName + L"RectTexture";
	m_mapComponent[ID_STATIC].insert({ RectTextureKey.c_str(), pComponent });

	wstring TextureKey = m_strName + L"TextureKey";
	pComponent = m_pTexture = CTexture::Create(m_pGraphicDev, m_strPath.c_str(), TEX_NORMAL);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TextureKey.c_str(), pComponent });

	wstring TransformKey = m_strName + L"Transform";
	pComponent = m_pTransform = CTransform::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TransformKey.c_str(), pComponent });

	return S_OK;
}
