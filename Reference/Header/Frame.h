#pragma once
#include "Base.h"
#include "Include.h"

BEGIN(Engine)

class ENGINE_DLL CFrame :public CBase
{
private:
	explicit CFrame();
	virtual ~CFrame();

public:
	_bool		IsPermit_Call(const _float& fTimeDelta);

public:
	HRESULT		Ready_Frame(const _float& fCallLimit);


private:
	_float		m_fCallLimit = 0.f;
	_float		m_fAccTimeDelta = 0.f;

public:
	static CFrame*		Create(const _float& fCallLimit);
	virtual void		Free(void) override;
};

END
