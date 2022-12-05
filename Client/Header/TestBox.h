#pragma once
#include "GameObject.h"
#include "Collisionable.h"

BEGIN(Engine)
class CTank_Head;
class CTransform;
END

class CTestBox : 
	public CGameObject , public ICollisionable
{
private:
	explicit CTestBox(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTestBox(const CTestBox& rhs);
	virtual ~CTestBox();
public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;
	virtual const  _vec3	Get_Info(void);
	virtual	void		Move_Info(_vec3 _Info);
	virtual			void		OBB_Collision_EX(void) {}
	virtual			void		Update_OBB(void);
	virtual			OBB*        Get_OBB(void) { return &m_stBody; }
private:
	HRESULT		Add_Component(void);

private:
	CTank_Head*			m_pHeadBuffer = nullptr;
	CTransform*			m_pTransformHead = nullptr;
public:
	static CTestBox*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

