#pragma once
#include "GameObject.h"

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
class CDefault_Enermy :public CGameObject
{

private:
	explicit CDefault_Enermy(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDefault_Enermy(const CDefault_Enermy& rhs);
	virtual ~CDefault_Enermy();

public:
	virtual HRESULT Ready_Object(void) override;
	HRESULT Ready_Object(void* pArg);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;
public:


	//로케이션에 줘야하는 함수 시간나면 이넘값으로 수정
	void	StateCheck();
	_bool Get_RightLocation() { return m_bRightLocationCount; }
	_bool Get_LeftLocation() { return m_bLeftLocationCount; }
	_bool Get_RightTopLocation() { return m_bRightTopLocationCount; }
	_bool Get_LeftTopLocation() { return m_bLeftTopLocationCount; }

	void  Set_SuccessOccupation(_int RecentSO) { m_iSuccessOccupation = RecentSO; }
	_int  Get_LocationCheck() { return m_iLocationCheck; }
	void  Set_LocationCheck(_int _Check) { m_iLocationCheck = _Check; }
	void  Set_Action(_int _action) { m_iAction = _action; }
	void  Set_PastLocation(_int _Past) { m_PastLocation = _Past; }
	_int Get_Action() { return m_iAction; }

public:
	//에너미에서 사용하는 함수
	void Basic(_float fTimeDelta);
	void Occupation(_float fTimeDelta);
	void Wait(_float fTimeDelta);
	void Enermy_In_Area(_float fTimeDelta);
	void Detect(_float fTimeDelta);
	_bool  Left_RightCheck(_vec3 _vDir, _vec3 _vLook);
	_float Dist(CTransform* _Target);

private:
	HRESULT		Add_Component(void);
	EData*      m_EData;
private:
	CCalculator*		m_pCalculatorCom = nullptr;
	//탱크형태
	CVoxel*				m_pBody = nullptr;
	CVoxel*				m_pHead = nullptr;
	CVoxel*				m_pPosin = nullptr;

	//탱크 움직임
	CTransform*			m_pTransformHead = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTransform*			m_pTransformPosin = nullptr;

private:
	_int  m_iLocationState;
	_int  m_iAction = AIACTION::AIACTION_END;

	_bool m_bRightLocationCount = false
		, m_bRightTopLocationCount = false
		, m_bLeftLocationCount = false
		, m_bLeftTopLocationCount = false;
	_int m_iSuccessOccupation = LOCATIONCHECK::LOCATIONCHECK_END;
	_int m_iLocationCheck = false;
	_int m_PastLocation;
	_vec3   m_vPatrol = {};
	_bool LeftCheck = false;

	_float m_fReloadTime = 0.f, m_fReload = 0.1f;
	//Test
	_float Test = 0.f;
public:
	static CDefault_Enermy*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CDefault_Enermy*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg);
private:
	virtual void Free(void) override;

};

