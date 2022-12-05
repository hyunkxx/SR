#pragma once
#include "TankSet.h"

BEGIN(Engine)
class CVoxel;
class CEffectPool;
END

class CTurret :
	public CTankSet
{
public:
	explicit CTurret(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTurret(const CTurret& rhs);
	virtual ~CTurret();

public:
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;
	virtual void RenderGUI(void) override;

public:
	virtual	HRESULT		Add_Component(void);
	virtual HRESULT		Ready_Object(void);

public:
	void		Target_Find(const _float& fTimeDelta);
	void		Posin_Shake(const _float& fTimeDelta);

private:
	_float		m_fPosinAccum = 0.f;
	_bool		m_bPosinShake = false;
	_bool		m_bTarget = false;
private:
	CEffectPool*			m_pEffectPool = nullptr;

public:
	static CTurret*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;
};

