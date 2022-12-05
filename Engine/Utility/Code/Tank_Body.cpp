#include "..\..\Header\Tank_Body.h"


USING(Engine)

CTank_Body::CTank_Body(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CTank_Body::CTank_Body(const CTank_Body & rhs)
	: CVIBuffer(rhs)
{
}

CTank_Body::~CTank_Body()
{
}

HRESULT CTank_Body::Ready_Buffer(void)
{
	m_dwVtxCnt = 8;
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwTriCnt = 12;
	m_dwFVF = FVF_COL;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(__super::Ready_Buffer(), E_FAIL);
	
	VTXCOL*			pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	//��ü ���� ����
	pVertex[0].vPos = { -1.f, 0.5f, 1.5f };
	pVertex[0].dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);

	pVertex[1].vPos = { 1.f, 0.5f,  1.5f };
	pVertex[1].dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);

	pVertex[2].vPos = { 1.f, 0.5f, -1.5f };
	pVertex[2].dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);

	pVertex[3].vPos = { -1.f, 0.5f, -1.5f };
	pVertex[3].dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);
	
	// ��ü �Ʒ��� ����
	pVertex[4].vPos = { -1.f, -0.5f, 1.5f };
	pVertex[4].dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);

	pVertex[5].vPos = { 1.f, -0.5f,  1.5f };
	pVertex[5].dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);

	pVertex[6].vPos = { 1.f, -0.5f, -1.5f };
	pVertex[6].dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);

	pVertex[7].vPos = { -1.f, -0.5f, -1.5f };
	pVertex[7].dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);

	m_pVB->Unlock();

	INDEX32*		pIndex = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	//���
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	//����
	pIndex[2]._0 = 2;
	pIndex[2]._1 = 1;
	pIndex[2]._2 = 5;

	pIndex[3]._0 = 2;
	pIndex[3]._1 = 5;
	pIndex[3]._2 = 6;

	//�ĸ�
	pIndex[4]._0 = 3;
	pIndex[4]._1 = 2;
	pIndex[4]._2 = 6;

	pIndex[5]._0 = 3;
	pIndex[5]._1 = 6;
	pIndex[5]._2 = 7;

	// ����
	pIndex[6]._0 = 0;
	pIndex[6]._1 = 3;
	pIndex[6]._2 = 7;

	pIndex[7]._0 = 0;
	pIndex[7]._1 = 7;
	pIndex[7]._2 = 4;

	//����
	pIndex[8]._0 = 1;
	pIndex[8]._1 = 0;
	pIndex[8]._2 = 4;

	pIndex[9]._0 = 1;
	pIndex[9]._1 = 4;
	pIndex[9]._2 = 5;

	// �ϴ�
	pIndex[10]._0 = 5;
	pIndex[10]._1 = 4;
	pIndex[10]._2 = 7;

	pIndex[11]._0 = 5;
	pIndex[11]._1 = 7;
	pIndex[11]._2 = 6;

	m_pIB->Unlock();

	return S_OK;
}

CTank_Body * CTank_Body::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTank_Body*		pInstance = new CTank_Body(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

CTank_Body * CTank_Body::Clone(void)
{
	return new CTank_Body(*this);
}

void CTank_Body::Free(void)
{
	__super::Free();
}
