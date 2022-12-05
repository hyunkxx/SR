#include "..\..\Header\FrameMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFrameMgr)

CFrameMgr::CFrameMgr()
{
}

CFrameMgr::~CFrameMgr()
{
	Free();
}

HRESULT CFrameMgr::Ready_Frame(const _tchar * pFrameTag, const _float& fTimeDelta)
{
	CFrame*	pFrame = Find_Frame(pFrameTag);
	if (nullptr != pFrame)
		return E_FAIL;

	pFrame = CFrame::Create(fTimeDelta);
	NULL_CHECK_RETURN(pFrame, E_FAIL);

	m_umapFrame.insert({ pFrameTag, pFrame });

	return S_OK;
}

_bool CFrameMgr::IsPermit_Call(const _tchar * pFrameTag, const _float & fTimeDelta)
{
	CFrame*	pFrame = Find_Frame(pFrameTag);
	NULL_CHECK_RETURN(pFrame, false);
	
	return pFrame->IsPermit_Call(fTimeDelta);
}

CFrame * CFrameMgr::Find_Frame(const _tchar * pFrameTag)
{
	auto		iter = find_if(m_umapFrame.begin(), m_umapFrame.end(), CTag_Finder(pFrameTag));

	if (iter == m_umapFrame.end())
		return nullptr;

	return iter->second;
}

void CFrameMgr::Free(void)
{
	for_each(m_umapFrame.begin(), m_umapFrame.end(), CDeleteMap());
	m_umapFrame.clear();
}
