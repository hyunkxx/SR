#pragma once

#include "Base.h"
#include "Include.h"
#include "Component.h"
#include "Cube.h"

BEGIN(Engine)

class ENGINE_DLL CEffector : public CComponent
{
	enum class TYPE { USING, UNUSED };
protected:
	explicit CEffector(LPDIRECT3DDEVICE9 pGraphicDevice, _vec3 vPosition);
	explicit CEffector(const CEffector& rhs);
	virtual ~CEffector();
public:
	virtual CComponent* Clone(void) override;
	virtual void		Free(void) override;
	virtual _int		Update_Component(const _float& fTimeDelta) override;
	virtual void		RenderEffect() {};
	virtual void		Reset() {};
public:
	static _uint EffectCount;
	static CEffector* Create(LPDIRECT3DDEVICE9 pGraphicDevice, _vec3 vPosition);
public:
	const _vec3& GetPosition() const { return m_vPosition; }
	void SetPosition(const _vec3& vPosition) { m_vPosition = vPosition; }
	const bool& GetRunngin() const { return m_bRunning; }
	void SetRunning(const bool& bValue) { m_bRunning = bValue; }
protected:
	bool  m_bRunning = false;		//실행중
	float m_fDuration;		//지속시간 = LifeTime
	float m_fLocalTime;		//누적시간 -> 지속시간계산
	float m_fGravityLocal;
	_vec3 m_vPosition;
	vector<CCube*> m_vecEffect;
};

END