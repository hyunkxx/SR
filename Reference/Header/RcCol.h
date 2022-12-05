#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcCol : public CVIBuffer
{
private:
	explicit CRcCol(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcCol(const CRcCol& rhs);
	virtual ~CRcCol();

public:
	virtual HRESULT Ready_Buffer(void) override;

public:
	static CRcCol*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CRcCol*	Clone(void)override;
	virtual void Free(void) override;

};

END