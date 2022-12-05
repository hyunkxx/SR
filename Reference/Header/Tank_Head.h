#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTank_Head :
	public CVIBuffer
{
private:
	explicit CTank_Head(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTank_Head(const CTank_Head& rhs);
	virtual ~CTank_Head();

public:
	virtual HRESULT Ready_Buffer(void) override;

public:
	static CTank_Head*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CTank_Head*		Clone(void)override;
	virtual void			Free(void) override;
};

END