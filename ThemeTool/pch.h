// SecureUxTheme - A secure boot compatible in-memory UxTheme patcher
// Copyright (C) 2020  namazso
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include "targetver.h"
#include "resource.h"

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <oleauto.h>
#include <shellapi.h>
#include <winternl.h>
#include <wtsapi32.h>
#include <psapi.h>
#include <tchar.h>
#include <wrl.h>

#include <tuple>
#include <list>
#include <memory>
#include <string>
#include <vector>
#include <string_view>
#include <random>

#include "stringencrypt.h"

using Microsoft::WRL::ComPtr;

#ifndef EXTERN_C_START
#define EXTERN_C_START extern "C" {
#endif
#ifndef EXTERN_C_END
#define EXTERN_C_END }
#endif

#ifdef _MSC_VER
#define LAMBDA_STDCALL
#else
#define LAMBDA_STDCALL __stdcall
#endif

#ifdef __MINGW32__
// From mingw-w64/mingw-w64-crt/crt/pesect.c, Public Domain
#if defined (_WIN64) && defined (__ia64__)
#error FIXME: Unsupported __ImageBase implementation.
#else
#ifdef __GNUC__
/* Hack, for bug in ld.  Will be removed soon.  */
#define __ImageBase __MINGW_LSYMBOL(_image_base__)
#endif
/* This symbol is defined by the linker.  */
extern IMAGE_DOS_HEADER __ImageBase;
#endif
#endif
