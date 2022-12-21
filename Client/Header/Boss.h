#pragma once
#include "GameObject.h"
#include "Collisionable.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CTransform;
class CVoxel;
END

class CBoss :
	public CGameObject, public ICollisionable
{
private:
	explicit CBoss(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBoss(const CBoss& rhs);
	virtual ~CBoss();

public:
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;


	virtual const _vec3	Get_Info(void)override;
	virtual	void		Move_Info(_vec3 _Info)override;
	virtual			void		RenderGUI(void)override {}
	virtual			void		OBB_Collision_EX(void) {}
	virtual			void		Update_OBB(void);
	virtual			OBB*        Get_OBB(void) { return &m_stBody; }

public:
	void				Appear(void);
	void				Pattern(const _float& fTimeDelta);
	void				Pattern_01(const _float& fTimeDelta);
	void				Pattern_02(const _float& fTimeDelta);
	void				Pattern_03(const _float& fTimeDelta);
	void				Pattern_04(const _float& fTimeDelta);
	void				Pattern_05(const _float& fTimeDelta);

private:
	virtual	HRESULT		Add_Component(void);
	virtual HRESULT		Ready_Object(void);
	virtual void		Sound_Setting(const _float & fTimeDelta);
	virtual void        Shoot_Bullet(BULLET_ID eID);
	void				Posin_Setting(const _vec3& _SetPos);
	void				Head_Setting(const _vec3& _SetPos);
	void				Body_Setting(const _vec3& _SetPos);
	virtual void		Dead_Motion(const _float& fTimeDelta);
	void				Head_Spin(const _float& fTimeDelta);
	void				Body_Spin(const _float& fTimeDelta);
	void				Move_Pos(_vec3 Move);

	void				Export_Hit_Point(void);
public:
	void				Rotation_Body(ROTATION eID, _float fAngle);
	void				Rotation_Head(ROTATION eID, _float fAngle);
	void				Rotation_Posin(ROTATION eID, _float fAngle);

private:
	CTransform*			m_pTransformBody = nullptr;
	CTransform*			m_pTransformHead = nullptr;
	CTransform*			m_pTransformPosin = nullptr;

	CVoxel*				m_pBody = nullptr;
	CVoxel*				m_pHead = nullptr;
	CVoxel*				m_pPosin = nullptr;

	Tank_State			m_stInfo;

	_int				m_iMotionCount = 0;


	_bool				m_bAppear = false;
	_int				m_iAppearCount = 0;

	_bool				m_bPattern_1 = false;
	_bool				m_bPattern_2 = false;
	_bool				m_bPattern_3 = false;
	_bool				m_bPattern_4 = false;
	_bool				m_bPattern_5 = false;

	_bool				m_bDeadEvent = false;

	_float				m_fShootCount = 0.f;
	_float				m_fPattern_3Count = 0.f;
	_float				m_fPattern_5Count = 0.f;
	_bool				m_bHeadMove = true;

	_vec3				m_vHitPoint = {};

	_bool				m_bPattern_Type = false;
	_float				m_fRushCount = 0.f;
	_vec3				m_vRushDist = {};

	_float				m_fBoomberTime = 0.f;
	_int				m_iBoomber_Count = 0;

	_float				m_fRushSpeed = 200.f;
public:
	virtual void Free(void) override;
	static CBoss* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

