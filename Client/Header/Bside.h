#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END
class CBside : public Engine::CGameObject
{
private:
	explicit CBside(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBside(const CBside& rhs);
	virtual ~CBside();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta)override;
	virtual void LateUpdate_Object(void)  override;
	virtual void Render_Object(void)  override;
	virtual const   _vec3		Get_Info(void) { return _vec3(); }
	virtual			void		Move_Info(_vec3 _Info) {}
	_float		Lerp(_float Start, _float End, _float Dist) { return (((1 - Dist)*Start) + (Dist*End)); }

	static CBside*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	HRESULT		Add_Component(void);

	virtual void Free(void) override;
	_int m_iCount = 0;
	_int m_iColor = 0;
	_bool Check = false;
	_float SaveDist = 0.f;
	_float Save = 0.f;
	_bool RightEnd = false, BottomEnd = false, LeftEnd = false;
	_bool m_bEnemyOcc = false, m_bAllyOcc = false;

	CRcTex*					m_pRcTex = nullptr;
	CRcTex*					m_pRcTexRight = nullptr;
	CRcTex*					m_pRcTexBottom = nullptr;
	CRcTex*					m_pRcTexLeft = nullptr;

	CRcTex*					m_pRcTexLogo = nullptr;

	CTexture*				m_pTexture = nullptr;
	CTexture*				m_pLogoTexture = nullptr;

	CTransform*				m_pTransformCom = nullptr;
	CTransform*				m_pTransformRight = nullptr;
	CTransform*				m_pTransformBottom = nullptr;
	CTransform*				m_pTransformLeft = nullptr;

	CTransform*				m_pTransformLogo = nullptr;
	_matrix					m_matProj;
	_float					m_fSizeX = 0.f, m_fSizeY = 0.f, m_fX = 0.f, m_fY = 0.f;
	_float					m_OrignfX = 0.f, m_OrignfY = 0.f;
	_float StartTime = 0.f;

};

