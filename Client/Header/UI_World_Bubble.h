#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CUI_World_Bubble : public Engine::CGameObject
{
private:
	explicit CUI_World_Bubble(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_World_Bubble(const CUI_World_Bubble& rhs);
	virtual ~CUI_World_Bubble();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta)override;
	virtual void LateUpdate_Object(void)  override;
	virtual void Render_Object(void)  override;
	virtual const   _vec3		Get_Info(void) { return _vec3(); }
	virtual			void		Move_Info(_vec3 _Info) {}
	static CUI_World_Bubble*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

	void				 Update_Pos(void);


private:
	HRESULT		Add_Component(void);

	virtual void Free(void) override;


	CRcTex*						m_pRcTex = nullptr;
	CTexture*					m_pTexture = nullptr;
	CTransform*				m_pTransform = nullptr;

	_matrix matView;
	_matrix  m_matProj;
private:

	_float					m_fScaleX;
	_float					m_fScaleY;
	_float					m_fScaleZ;

	_float					m_fPosX;
	_float					m_fPosY;
	_float					m_fPosZ;

	_float					m_fOrgin_ScaleX;
	_float					m_fOrgin_PosX;

	_float					m_fTime = 0.f;
	_float					m_fThreeSec = 3.f;
	_bool					m_bShow = false;

};

