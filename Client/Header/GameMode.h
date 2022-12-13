#pragma once
#include "Base.h"
#include "Include.h"

BEGIN(Engine)
class CGameObject;
END

class CGameMode : public CBase
{
	DECLARE_SINGLETON(CGameMode)
private:
	explicit CGameMode();
	virtual ~CGameMode();
public:
	inline virtual void	Free(void);
public:

private:

};