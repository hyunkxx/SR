#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CTransform;
END

class CBattleShip_Support :
	public CGameObject
{
public:
	explicit CBattleShip_Support(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBattleShip_Support(const CBattleShip_Support & rhs);
	virtual ~CBattleShip_Support();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;
	virtual void		RenderGUI(void)override;
public:
	void				Air_Rain(_vec3	_vPos);
private:
	void				Key_Input(const _float& fTimeDelta);
private:
	HRESULT		Add_Component(void);
private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	_bool				m_bStrike = false;
	_float				m_fDaedCount = 0.f;
	_bool				m_bRock = false;

	CRcTex*				m_pBufferCom_Copy[3] = { nullptr ,nullptr ,nullptr };
	CTexture*			m_pTextureCom_Copy[3] = { nullptr ,nullptr ,nullptr };
	CTransform*			m_pTransformCom_Copy[3] = { nullptr ,nullptr ,nullptr };

	_bool				m_bSetting = false;
	_int				m_iShootCount = 0;
public:
	static CBattleShip_Support*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;
};

