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
	// 버텍스 버퍼 생성 함수
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt, // 정점 버퍼의 전체 사이즈(바이트 단위)
		0,					   // 버퍼 사용 방식(0을 넣을 경우 정적 버퍼를 사용) -> D3DUSAGE_DYNAMIC을 지정하는 경우 동적 버퍼
		m_dwFVF,				// 사용하는 정점 버퍼의 스타일을 지정하는 플래그
		D3DPOOL_MANAGED,		// 정적 버퍼 사용 시 메모리 풀은 MANAGED, 동적버퍼 사용 시 무조건  D3DPOOL_DEFAULT를 사용
		&m_pVB,					// 만들어지는 버텍스 버퍼 컴객체
		NULL)))					// 버퍼 만들때 필요한 예약 상태(NULL 넣으면 됨)
		return E_FAIL;


	// 버텍스 버퍼 생성 함수
	if (FAILED(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt, // 인덱스 버퍼의 전체 사이즈(바이트 단위)
		0,					   // 버퍼 사용 방식(0을 넣을 경우 정적 버퍼를 사용) -> D3DUSAGE_DYNAMIC을 지정하는 경우 동적 버퍼
		m_IdxFmt,				// 사용하는 정점 버퍼의 스타일을 지정하는 플래그
		D3DPOOL_MANAGED,		// 정적 버퍼 사용 시 메모리 풀은 MANAGED, 동적버퍼 사용 시 무조건  D3DPOOL_DEFAULT를 사용
		&m_pIB,					// 만들어지는 버텍스 버퍼 컴객체
		NULL)))					// 버퍼 만들때 필요한 예약 상태(NULL 넣으면 됨)
		return E_FAIL;


	return S_OK;
}

void CVIBuffer::Render_Buffer(void)
{
	// SetStreamSource : 버텍스 버퍼의 내용을 디바이스 장치와 링크시키는 함수

	// 1. 몇 번 슬롯(장치는 데이터를 보관하기 위한 슬롯이 존재)에 보관할 것인가
	// 2. 어떤 것을 넘겨줄 것인가
	// 3. 어디서부터 그릴 것인가, 0인 경우 버퍼의 처음부터 그리기
	// 4. 어떤 단위로 표현할 것인가

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);

	// 매 프레임마다 정점의 속성 정보를 갱신
	m_pGraphicDev->SetFVF(m_dwFVF);

	// 디바이스 장치와 인덱스 버퍼를 링크
	m_pGraphicDev->SetIndices(m_pIB);

	// 그리기 함수
	// 1. 어떤 도형을 그릴 것인가
	// 2. 0인 경우 몇 번째 버텍스부터 그릴 것인가
	// 3. 몇 개의 도형을 그릴 것인가
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
