#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CVISmoke;



class ENGINE_DLL CSmoke_Bullet :
	public CGameObject
{
public:
	explicit CSmoke_Bullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSmoke_Bullet(const CSmoke_Bullet& rhs);
	virtual ~CSmoke_Bullet();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

private:
	HRESULT		Add_Component(void);

private:
	CVISmoke*			m_pSmokeBufferCom = nullptr;
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
public:
	void Set_ID(BULLET_ID eID);
	BULLET_ID	Get_ID(void) { return m_eID; }
	void Bullet_Setting(_vec3 vPos, _vec3 vDir, const _float fSpeed, _float fAngleX, _float AngleY);
	void		Reset_Trans(void);
private:
	_bool			m_bPage = false;
	_float			m_fFrame = 0.f;

	_vec3	m_vPos;
	_vec3	m_vDir;
	_float	m_fSpeed = 0.f;
	_float	m_fAccum = 0.f;
	_float	m_fAngleX = 0.f;
	_float	m_fAngleY = 0.f;

	BULLET_ID m_eID;
public:
	static CSmoke_Bullet*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;

};

END