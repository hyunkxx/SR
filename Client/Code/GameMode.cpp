#include "stdafx.h"
#include "..\Header\GameMode.h"

#include "Humvee.h"
#include "SmallTank.h"
#include "MiddleTank.h"
#include "BigTank.h"
#include "LongTank.h"

IMPLEMENT_SINGLETON(CGameMode)

CGameMode::CGameMode()
{
}


CGameMode::~CGameMode()
{
}

inline void CGameMode::Free(void)
{
}
