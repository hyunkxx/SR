#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex : public CVIBuffer
{
private:
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrainTex(const CTerrainTex& rhs);
	virtual ~CTerrainTex();

public:
	const _vec3*		Get_VtxPos() const { return m_pPos; }
	 _ulong		Get_VtxCntX() const { return m_iH.biWidth; }
	 _ulong		Get_VtxCntZ() const { return m_iH.biHeight; }

public:
	HRESULT Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv );

private:
	HANDLE		m_hFile;
	BITMAPFILEHEADER		m_fH;
	BITMAPINFOHEADER		m_iH;

	_vec3*				m_pPos;

public:
	static CTerrainTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
									const _ulong& dwCntX = VTXCNTX, 
									const _ulong& dwCntZ = VTXCNTZ,
									const _ulong& dwVtxItv = VTXITV);

	virtual CTerrainTex*	Clone(void)override;
	virtual void Free(void) override;

};

END