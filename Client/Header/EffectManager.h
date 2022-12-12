#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CEffectPool;
END

class CEffectManager :
	public CGameObject
{
private:
	explicit CEffectManager(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffectManager(const CEffectManager& rhs);
	virtual ~CEffectManager();
public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;
	virtual const  _vec3 Get_Info(void);

	CEffectPool* GetEffectPool() { return m_pEffectPool; }
private:
	HRESULT	Add_Component(void);
private:
	CEffectPool* m_pEffectPool = nullptr;
public:
	static CEffectManager* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

