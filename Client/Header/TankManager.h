#pragma once
#include "Base.h"
#include "Include.h"

BEGIN(Engine)
class CGameObject;
END

class CTankManager : public CBase
{
	DECLARE_SINGLETON(CTankManager)
private:
	explicit CTankManager();
	virtual ~CTankManager();
public:
	inline virtual void	Free(void);
public:
	void CreateVehicle(LPDIRECT3DDEVICE9 pGraphic, VEHICLE eType);
	CGameObject** GetVehicle() { return &m_pVehicle; }

	const TANK_STATE& GetData(VEHICLE eType) { return tankData[(UINT)eType]; };
	const wstring& GetName(VEHICLE eType) { return tankData[(UINT)eType].strName; };

	bool IsLock() { return m_bMouseLBTLock; }
	void MouseLBTLock(bool value) { m_bMouseLBTLock = value; }
private:
	void InitalizeData();
private:
	bool m_bMouseLBTLock = false;

	CGameObject* m_pVehicle;
	TANK_STATE tankData[(UINT)VEHICLE::MAX];
};

