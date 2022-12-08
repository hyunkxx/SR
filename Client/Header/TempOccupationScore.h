#pragma once
#include"GameObject.h"
BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CTempOccupationScore : public CGameObject
{
private:
	explicit CTempOccupationScore(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTempOccupationScore(const CTempOccupationScore& rhs);
	virtual ~CTempOccupationScore();

public:
	virtual HRESULT Ready_Object(void) override;
	HRESULT Ready_Object(void* pArg);
	virtual _int Update_Object(const _float& fTimeDelta)override;
	virtual void LateUpdate_Object(void)  override;
	virtual void Render_Object(void)  override;

private:
	HRESULT		Add_Component(void);

private:
	//Left,Right,Lefttop,Righttop
	CRcTex*					m_pBufferLeftCom = nullptr;
	CRcTex*					m_pBufferRightCom = nullptr;
	CRcTex*					m_pBufferLefttopCom = nullptr;
	CRcTex*					m_pBufferRighttopCom = nullptr;

	CTransform*				m_pTransformLeftCom = nullptr;
	CTransform*				m_pTransformRightCom = nullptr;
	CTransform*				m_pTransformLeftTopCom = nullptr;
	CTransform*				m_pTransformRightTopCom = nullptr;


	_short					m_sLeftTop = 0, m_sLeft = 0, m_sRight = 0, m_sRightTop = 0;
	CTexture*				m_pTextureCom = nullptr;
private:
	_matrix  m_matProj;
	_float m_fX, m_fY, m_fSizeX, m_fSizeY;
public:
	static CTempOccupationScore*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CTempOccupationScore*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg);
private:
	virtual void Free(void) override;
};
