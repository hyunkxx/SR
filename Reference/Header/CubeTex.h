#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeTex : public CVIBuffer
{
private:
	explicit CCubeTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCubeTex(const CCubeTex& rhs);
	virtual ~CCubeTex();

public:
	virtual HRESULT Ready_Buffer(void) override;

public:
	static CCubeTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CCubeTex*	Clone(void)override;
	virtual void Free(void) override;

};

END