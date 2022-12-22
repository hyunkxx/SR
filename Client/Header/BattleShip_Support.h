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
	void				Fire(void);
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

	_float				m_ShootingTime = 0.f;
	_bool				m_bBulletShoot = false;
	_int				m_iBulletCount = 0;
public:
	static CBattleShip_Support*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;
	// Minimap UI
	_matrix            UI_Minimap_matProj;
	CRcTex*            m_pMinimap_RcTex = nullptr;
	CTexture*         m_pMinimap_Texure = nullptr;
	CTransform*      m_pMinimap_Transform = nullptr;
	_float               m_fMinimap[TRANSFORM_FLOAT_END];
public:
	void					Update_Minimap(void);
};

