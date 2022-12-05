#pragma once
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <concurrent_queue.h>
#include <concurrent_priority_queue.h>
#include <unordered_map>
#include <unordered_set>
#include <cstring>
#include <string.h>
#include <string>

#include <functional>
#include <algorithm>
#include <ctime>
#include <cassert>

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>

#include <process.h>

#include "Macro.h"
#include "Enum.h"

#include <io.h>

#include "fmod.hpp"
#include "fmod.h"

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

using namespace std;



