#pragma once
#include "GameObject.h"
#include "Voxel.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CTerrainTex;
class CTransform;
END

class CBattleShip :
	public CGameObject
{
private:
	explicit CBattleShip(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBattleShip(const CBattleShip & rhs);
	virtual ~CBattleShip();
public:
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;
	virtual void		RenderGUI(void)override;
public:
	void				Strike(void);

private:
	virtual	HRESULT		Add_Component(void);
	virtual HRESULT		Ready_Object(void);
private:
	CTransform*			m_pTransformBody = nullptr;
	CTransform*			m_pTransformHead = nullptr;
	CTransform*			m_pTransformPosin = nullptr;
	CTransform*			m_pTransformHead2 = nullptr;
	CTransform*			m_pTransformPosin2 = nullptr;

	CTransform*	        m_pTransformSee = nullptr;
	CTerrainTex*		m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;

	CVoxel*				m_pBody = nullptr;
	CVoxel*				m_pHead = nullptr;
	CVoxel*				m_pPosin = nullptr;
	CVoxel*				m_pHead2 = nullptr;
	CVoxel*				m_pPosin2 = nullptr;

	_float				m_fMoveTime = 0.f;

	_bool				m_bShootReady = false;
	_bool				m_bBoomShoot = false;;
	_float				m_fShootTime = 0.f;
	_int				m_iShootCount = 0;
	_float				m_fAngle_1 = 0.f;
	_float				m_fAngle_2 = 0.f;
	_float				m_fChangeTime = 0.f;

	CGameObject*		m_pNewTarget = nullptr;
public:
	static CBattleShip* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);
};

