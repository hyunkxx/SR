#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CUI_MiniMap : public Engine::CGameObject
{
private:
	explicit CUI_MiniMap(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_MiniMap(const CUI_MiniMap& rhs);
	virtual ~CUI_MiniMap();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta)override;
	virtual void LateUpdate_Object(void)  override;
	virtual void Render_Object(void)  override;
	virtual const   _vec3		Get_Info(void) { return _vec3(); }
	virtual			void		Move_Info(_vec3 _Info) {}
	static CUI_MiniMap*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:

	_int				m_iTankType;
	wstring		m_szTankType;

	HRESULT		Add_Component(void);

	virtual void Free(void) override;


	CRcTex*						m_pRcTex = nullptr;
	CTexture*					m_pTexture = nullptr;
	CTransform*				m_pTransform = nullptr;

	_matrix  m_matProj;

	_float					m_fScaleX;
	_float					m_fScaleY;
	_float					m_fScaleZ;

	_float					m_fPosX;
	_float					m_fPosY;
	_float					m_fPosZ;
	_bool					m_bPosinOn = true;
public:


};

