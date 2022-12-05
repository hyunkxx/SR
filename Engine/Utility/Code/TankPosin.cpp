#include "..\..\Header\TankPosin.h"

USING(Engine)

CTankPosin::CTankPosin(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CTankPosin::CTankPosin(const CTankPosin & rhs)
	: CVIBuffer(rhs)
{
}

CTankPosin::~CTankPosin()
{
}

HRESULT CTankPosin::Ready_Buffer(void)
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
	// ���� ����

	pVertex[0].vPos = { -0.1f, 0.05f, 3.5f };
	pVertex[0].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVertex[1].vPos = { 0.1f, 0.05f, 3.5f };
	pVertex[1].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVertex[2].vPos = { 0.1f, 0.05f, 0.f };
	pVertex[2].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVertex[3].vPos = { -0.1f, 0.05f, 0.f };
	pVertex[3].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	// ���� �Ʒ���
	pVertex[4].vPos = { -0.1f, -0.05f, 3.5f };
	pVertex[4].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVertex[5].vPos = { 0.1f, -0.05f, 3.5f };
	pVertex[5].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVertex[6].vPos = { 0.1f, -0.05f, 0.f };
	pVertex[6].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVertex[7].vPos = { -0.1f, -0.05f, 0.f };
	pVertex[7].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	m_pVB->Unlock();
	INDEX32*		pIndex = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);
	
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

CTankPosin * CTankPosin::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTankPosin*		pInstance = new CTankPosin(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

CTankPosin * CTankPosin::Clone(void)
{
	return new CTankPosin(*this);
}

void CTankPosin::Free(void)
{
	__super::Free();
}
