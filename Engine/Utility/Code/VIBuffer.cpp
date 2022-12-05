#include "..\..\Header\VIBuffer.h"

USING(Engine)

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
	, m_dwVtxSize(0)
	, m_dwTriCnt(0)
	, m_dwFVF(0)
	, m_dwVtxCnt(0)
	, m_dwIdxSize(0)
{
}


Engine::CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_dwVtxSize(rhs.m_dwVtxSize)
	, m_dwTriCnt(rhs.m_dwTriCnt)
	, m_dwFVF(rhs.m_dwFVF)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_pIB(rhs.m_pIB)
	, m_dwIdxSize(rhs.m_dwIdxSize)
	, m_IdxFmt(rhs.m_IdxFmt)
{
	m_pVB->AddRef();
	m_pIB->AddRef();
}

CVIBuffer::~CVIBuffer()
{
}

HRESULT Engine::CVIBuffer::Ready_Buffer(void)
{
	// ���ؽ� ���� ���� �Լ�
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt, // ���� ������ ��ü ������(����Ʈ ����)
		0,					   // ���� ��� ���(0�� ���� ��� ���� ���۸� ���) -> D3DUSAGE_DYNAMIC�� �����ϴ� ��� ���� ����
		m_dwFVF,				// ����ϴ� ���� ������ ��Ÿ���� �����ϴ� �÷���
		D3DPOOL_MANAGED,		// ���� ���� ��� �� �޸� Ǯ�� MANAGED, �������� ��� �� ������  D3DPOOL_DEFAULT�� ���
		&m_pVB,					// ��������� ���ؽ� ���� �İ�ü
		NULL)))					// ���� ���鶧 �ʿ��� ���� ����(NULL ������ ��)
		return E_FAIL;


	// ���ؽ� ���� ���� �Լ�
	if (FAILED(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt, // �ε��� ������ ��ü ������(����Ʈ ����)
		0,					   // ���� ��� ���(0�� ���� ��� ���� ���۸� ���) -> D3DUSAGE_DYNAMIC�� �����ϴ� ��� ���� ����
		m_IdxFmt,				// ����ϴ� ���� ������ ��Ÿ���� �����ϴ� �÷���
		D3DPOOL_MANAGED,		// ���� ���� ��� �� �޸� Ǯ�� MANAGED, �������� ��� �� ������  D3DPOOL_DEFAULT�� ���
		&m_pIB,					// ��������� ���ؽ� ���� �İ�ü
		NULL)))					// ���� ���鶧 �ʿ��� ���� ����(NULL ������ ��)
		return E_FAIL;


	return S_OK;
}

void CVIBuffer::Render_Buffer(void)
{
	// SetStreamSource : ���ؽ� ������ ������ ����̽� ��ġ�� ��ũ��Ű�� �Լ�

	// 1. �� �� ����(��ġ�� �����͸� �����ϱ� ���� ������ ����)�� ������ ���ΰ�
	// 2. � ���� �Ѱ��� ���ΰ�
	// 3. ��𼭺��� �׸� ���ΰ�, 0�� ��� ������ ó������ �׸���
	// 4. � ������ ǥ���� ���ΰ�

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);

	// �� �����Ӹ��� ������ �Ӽ� ������ ����
	m_pGraphicDev->SetFVF(m_dwFVF);

	// ����̽� ��ġ�� �ε��� ���۸� ��ũ
	m_pGraphicDev->SetIndices(m_pIB);

	// �׸��� �Լ�
	// 1. � ������ �׸� ���ΰ�
	// 2. 0�� ��� �� ��° ���ؽ����� �׸� ���ΰ�
	// 3. �� ���� ������ �׸� ���ΰ�
	//m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_dwTriCnt);

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
	m_pGraphicDev->SetTexture(0, nullptr);
}

void Engine::CVIBuffer::Free(void)
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	__super::Free();

}
