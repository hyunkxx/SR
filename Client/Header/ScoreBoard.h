#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CScoreBoard : public Engine::CGameObject
{
private:
	explicit CScoreBoard(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CScoreBoard(const CScoreBoard& rhs);
	virtual ~CScoreBoard();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta)override;
	virtual void LateUpdate_Object(void)  override;
	virtual void Render_Object(void)  override;
	virtual const   _vec3		Get_Info(void) { return _vec3(); }
	virtual			void		Move_Info(_vec3 _Info) {}
	static CScoreBoard*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	HRESULT		Add_Component(void);

	virtual void Free(void) override;


	CRcTex*					m_pRcTex = nullptr;
	CTexture*				m_pTexture = nullptr;
	CTransform*				m_pTransform = nullptr;

	_matrix  m_matProj;

	_float					m_fSizeX = 0.f, m_fSizeY = 0.f, m_fX = 0.f, m_fY = 0.f;

};

