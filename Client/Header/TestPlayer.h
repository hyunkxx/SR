#pragma once
#include "GameObject.h"

class CStaticCamera;

BEGIN(Engine)
class CRcTex;
class CTank_Body;
class CTank_Head;
class CTankPosin;
class CHitPoint;
class CTransform;
class CTexture;
class CCalculator;
class CVoxel;
END

class CTestPlayer :	public Engine::CGameObject
{
private:
	explicit CTestPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTestPlayer(const CTestPlayer& rhs);
	virtual ~CTestPlayer();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;
	virtual void RenderGUI(void) override;
	virtual const   _vec3		Get_Info(void);
	virtual			void		Move_Info(_vec3 _Info);
private:
	void		Expect_Hit_Point(const _float& fTimeDelta);

public:
	void		Get_CameraInst(CGameObject* pCameraInst) {m_pCameraInst = pCameraInst;}

private:
	HRESULT		Add_Component(void);
	void		Key_Input(const _float& fTimeDelta);
	void		SetUp_OnTerrain(void);
	void		Head_Spin(const _float& fTimeDelta);

	_float		Plus_Advance_AccelSpeed(const _float& fTimeDelta);
	_float		Minus_Advance_AccelSpeed(const _float& fTimeDelta);

	_float		Plus_Back_AccelSpeed(const _float& fTimeDelta);
	_float		Minus_Back_AccelSpeed(const _float& fTimeDelta);

	_vec3 PickUp_OnTerrain(void);
	//junior
private:
	CTank_Body*			m_pBodyBuffer = nullptr;
	CTank_Head*			m_pHeadBuffer = nullptr;
	CTankPosin*			m_pPosinBuffer = nullptr;
	CHitPoint*			m_pHitPointBuffer = nullptr;
	CTexture*			m_pTextureCom = nullptr;

	CTransform*			m_pTransformBody = nullptr;
	CTransform*			m_pTransformHead = nullptr;
	CTransform*			m_pTransformPosin = nullptr;
	CTransform*			m_pTransHitPoint = nullptr;

	CCalculator*		m_pCalculatorCom = nullptr;
	// (가속도 공식 용)속도 , 전진 가속도, 후진 가속도 , 누적시간값 , 전진 후진 상태
	_float				m_fSpeed = 0.f;
	_float				m_fAccel_Ad = 0.15f;
	_float				m_fAccel_Back = 0.07f;
	_float				m_fAccum = 0.f;
	_bool				m_bAdvance = false;
	_bool				m_bBack = false;
	CGameObject*		m_pCameraInst = nullptr;
	
	_float				m_fBooster = 0.05f;
	_float				m_fBoosterAccum = 0.f;

	// 장전 속도
	_float				m_fReload = 1.f;
	_float				m_fReloadTime = 0.f;
	_int				m_iCannonSpeed = 500;
	//Voxel 추가
	CVoxel*				m_pBody  = nullptr;
	CVoxel*				m_pHead  = nullptr;
	CVoxel*				m_pPosin = nullptr;
public:
	static CTestPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

}; 