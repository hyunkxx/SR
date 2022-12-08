#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CFloorTex : public CVIBuffer
{
private:
	explicit CFloorTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFloorTex(const CFloorTex& rhs);
	virtual ~CFloorTex();

public:
	virtual HRESULT Ready_Buffer(void) override;

public:
	static CFloorTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CFloorTex*	Clone(void)override;
	virtual void Free(void) override;

};

END