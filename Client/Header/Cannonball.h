#pragma once
#include "Bullet.h"

BEGIN(Engine)
class CVIBullet;
class CTransform;
END

class CCannonball :
	public CBullet
{
public:
	explicit CCannonball(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCannonball(const CCannonball& rhs);
	virtual ~CCannonball();
public:
	virtual HRESULT Ready_Object(void)override;
	virtual _int Update_Object(const _float& fTimeDelta)override;
	virtual void LateUpdate_Object(void)override;
	virtual void Render_Object(void)override;
	virtual void Bullet_Setting(_vec3 vPos, _vec3 vDir, const _float fSpeed, _float fAngleX, _float fAngleY);
	virtual const   _vec3		Get_Info(void) { return _vec3(); }
	virtual			void		Move_Info(_vec3 _Info) {}
private:
	HRESULT		Add_Component(void);

private:
	CTransform* m_pTransform = nullptr;
	CVIBullet* m_pBUlletBuffer = nullptr;

public:
	static CCannonball*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	virtual void		Free(void)override;
};

