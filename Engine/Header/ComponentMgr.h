#pragma once

#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CComponentMgr : public CBase
{
	DECLARE_SINGLETON(CComponentMgr)

private:
	explicit CComponentMgr();
	virtual ~CComponentMgr();

public:
	HRESULT			Ready_Prototype(const _tchar* pComponentTag, CComponent* pComponent);
	CComponent*		Clone_Prototype(const _tchar* pComponentTag);

private:
	CComponent*		Find_Prototype(const _tchar* pComponentTag);

private:
	unordered_map<const _tchar*, CComponent*>		m_umapProto;

public:
	virtual void	Free()override;
};

END
