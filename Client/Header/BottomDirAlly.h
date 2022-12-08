#pragma once
#include "GameObject.h"
#include"TankSet.h"
BEGIN(Engine)

class CTransform;
class CRcTex;
class CTexture;
class CCalculator;
class CLocation;
class CTank_Body;
class CTank_Head;
class CTankPosin;
class CVoxel;
END
class CBottomDirAlly :public CGameObject
{

private:
	explicit CBottomDirAlly(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBottomDirAlly(const CBottomDirAlly& rhs);
	virtual ~CBottomDirAlly();

public:
	virtual HRESULT Ready_Object(void) override;
	HRESULT Ready_Object(void* pArg);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;
public:

	void	StateCheck();
	_bool Get_RightLocation() { return m_bRightLocationCount; }
	_bool Get_LeftLocation() { return m_bLeftLocationCount; }
	_bool Get_RightTopLocation() { return m_bRightTopLocationCount; }
	_bool Get_LeftTopLocation() { return m_bLeftTopLocationCount; }

	_int Get_LocationCheck() { return m_iLocationCheck; }
	void  Set_LocationCheck(_int _Check) { m_iLocationCheck = _Check; }
	void  Set_Action(_int _action) { m_iAction = _action; }
	void  Set_PastLocation(_int _Past) { m_PastLocation = _Past; }
	_float Dist(CTransform* _Target);
public:

	void Basic(_float fTimeDelta);
	void Occupation(_float fTimeDelta);
	void Wait(_float fTimeDelta);
	void Enermy_In_Area(_float fTimeDelta);
	void Detect(_float fTimeDelta);

	_bool  Left_RightCheck(_vec3 _vDir, _vec3 _vLook);
private:
	HRESULT		Add_Component(void);

private:
	CCalculator*		m_pCalculatorCom = nullptr;
	//≈ ≈©«¸≈¬
	CVoxel*				m_pBody = nullptr;
	CVoxel*				m_pHead = nullptr;
	CVoxel*				m_pPosin = nullptr;
	//≈ ≈© øÚ¡˜¿”
	CTransform*			m_pTransformHead = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTransform*			m_pTransformPosin = nullptr;
	CTransform*        m_PreTransformCom = nullptr;
	EData*       m_EData;
	_float       preDist;
	_bool		m_bonce = false;
private:
	_int  m_iLocationState;
	_int  m_iAction = AIACTION::AIACTION_END;
	_bool m_bRightLocationCount = false
		, m_bRightTopLocationCount = false
		, m_bLeftLocationCount = false
		, m_bLeftTopLocationCount = false;
	_int m_iLocationCheck = LOCATIONCHECK::LOCATIONCHECK_END;
	_int m_PastLocation = LOCATIONCHECK::LOCATIONCHECK_END;
	_vec3   m_vPatrol = {};
	_bool LeftCheck = false;
	_float m_fReloadTime = 0.f, m_fReload = 1.f;
	_vec3 vPatrolRange = {};
	_bool m_bPatrol = false;

public:
	static CBottomDirAlly*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CBottomDirAlly*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg);
private:
	virtual void Free(void) override;

};