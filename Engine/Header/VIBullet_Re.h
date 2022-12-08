#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBullet_Re :
	public CVIBuffer
{
public:
	explicit CVIBullet_Re(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBullet_Re(const CVIBullet_Re& rhs);
	virtual ~CVIBullet_Re();

public:
	virtual HRESULT Ready_Buffer(void) override;

public:
	static CVIBullet_Re*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CVIBullet_Re*	Clone(void)override;
	virtual void Free(void) override;

};

END
