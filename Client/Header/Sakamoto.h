#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CSakamoto :
	public CGameObject
{
public:
	explicit CSakamoto(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSakamoto(const CSakamoto& rhs);
	virtual ~CSakamoto();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

private:
	HRESULT		Add_Component(void);
public:
	void		Set_Sakamoto(void);
private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;

private:
	/* 직교 투영 */
	_matrix				m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

	_bool				m_bType = false;
	_bool				m_bSound = false;
	_float				m_fSoundCount = 0.f;
public:
	static CSakamoto*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;
};

