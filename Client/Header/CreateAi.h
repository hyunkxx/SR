#pragma once
#include "GameObject.h"
#include"GameMode.h"
BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CCreateAi : public Engine::CGameObject
{
private:
	explicit CCreateAi(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCreateAi(const CCreateAi& rhs);
	virtual ~CCreateAi();
public:
	static CCreateAi*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta)override;
	virtual void LateUpdate_Object(void)  override;
	virtual void Render_Object(void)  override;
	void     Set_FieldCount(_int _minusCount) { Field -= _minusCount; }
	void     CheckFieldCount(void);
	void    Produce(_float fTimeDelta);
private:
	CGameMode* Enermy = nullptr;
	EData    m_eData;
	_float   m_CoolTime = 0.f;
	_int Field = 0;
	_int Random = 0;
	_int LimitHum = 0;
	_bool Check = false;
private:
	virtual void Free(void) override;
};



