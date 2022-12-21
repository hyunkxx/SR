#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRcCol;
END
class CRedCarpet :
	public CGameObject
{
public:
	explicit CRedCarpet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRedCarpet(const CRedCarpet& rhs);
	virtual ~CRedCarpet();

public:
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;
private:
	virtual	HRESULT		Add_Component(void);
	virtual HRESULT		Ready_Object(void);

public:
	void				Set_Carpet(CTransform* pTrans, OBB* pOBB);
private:
	CTransform* m_pTransCom = nullptr;
	CRcCol*		m_pRcCom = nullptr;

	CTransform* m_pBossTransCom = nullptr;
	_float		m_bDeadCount = 0.f;
	_float		m_fXDist = 1.f;

	_vec3		m_vPos;
	_vec3		m_vDir;
	_vec3		m_vLen;
public:
	static CRedCarpet*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free(void) override;
};

