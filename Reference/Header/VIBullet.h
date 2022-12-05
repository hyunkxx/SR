#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBullet :
	public CVIBuffer
{
public:
	explicit CVIBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBullet(const CVIBullet& rhs);
	virtual ~CVIBullet();

public:
	virtual HRESULT Ready_Buffer(void) override;

public:
	static CVIBullet*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CVIBullet*	Clone(void)override;
	virtual void Free(void) override;

};

END

