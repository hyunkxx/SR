#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CAim_UI_Pers :
	public CGameObject
{
public:
	explicit CAim_UI_Pers(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAim_UI_Pers(const CAim_UI_Pers& rhs);
	virtual ~CAim_UI_Pers();

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
	CRcTex*				m_pBufferCom2 = nullptr;
	CTexture*			m_pTextureCom_0 = nullptr;
	CTexture*			m_pTextureCom_1 = nullptr;
	CTexture*			m_pTextureCom_2 = nullptr;
	CTransform*			m_pTransformCom = nullptr;

	_bool				m_bTexture[3];
public:
	static CAim_UI_Pers*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;

};

