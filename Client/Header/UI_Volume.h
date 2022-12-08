#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CUI_Volume : public Engine::CGameObject
{
private:
	explicit CUI_Volume(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_Volume(const CUI_Volume& rhs);
	virtual ~CUI_Volume();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta)override;
	virtual void LateUpdate_Object(void)  override;
	virtual void Render_Object(void)  override;
	virtual const   _vec3		Get_Info(void) { return _vec3(); }
	virtual			void		Move_Info(_vec3 _Info) {}
	static CUI_Volume*		Create(LPDIRECT3DDEVICE9 pGraphicDev);


	void				 Move_Pos(void);
	void				 Sound_Size_Control(void);
	_bool			Get_Volume_Show(void) { return m_bShow; }


private:
	HRESULT		Add_Component(void);

	virtual void Free(void) override;


	CRcTex*						m_pRcTex = nullptr;
	CTexture*					m_pTexture = nullptr;
	CTransform*				m_pTransform = nullptr;

	_matrix matView;
	_matrix  m_matProj;
private:

	// 전체 창 크기
	_float					m_fScaleX,	m_fScaleY,	m_fScaleZ;
	_float					m_fPosX, m_fPosY, m_fPosZ;

	// 단추 키 크기
	_float					m_RatioX, m_RatioY, m_RatioZ;
	_float					m_RatioSizeX, m_RatioSizeY, m_RatioSizeZ;

	_bool					m_bShow = false;

};

