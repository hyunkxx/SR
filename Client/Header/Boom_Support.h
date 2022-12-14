#pragma once
#include "GameObject.h"
#include "Voxel.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CTransform;
END

class CBoom_Support :
	public CGameObject
{
public:
	explicit CBoom_Support(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBoom_Support(const CBoom_Support& rhs);
	virtual ~CBoom_Support();
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

	_bool				m_bSetting = false;
public:
	static CBoom_Support*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
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

