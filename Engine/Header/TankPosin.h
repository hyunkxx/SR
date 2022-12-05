#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTankPosin :
	public CVIBuffer
{
private:
	explicit CTankPosin(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTankPosin(const CTankPosin& rhs);
	virtual ~CTankPosin();

public:
	virtual HRESULT Ready_Buffer(void) override;

public:
	static CTankPosin*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CTankPosin*		Clone(void)override;
	virtual void			Free(void) override;
};

END

