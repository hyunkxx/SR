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
class CCalculator;
class CVoxel;
END
class CBottomDirEnermy :public CGameObject
{

private:
	explicit CBottomDirEnermy(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBottomDirEnermy(const CBottomDirEnermy& rhs);
	virtual ~CBottomDirEnermy();

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
	_int  Get_Targeted() { return Targeted; }

	void  Set_Targeted(_int _Count) { Targeted += _Count; }
	void  Set_SuccessOccupation(_int RecentSO) { m_iSuccessOccupation = RecentSO; }
	_int  Get_LocationCheck() { return m_iLocationCheck; }
	void  Set_LocationCheck(_int _Check) { m_iLocationCheck = _Check; }
	void  Set_Action(_int _action) { m_iAction = _action; }
	void  Set_PastLocation(_int _Past) { m_PastLocation = _Past; }

public:
	//에너미에서 사용하는 함수
	void Basic(_float fTimeDelta);
	void Occupation(_float fTimeDelta);
	void Wait(_float fTimeDelta);
	void Enermy_In_Area(_float fTimeDelta);
	void Detect(_float fTimeDelta);
	_bool  Left_RightCheck(_vec3 _vDir, _vec3 _vLook);
	_float Dist(CTransform* _Target);
	_float PreDist = 0.f;
	_bool m_bOcne = false;
	CTransform* pTempTr = nullptr;
private:
	HRESULT		Add_Component(void);

private:
	CVoxel*				m_pBody = nullptr;
	CVoxel*				m_pHead = nullptr;
	CVoxel*				m_pPosin = nullptr;
	//탱크형태
	CTank_Body*			m_pBodyBuffer = nullptr;
	CTank_Head*			m_pHeadBuffer = nullptr;
	//탱크 움직임
	CTransform*			m_pTransformHead = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTransform*			m_pTransformPosin = nullptr;

	EData*				m_EData;
	CCalculator*		m_pCalculatorCom = nullptr;
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
	_int Targeted = 0;
	_float m_fReloadTime = 0.f, m_fReload = 1.f;
	//Test
public:
	static CBottomDirEnermy*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CBottomDirEnermy*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg);

private:
	virtual void Free(void) override;

	// HP UI
	_matrix				UI_matViewF, m_UI_ProjMatrix;
	CRcTex*				m_pRcTexF = nullptr;
	CTexture*			m_pTextureF = nullptr;
	CTransform*		m_pTransformHP_UI = nullptr;
	_float					UI_Orgin_HP, UI_fHP;
	_float					UI_fScaleX, UI_fScaleY, UI_fScaleZ;
	_float					UI_fOrgin_ScaleX;
	void						Minus_HP_UI(_float HP_minus) { UI_fHP -= HP_minus; }
	void						Plus_HP_UI(_float HP_plus) { UI_fHP += HP_plus; }
public:
	void						 Update_UI(void);

};

