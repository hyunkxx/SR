#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CTexture;
class CLocation;

END
class CLeftTopLocation :public CGameObject
{
private:
	explicit CLeftTopLocation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLeftTopLocation(const CLeftTopLocation& rhs);
	virtual ~CLeftTopLocation();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;
private:
	HRESULT		Add_Component(void);

private:

	CTransform*			m_pTransformCom = nullptr;
	CLocation*          m_pLocationCom = nullptr;
	_int				m_LocationState;
	_int				m_iAllyCount = 0, m_iEnermyCount = 0;
	_float				m_EnermyOccupation = 0.f
		, m_AllyOccupation = 0.f;
	_bool               m_Test = false;
public:
	_int				Get_LocationState() { return m_LocationState; }
	void				CheckObject(_int _ObjID);

	_float				Get_AllyOccupation() { return m_AllyOccupation; }
	_float				Get_EnermyOccupation() { return m_EnermyOccupation; }
	_int				Get_AllyCount() { return m_iAllyCount; }
	_int				Get_EnemyCount() { return m_iEnermyCount; }

	void				Set_EnermyCount() { m_iEnermyCount -= 1; }
	void                Set_AllyCount() { m_iAllyCount -= 1; }
public:
	static CLeftTopLocation*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;

};

