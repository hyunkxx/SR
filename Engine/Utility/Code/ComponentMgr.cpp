#include "..\..\Header\ComponentMgr.h"


USING(Engine)
IMPLEMENT_SINGLETON(CComponentMgr)

CComponentMgr::CComponentMgr()
{
}

CComponentMgr::~CComponentMgr()
{
	Free();
}

HRESULT CComponentMgr::Ready_Prototype(const _tchar * pComponentTag, CComponent * pComponent)
{
	CComponent*		pPrototype = Find_Prototype(pComponentTag);

	if (nullptr != pPrototype || nullptr == pComponent)
		return E_FAIL;

	m_umapProto.insert({ pComponentTag, pComponent });

	return S_OK;
}

CComponent * CComponentMgr::Clone_Prototype(const _tchar * pComponentTag)
{
	CComponent*		pPrototype = Find_Prototype(pComponentTag);

	if (nullptr == pPrototype)
		return nullptr;
	
	return pPrototype->Clone();
}

CComponent * CComponentMgr::Find_Prototype(const _tchar * pComponentTag)
{
	auto	iter = find_if(m_umapProto.begin(), m_umapProto.end(), CTag_Finder(pComponentTag));

	if (iter == m_umapProto.end())
		return nullptr;

	return iter->second;
}

void CComponentMgr::Free()
{
	for_each(m_umapProto.begin(), m_umapProto.end(), CDeleteMap());
	m_umapProto.clear();
}
