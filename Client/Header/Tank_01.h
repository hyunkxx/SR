#pragma once
#include "TankSet.h"

BEGIN(Engine)
class CCalculator;
class CVoxel;
class CEffectPool;
END

class CTank_01 :
	public CTankSet
{
private:
	explicit CTank_01(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTank_01(const CTank_01& rhs);
	virtual ~CTank_01();

public:
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;
	virtual void RenderGUI(void) override;
public:
	virtual	HRESULT		Add_Component(void);
	virtual HRESULT		Ready_Object(void);

private:
	CCalculator*		m_pCalculatorCom = nullptr;

	CEffectPool*			m_pEffectPool = nullptr;
public:
	void		Key_Input(const _float& fTimeDelta);
	void		SetUp_OnTerrain(void);
	void		Head_Spin(const _float& fTimeDelta);
private:
	void		Expect_Hit_Point(const _float& fTimeDelta);
	_vec3		PickUp_OnTerrain(void);
	void		Camera_Change(void);
	void		Posin_Shake(const _float& fTimeDelta);

private:
	_float		m_fPosinAccum = 0.f;
	_bool		m_bPosinShake = false;

public:
	static CTank_01*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;
};

