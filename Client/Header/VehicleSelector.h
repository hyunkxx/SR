#pragma once
#include "GameObject.h"
#include "Include.h"
#include "ButtonUI.h"

BEGIN(Engine)

class CVehicle;

END

class CVehicleSelector : public CGameObject
{
public:
public:
	static CVehicleSelector* Create(LPDIRECT3DDEVICE9 pGramphicDev);
	virtual void Free(void);
	virtual ~CVehicleSelector();
private:
	explicit CVehicleSelector(LPDIRECT3DDEVICE9 pGramphicDev);
	explicit CVehicleSelector(const CVehicleSelector& rhs);
public:
	virtual	HRESULT	Ready_Object(void) override;
	virtual	_int	Update_Object(const _float& fTimeDelta) override;
	virtual	void	LateUpdate_Object(void) override;
	virtual	void	Render_Object(void) override;
	virtual	void	RenderGUI(void) override;
public:

private:
	HRESULT AddComponent();
private:
	bool m_bShow;
	CButtonUI* m_pButton[(UINT)VEHICLE::MAX];
};