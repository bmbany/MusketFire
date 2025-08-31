#pragma once

// 여기에 미리 컴파일하려는 헤더 추가
#include <iostream>
#include <Windows.h>
#include <io.h>

#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <queue>
#include <algorithm>

#include "Engine_Function.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Typedef.h"
#include "Engine_Enum.h"