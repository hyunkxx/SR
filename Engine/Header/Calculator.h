#pragma once
#include "Component.h"

BEGIN(Engine)

class CTerrainTex;
class CTransform;
class ENGINE_DLL CCalculator : public CComponent
{
public:
	explicit CCalculator(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCalculator(const CCalculator& rhs);
	virtual ~CCalculator();

public:
	HRESULT		Ready_Calculator(void);
	HRESULT		Compute_HeightOnTerrain(_vec3* pPos, const _vec3* pTerrainVtxPos, const _ulong& dwCntX = VTXCNTX, const _ulong& dwCntZ = VTXCNTZ, const _ulong& dwItv = VTXITV);
	_vec3		Picking_OnTerrain(HWND hWnd, const CTerrainTex* pTerrainVtxCom, const CTransform* pTerrainTransform);


public:
	static CCalculator*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*	Clone(void)override;
private:
	virtual void Free(void) override;
};



END
