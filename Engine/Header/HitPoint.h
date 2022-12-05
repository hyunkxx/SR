#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CHitPoint :
	public CVIBuffer
{
private:
	explicit CHitPoint(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CHitPoint(const CHitPoint& rhs);
	virtual ~CHitPoint();

public:
	virtual HRESULT Ready_Buffer(void) override;

public:
	static CHitPoint*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CHitPoint*		Clone(void)override;
	virtual void			Free(void) override;
};

END

