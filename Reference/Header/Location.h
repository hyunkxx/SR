#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CLocation : public CVIBuffer
{
private:
	explicit				CLocation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit				CLocation(const CLocation& rhs);
	virtual					~CLocation();

public:
	virtual HRESULT			Ready_Buffer(void) override;

public:
	static  CLocation*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CLocation*		Clone(void)override;
	virtual void			Free(void) override;

};

END