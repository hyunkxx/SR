#pragma once
#include "Base.h"
#include "Include.h"

BEGIN(Engine)
class CGameObject;
END

class CPlayerManager : public CBase
{
	DECLARE_SINGLETON(CPlayerManager)
private:
	explicit CPlayerManager();
	virtual ~CPlayerManager();
public:
	inline virtual void	Free(void);
public:
	void CreateVehicle(LPDIRECT3DDEVICE9 pGraphic, VEHICLE eType);
	CGameObject** GetVehicle() { return &m_pVehicle; }
private:
	CGameObject* m_pVehicle;
};

