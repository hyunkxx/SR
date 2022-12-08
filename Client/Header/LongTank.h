#pragma once
#include "TankSet.h"

BEGIN(Engine)
class CCalculator;
class CVoxel;
class CEffectPool;
END

class CLongTank :
	public CTankSet
{
private:
	explicit CLongTank(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLongTank(const CLongTank& rhs);
	virtual ~CLongTank();

public:
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;
	virtual void RenderGUI(void) override;

	void				 Update_UI(void);
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

	_bool		m_bStart = false;
	_bool		m_bReLoad = true;
public:
	static CLongTank*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;
};

