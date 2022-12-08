#include "..\..\Header\FloorTex.h"

USING(Engine)


CFloorTex::CFloorTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}


Engine::CFloorTex::CFloorTex(const CFloorTex& rhs)
	: CVIBuffer(rhs)
{

}

CFloorTex::~CFloorTex()
{
}

HRESULT CFloorTex::Ready_Buffer(void)
{
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwTriCnt = 2;
	m_dwFVF = FVF_TEX;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(__super::Ready_Buffer(), E_FAIL);

	VTXTEX*			pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0); // 3���� : ���ؽ� ������ ���� �� ù��° �ּ� ���� ����
											// ���� 
	pVertex[0].vPos = { -1.f, 1.f, 0.f };
	pVertex[0].vTexUV = { 0.f, 0.f };

	pVertex[1].vPos = { 1.f, 1.f, 0.f };
	pVertex[1].vTexUV = { 50.f, 0.f };

	pVertex[2].vPos = { 1.f, -1.f, 0.f };
	pVertex[2].vTexUV = { 50.f, 50.f };

	pVertex[3].vPos = { -1.f, -1.f, 0.f };
	pVertex[3].vTexUV = { 0.f , 50.f };

	m_pVB->Unlock();

	INDEX32*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

CFloorTex* Engine::CFloorTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFloorTex*		pInstance = new CFloorTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

CFloorTex* Engine::CFloorTex::Clone(void)
{
	return new CFloorTex(*this);
}

void CFloorTex::Free(void)
{
	__super::Free();
}

