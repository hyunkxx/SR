#pragma once
#include "TankSet.h"


BEGIN(Engine)
class CCalculator;
class CVoxel;
END


class CBigTank :
	public CTankSet
{

private:
	explicit CBigTank(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBigTank(const CBigTank& rhs);
	virtual ~CBigTank();
public:
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;
	virtual void		RenderGUI(void) override;

public:
	virtual	HRESULT		Add_Component(void);
	virtual HRESULT		Ready_Object(void);

private:
	CTransform*			m_pTransHitPoint = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;

public:
	void		Key_Input(const _float& fTimeDelta);
	void		SetUp_OnTerrain(void);
	void		Head_Spin(const _float& fTimeDelta);

private:
	void		Expect_Hit_Point(const _float& fTimeDelta);
	_vec3		PickUp_OnTerrain(void);
	void		Camera_Change(void);
public:
	static CBigTank*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;
};

