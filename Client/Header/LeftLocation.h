#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CTexture;
class CLocation;
class CRcTex;
END
class CLeftLocation :public CGameObject
{
private:
	explicit CLeftLocation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLeftLocation(const CLeftLocation& rhs);
	virtual ~CLeftLocation();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

private:
	HRESULT		Add_Component(void);

private:

	CTransform*			m_pTransformCom = nullptr;
	CTexture*			m_pTexture = nullptr;
	CRcTex*				m_pRcTex = nullptr;
	CLocation*          m_pLocationCom = nullptr;
	_int				m_LocationState;
	_int				m_iAllyCount = 0, m_iEnermyCount = 0;
	_float				m_EnermyOccupation = 0.f
		, m_AllyOccupation = 100.f;
public:
	void                Set_AllyCount() { m_iAllyCount -= 1; }
	void				Set_EnermyCount() { m_iEnermyCount -= 1; }
	_int				Get_LocationState() { return m_LocationState; }
	void				CheckObject(_int _ObjID);
	void				Occupation(_int _ObjID);
	_bool               m_Test = false;
	_float				Get_AllyOccupation() { return m_AllyOccupation; }
	_float				Get_EnermyOccupation() { return m_EnermyOccupation; }
public:
	static CLeftLocation*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;

};

