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
#include "pch.h"

#include "main.h"

#include "dlg.h"
#include "MainDialog.h"
#include "utl.h"

#pragma comment(lib, "ComCtl32.lib")
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "Wtsapi32.lib")

// we use the builtin one so all our crt methods aren't resolved from ntdll
#pragma comment(lib, "ntdll.lib")

typedef VOID(NTAPI LDR_ENUM_CALLBACK)(
  _In_ PLDR_DATA_TABLE_ENTRY ModuleInformation,
  _In_ PVOID Parameter,
  _Out_ BOOLEAN* Stop
  );
typedef LDR_ENUM_CALLBACK* PLDR_ENUM_CALLBACK;

NTSTATUS
NTAPI
LdrEnumerateLoadedModules(
  _In_ BOOLEAN ReservedFlag,
  _In_ PLDR_ENUM_CALLBACK EnumProc,
  _In_opt_ PVOID Context
);

HINSTANCE g_instance;
ComPtr<IThemeManager2> g_pThemeManager2;

static int main_gui(int nCmdShow)
{
  INITCOMMONCONTROLSEX iccex
  {
    sizeof(INITCOMMONCONTROLSEX),
    ICC_WIN95_CLASSES
  };

#define POST_ERROR(...) utl::FormattedMessageBox(nullptr, _T("Error"), MB_OK | MB_ICONERROR, __VA_ARGS__)

  if (!InitCommonControlsEx(&iccex))
    return POST_ERROR(ESTRt(L"InitCommonControlsEx failed, LastError = %08X"), GetLastError());

  auto hr = CoInitialize(nullptr);
  if (FAILED(hr))
    return POST_ERROR(ESTRt(L"CoInitialize failed, hr = %08X"), hr);

  // IID_PPV_ARGS causes "undefined reference to `_GUID const& __mingw_uuidof<IThemeManager2>()'" linker error
  // So, use IID_PPV_ARGS_Helper directly
  hr = CoCreateInstance(CLSID_ThemeManager2, NULL, CLSCTX_ALL, IID_IThemeManager2, IID_PPV_ARGS_Helper(g_pThemeManager2.GetAddressOf()));
  if (FAILED(hr))
    return POST_ERROR(ESTRt(L"CoCreateInstance failed, hr = %08X"), hr);

  hr = g_pThemeManager2->Init(ThemeInitNoFlags);
  if (FAILED(hr))
    return POST_ERROR(ESTRt(L"g_pThemeManager2->Init failed, hr = %08X"), hr);

  // win8
  LoadLibraryExW(ESTRt(L"advapi32"), nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
  // win10
  LoadLibraryExW(ESTRt(L"cryptsp"), nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);

  const auto ntdll = GetModuleHandleW(ESTRt(L"ntdll"));
  if (!ntdll)
    return POST_ERROR(ESTRt(L"ntdll is null, GetLastError() = %08X"), GetLastError());

  const auto pLdrEnumerateLoadedModules = (decltype(&LdrEnumerateLoadedModules))GetProcAddress(ntdll, ESTRt("LdrEnumerateLoadedModules"));
  if (!pLdrEnumerateLoadedModules)
    return POST_ERROR(ESTRt(L"pLdrEnumerateLoadedModules is null, GetLastError() = %08X"), GetLastError());

  // This one only supports local process, so antiviruses dont spazz out over it. Or they just don't know it exists
  pLdrEnumerateLoadedModules(
    0,
    [](
      _In_ PLDR_DATA_TABLE_ENTRY ModuleInformation,
      _In_ PVOID Parameter,
      _Out_ BOOLEAN* Stop
      ) LAMBDA_STDCALL
    {
      *Stop = FALSE;
      if (const auto pfn = GetProcAddress((HMODULE)ModuleInformation->DllBase, ESTRt("CryptVerifySignatureW")))
      {
        // We can just do a dirty patch here since noone else would be calling CryptVerifySignatureW in our process

#if defined(_M_IX86) || defined(__i386__)

        constexpr BYTE bytes[] = {
          0xB8, 0x01, 0x00, 0x00, 0x00,   // mov eax, 1
          0xC2, 0x18, 0x00                // ret 18
        };

        // Idiotic antiviruses think we're writing memory of some other process, so we have to do VirtualProtect here
        /*const auto ret = WriteProcessMemory(
          GetCurrentProcess(),
          (PVOID)pfn,
          bytes,
          sizeof(bytes),
          nullptr
        );*/

        DWORD old_protect = 0;
        const auto ret = VirtualProtect(
          (PVOID)pfn,
          sizeof(bytes),
          PAGE_EXECUTE_READWRITE,
          &old_protect
        );

        if (!ret)
          return;

        memcpy((PVOID)pfn, bytes, sizeof(bytes));

        // we don't care if this fails, the page will just stay RWX at most
        VirtualProtect(
          (PVOID)pfn,
          sizeof(bytes),
          old_protect,
          &old_protect
        );

#else
#error ThemeTool only supports x86 builds
#endif

      }
    },
    nullptr
  );


  const auto dialog = CreateDialogParam(
    g_instance,
    MAKEINTRESOURCE(IDD_MAIN),
    nullptr,
    &DlgProcClassBinder<MainDialog>,
    0
  );
  ShowWindow(dialog, nCmdShow);

  MSG msg;
  while (GetMessage(&msg, nullptr, 0, 0))
  {
    if (!IsDialogMessage(dialog, &msg))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return (int)msg.wParam;
}

int APIENTRY wWinMain(
  _In_ HINSTANCE     hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPWSTR        lpCmdLine,
  _In_ int           nCmdShow
)
{
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  g_instance = hInstance;

  MessageBoxW(
    nullptr,
    ESTRt(L"This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details."),
    ESTRt(L"Warranty disclaimer"),
    MB_OK | MB_ICONWARNING
  );

  return main_gui(nCmdShow);
}
