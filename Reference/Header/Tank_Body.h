#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL  CTank_Body :
	public CVIBuffer
{
private:
	explicit CTank_Body(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTank_Body(const CTank_Body& rhs);
	virtual ~CTank_Body();

public:
	virtual HRESULT Ready_Buffer(void) override;

public:
	static CTank_Body*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CTank_Body*		Clone(void)override;
	virtual void			Free(void) override;
};

END