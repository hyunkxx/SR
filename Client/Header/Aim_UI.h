#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CAim_UI :
	public CGameObject
{
public:
	explicit CAim_UI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAim_UI(const CAim_UI& rhs);
	virtual ~CAim_UI();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

private:
	HRESULT		Add_Component(void);
	void		Key_Input(void);
private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom_0 = nullptr;
	CTexture*			m_pTextureCom_1 = nullptr;
	CTexture*			m_pTextureCom_2 = nullptr;
	CTransform*			m_pTransformCom = nullptr;

	_bool				m_bTexture[3];
private:
	/* 직교 투영 */
	_matrix				m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

public:
	static CAim_UI*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;

};
