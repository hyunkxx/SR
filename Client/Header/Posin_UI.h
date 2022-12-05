#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CPosin_UI : public Engine::CGameObject
{
private:
	explicit CPosin_UI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPosin_UI(const CPosin_UI& rhs);
	virtual ~CPosin_UI();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta)override;
	virtual void LateUpdate_Object(void)  override;
	virtual void Render_Object(void)  override;
	virtual const   _vec3		Get_Info(void) { return _vec3(); }
	virtual			void		Move_Info(_vec3 _Info) {}
	static CPosin_UI*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
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

	void				Set_ScaleX(const _float& fX) { m_fScaleX = fX; }
	void				Set_ScaleY(const _float& fY) { m_fScaleY = fY; }
	void				Set_ScaleZ(const _float& fZ) { m_fScaleZ = fZ; }

	void				Set_PosX(const _float& fX) { m_fPosX = fX; }
	void				Set_PosY(const _float& fY) { m_fPosY = fY; }
	void				Set_PosZ(const _float& fZ) { m_fPosZ = fZ; }

};

