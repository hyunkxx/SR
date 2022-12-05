#ifndef Include_h__
#define Include_h__

#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
#include <list>
#include <map>
#include <queue>
#include <string>
#include <unordered_map>

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>

#define		DIRECTINPUT_VERSION		0x0800
#include <dinput.h>

#include "Typedef.h"
#include "Enum.h"
#include "Macro.h"
#include "Struct.h"
#include "Template.h"

#include <io.h>
#include "fmod.hpp"
#include "fmod.h"

#pragma warning(disable : 4251)

extern HINSTANCE	 g_hInst;
extern HWND g_hWnd;

using namespace std;
using namespace Engine;


#endif // Include_h__
