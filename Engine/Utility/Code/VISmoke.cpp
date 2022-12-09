#include "..\..\Header\VISmoke.h"

CVISmoke::CVISmoke(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CVISmoke::CVISmoke(const CVISmoke & rhs)
	: CVIBuffer(rhs)
{
}

CVISmoke::~CVISmoke()
{
}

HRESULT CVISmoke::Ready_Buffer(void)
{
	m_dwVtxCnt = 9;
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwTriCnt = 10;
	m_dwFVF = FVF_COL;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(__super::Ready_Buffer(), E_FAIL);

	VTXCOL*			pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	//몸체 위쪽 버퍼
	pVertex[0].vPos = { -0.1f, 0.1f, 0.f };
	pVertex[0].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	pVertex[1].vPos = { 0.1f, 0.1f, 0.f };
	pVertex[1].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	pVertex[2].vPos = { 0.1f, 0.1f, -0.5f };
	pVertex[2].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	pVertex[3].vPos = { -0.1f, 0.1f, -0.5f };
	pVertex[3].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	// 몸체 아래쪽 버퍼
	pVertex[4].vPos = { -0.1f, -0.1f, 0.f };
	pVertex[4].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	pVertex[5].vPos = { 0.1f, -0.1f, 0.f };
	pVertex[5].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	pVertex[6].vPos = { 0.1f, -0.1f, -0.5f };
	pVertex[6].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	pVertex[7].vPos = { -0.1f, -0.1f, -0.5f };
	pVertex[7].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//총알 대가리
	m_pVB->Unlock();

	INDEX32*		pIndex = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	//상단
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	//우측
	pIndex[2]._0 = 2;
	pIndex[2]._1 = 1;
	pIndex[2]._2 = 5;

	pIndex[3]._0 = 2;
	pIndex[3]._1 = 5;
	pIndex[3]._2 = 6;

	//후면
	pIndex[4]._0 = 3;
	pIndex[4]._1 = 2;
	pIndex[4]._2 = 6;

	pIndex[5]._0 = 3;
	pIndex[5]._1 = 6;
	pIndex[5]._2 = 7;

	// 좌측
	pIndex[6]._0 = 0;
	pIndex[6]._1 = 3;
	pIndex[6]._2 = 7;

	pIndex[7]._0 = 0;
	pIndex[7]._1 = 7;
	pIndex[7]._2 = 4;
	// 하단
	pIndex[8]._0 = 5;
	pIndex[8]._1 = 4;
	pIndex[8]._2 = 7;

	pIndex[9]._0 = 5;
	pIndex[9]._1 = 7;
	pIndex[9]._2 = 6;

	m_pIB->Unlock();

	return S_OK;
}

CVISmoke * CVISmoke::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CVISmoke*		pInstance = new CVISmoke(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

CVISmoke * CVISmoke::Clone(void)
{

	return new CVISmoke(*this);
}

void CVISmoke::Free(void)
{
	__super::Free();
}
