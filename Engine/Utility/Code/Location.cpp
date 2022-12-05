#include "..\..\Header\Location.h"

USING(Engine)


CLocation::CLocation(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}


Engine::CLocation::CLocation(const CLocation& rhs)
	: CVIBuffer(rhs)
{

}

CLocation::~CLocation()
{
}

HRESULT CLocation::Ready_Buffer(void)
{
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwTriCnt = 2;
	m_dwFVF = FVF_TEX;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(__super::Ready_Buffer(), E_FAIL);

	VTXTEX*			pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0); // 3인자 : 버텍스 버퍼의 원소 중 첫번째 주소 값을 추출
											// 위쪽 
	pVertex[0].vPos = { -1.f, 0.f, 1.f };
	pVertex[0].vTexUV = { 0.f, 0.f };

	pVertex[1].vPos = { 1.f, 0.f, 1.f };
	pVertex[1].vTexUV = { 1.f, 0.f };

	pVertex[2].vPos = { 1.f, 0.f, -1.f };
	pVertex[2].vTexUV = { 1.f, 1.f };

	pVertex[3].vPos = { -1.f, 0.f, -1.f };
	pVertex[3].vTexUV = { 0.f ,1.f };

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

CLocation* Engine::CLocation::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLocation*		pInstance = new CLocation(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
		return pInstance;

	return pInstance;
}

CLocation* Engine::CLocation::Clone(void)
{
	return new CLocation(*this);
}

void CLocation::Free(void)
{
	__super::Free();
}

