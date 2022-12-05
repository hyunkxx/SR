#include "..\..\Header\Cube.h"

USING(Engine)

CCube::CCube(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
	D3DXMatrixIdentity(&m_matWorld);
	ZeroMemory(&m_tagData, sizeof(CUBE_DATA));
}

CCube::CCube(const CCube& rhs)
	: CVIBuffer(rhs)
{
	D3DXMatrixIdentity(&m_matWorld);
	ZeroMemory(&m_tagData, sizeof(CUBE_DATA));
}

CCube::~CCube()
{
}

_int CCube::Update_Component(const _float & fTimeDelta)
{

	return 0;
}

CComponent* CCube::Clone(void)
{
	return new CCube(*this);
}

void CCube::Free(void)
{
	__super::Free();
}

CCube* CCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCube* pInstance = new CCube(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		MSG_BOX("Failed CCube::Create() - Ready_Buffer()");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCube::SetColor(CCube * pCube, D3DXCOLOR color)
{
	Vertex*	pVertex = nullptr;
	pCube->m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (int i = 0; i < 8; ++i)
		pVertex[i].color = color;

	pCube->m_pVB->Unlock();
}

void CCube::SetColor(D3DXCOLOR color)
{
	Vertex*	pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (int i = 0; i < 8; ++i)
		pVertex[i].color = color;

	m_pVB->Unlock();
}

void CCube::Initalize(CUBE_DATA refData)
{
	m_tagData.scale = refData.scale;
	m_tagData.rotation = refData.rotation;
	m_tagData.position = refData.position;
	m_tagData.originColor = refData.originColor;

	SetColor(this, m_tagData.originColor);
}

HRESULT CCube::Ready_Buffer(void)
{
	m_dwVtxCnt = 8;
	m_dwVtxSize = sizeof(Vertex);
	m_dwTriCnt = 12;
	m_dwFVF = FVF_VOX;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(__super::Ready_Buffer()))
		return E_FAIL;

	Vertex*	pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].pos = { -1.f, 1.f, -1.f };
	pVertex[0].color = D3DCOLOR_RGBA(255, 0, 0, 255);

	pVertex[1].pos = { 1.f, 1.f,-1.f };
	pVertex[1].color = D3DCOLOR_RGBA(0, 255, 0, 255);

	pVertex[2].pos = { 1.f, -1.f, -1.f };
	pVertex[2].color = D3DCOLOR_RGBA(0, 0, 255, 255);

	pVertex[3].pos = { -1.f, -1.f, -1.f };
	pVertex[3].color = D3DCOLOR_RGBA(255, 0, 0, 255);

	pVertex[4].pos = { -1.f, 1.f, 1.f };
	pVertex[4].color = D3DCOLOR_RGBA(0, 255, 0, 255);

	pVertex[5].pos = { 1.f, 1.f, 1.f };
	pVertex[5].color = D3DCOLOR_RGBA(0, 0, 255, 255);

	pVertex[6].pos = { 1.f, -1.f, 1.f };
	pVertex[6].color = D3DCOLOR_RGBA(255, 0, 0, 255);

	pVertex[7].pos = { -1.f, -1.f, 1.f };
	pVertex[7].color = D3DCOLOR_RGBA(0, 255, 0, 255);

	m_pVB->Unlock();

	Index32* pIndex = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// x+
	pIndex[0]._0 = 1;
	pIndex[0]._1 = 5;
	pIndex[0]._2 = 6;

	pIndex[1]._0 = 1;
	pIndex[1]._1 = 6;
	pIndex[1]._2 = 2;

	// x-
	pIndex[2]._0 = 4;
	pIndex[2]._1 = 0;
	pIndex[2]._2 = 3;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 3;
	pIndex[3]._2 = 7;

	// y+
	pIndex[4]._0 = 4;
	pIndex[4]._1 = 5;
	pIndex[4]._2 = 1;

	pIndex[5]._0 = 4;
	pIndex[5]._1 = 1;
	pIndex[5]._2 = 0;

	// y-
	pIndex[6]._0 = 3;
	pIndex[6]._1 = 2;
	pIndex[6]._2 = 6;

	pIndex[7]._0 = 3;
	pIndex[7]._1 = 6;
	pIndex[7]._2 = 7;

	// z+
	pIndex[8]._0 = 7;
	pIndex[8]._1 = 6;
	pIndex[8]._2 = 5;


	pIndex[9]._0 = 7;
	pIndex[9]._1 = 5;
	pIndex[9]._2 = 4;

	// z-
	pIndex[10]._0 = 0;
	pIndex[10]._1 = 1;
	pIndex[10]._2 = 2;

	pIndex[11]._0 = 0;
	pIndex[11]._1 = 2;
	pIndex[11]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}