#include "stdafx.h"
#include "..\Header\Terrain.h"

#include "Export_Function.h"
#include "FloorTex.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev, TYPE eType)
	: Engine::CGameObject(pGraphicDev)
	, m_eType(eType)
{
}

CTerrain::CTerrain(const CTerrain& rhs)
	: Engine::CGameObject(rhs)
{

}

CTerrain::~CTerrain()
{
}

HRESULT CTerrain::Ready_Object(void)
{
	_matrix matScale, matRot, matTrans;
	D3DXMatrixIdentity(&m_matBackground);

	D3DXMatrixScaling(&matScale, 1000.f, 1000.f, 0.f);
	D3DXMatrixRotationX(&matRot, D3DXToRadian(90.f));
	D3DXMatrixTranslation(&matTrans, 0.f, -0.1f, 0.f);
	m_matBackground = matScale * matRot * matTrans;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CTerrain::Update_Object(const _float& fTimeDelta)
{
	//Key_Input(fTimeDelta);

	__super::Update_Object(fTimeDelta);

	return OBJ_NOEVENT;
}

void CTerrain::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_PRIORITY, this);
}

void CTerrain::Render_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();


	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matBackground);
	m_pBackground->Set_Texture(0);
	m_pBackgroundRect->Render_Buffer();
}

void CTerrain::Free(void)
{
	__super::Free();
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev, TYPE eType)
{
	CTerrain*		pInstance = new CTerrain(pGraphicDev, eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Terrain Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CTerrain::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CTerrainTex*>(Clone_Prototype(L"Proto_TerrainTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TerrainTex", pComponent });

	switch (m_eType)
	{
	case TYPE::SAND:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Prototype(L"Proto_TerrainSandTexture"));
		NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_Texture_1", pComponent });

		pComponent = m_pBackground = dynamic_cast<CTexture*>(Clone_Prototype(L"Proto_TerrainSandTexture"));
		NULL_CHECK_RETURN(m_pBackground, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_Texture_2", pComponent });

		break;
	case TYPE::GRASS:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Prototype(L"Proto_TerrainGrassTexture"));
		NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_Texture_1", pComponent });

		pComponent = m_pBackground = dynamic_cast<CTexture*>(Clone_Prototype(L"Proto_TerrainGrassTexture"));
		NULL_CHECK_RETURN(m_pBackground, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_Texture_2", pComponent });

		break;
	default:
		break;
	}

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pBackgroundRect = CFloorTex::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pBackgroundRect, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"backgroundTex", pComponent });

	return S_OK;
}

void CTerrain::Key_Input(const _float& fTimeDelta)
{
	_vec3	vDir;
	m_pTransformCom->Get_Info(INFO_UP, &vDir);

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->Move_Pos(&(vDir * m_fSpeed * fTimeDelta));
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->Move_Pos(&(vDir * -m_fSpeed * fTimeDelta));
	}

	if (GetAsyncKeyState('Q') & 0x8000)
		m_pTransformCom->Rotation(ROT_X, D3DXToRadian(180.f * fTimeDelta));

	if (GetAsyncKeyState('A') & 0x8000)
		m_pTransformCom->Rotation(ROT_X, D3DXToRadian(-180.f * fTimeDelta));

	if (GetAsyncKeyState('W') & 0x8000)
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));

	if (GetAsyncKeyState('S') & 0x8000)
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-180.f * fTimeDelta));

	if (GetAsyncKeyState('E') & 0x8000)
		m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(180.f * fTimeDelta));

	if (GetAsyncKeyState('D') & 0x8000)
		m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(-180.f * fTimeDelta));


}

HRESULT CTerrain::SetUp_Material(void)
{
	D3DMATERIAL9			tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	// 램버트 확산 조명공식
	// (빛의 세기 * 색상) * (Light.d * Mtrl.d) + (Light.a * Mtrl.a)

	tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrl.Power = 0.f;

	m_pGraphicDev->SetMaterial(&tMtrl);

	return S_OK;
}
