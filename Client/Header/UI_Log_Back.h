#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CUI_Log_Back : public Engine::CGameObject
{
private:
	explicit CUI_Log_Back(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_Log_Back(const CUI_Log_Back& rhs);
	virtual ~CUI_Log_Back();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta)override;
	virtual void LateUpdate_Object(void)  override;
	virtual void Render_Object(void)  override;
	virtual const   _vec3		Get_Info(void) { return _vec3(); }
	virtual			void		Move_Info(_vec3 _Info) {}
	static CUI_Log_Back*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

	void UpdateChatting(void);

private:
	HRESULT		Add_Component(void);

	virtual void Free(void) override;

	wstring	m_szChattingLog[5];
	_int			m_iPlayerTime;



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


public:

	void				Set_ScaleX(const _float& fX) { m_fScaleX = fX; }
	void				Set_ScaleY(const _float& fY) { m_fScaleY = fY; }
	void				Set_ScaleZ(const _float& fZ) { m_fScaleZ = fZ; }

	void				Set_PosX(const _float& fX) { m_fPosX = fX; }
	void				Set_PosY(const _float& fY) { m_fPosY = fY; }
	void				Set_PosZ(const _float& fZ) { m_fPosZ = fZ; }

};

