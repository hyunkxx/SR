#pragma once

#include "Base.h"
#include "Include.h"

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject :	public CBase
{
public :
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	CComponent*	  Get_Component(const _tchar* pComponentTag, COMPONENTID eID);
	const			_float		Get_Dist(void)const { return m_fCollisionDist; }
	_float			Get_ViewZ(void) { return m_fViewZ; }
	void				Compute_ViewZ(const _vec3* pPos);

public:
	virtual			HRESULT		Ready_Object(void);
	virtual			_int		Update_Object(const _float& fTimeDelta);
	virtual			void		LateUpdate_Object(void);
	virtual			void		Render_Object(void);
	virtual			void		RenderGUI(void);
	//충돌처리 용도
	virtual         _bool		Get_Dead(void) { return m_bDead; }
private:
	CComponent*			Find_Component(const _tchar* pComponentTag, COMPONENTID eID);
	void				Set_CollisionDist(void);
protected:
	unordered_map<const _tchar*, CComponent*>			m_mapComponent[ID_END];
	LPDIRECT3DDEVICE9			m_pGraphicDev;
public:
	virtual LPDIRECT3DDEVICE9 Get_GraphicDev(void) { return m_pGraphicDev; }
protected:
	_float						m_fViewZ;
	_bool	m_bDead = false;
	// 월드상태 스케일값 조정용
	_float  m_fScale;
	// 충돌 처리 거리 설정용
	_float m_fCollisionDist;
	OBB		m_stHead;
	OBB		m_stBody;

public:
	virtual void	Free(void);
};

END