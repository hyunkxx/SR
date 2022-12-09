#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVISmoke :
	public CVIBuffer
{
public:
	explicit CVISmoke(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVISmoke(const CVISmoke& rhs);
	virtual ~CVISmoke();

public:
	virtual HRESULT Ready_Buffer(void) override;

public:
	static CVISmoke*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CVISmoke*	Clone(void)override;
	virtual void Free(void) override;

};

END