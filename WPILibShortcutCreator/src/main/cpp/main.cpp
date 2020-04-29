/*
 * This C++ source file was generated by the Gradle 'init' task.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>

#include "ShortcutCreator.h"

#include <objbase.h>
#include <winerror.h>
#include "shobjidl.h"
#include "objidl.h"
#include "shlguid.h"
#include "ComPtr.h"

#include "ShortcutData.h"

#include <nlohmann/json.hpp>

// for convenience
using json = nlohmann::json;

#pragma comment(lib, "Ole32.lib")

class CCoInitialize {
public:
 CCoInitialize() : m_hr(CoInitialize(NULL)) { }
 ~CCoInitialize() { if (SUCCEEDED(m_hr)) CoUninitialize(); }
 operator HRESULT() const { return m_hr; }
 HRESULT m_hr;
};

#define WPILIB_MISSING_PROGRAM_ARGUMENTS -1
#define WPILIB_INITIALIZATION_FAILURE -2
#define WPILIB_CREATION_FAILURE -3
#define WPILIB_SUCCESS 0


int main (int argc, char *argv[]) {
    if (argc < 2) {
        return WPILIB_MISSING_PROGRAM_ARGUMENTS;
    }


    // Initialize COM
    CCoInitialize init;

    if(FAILED(init)) {
        return WPILIB_INITIALIZATION_FAILURE;
    }

    // Initialize the shortcut creator
    ShortcutCreator shortcutCreator;

    if (FAILED(shortcutCreator)) {
        return WPILIB_INITIALIZATION_FAILURE;
    }

    json j;

    std::ifstream i(argv[1]);
    i >> j;

    ShortcutData s = j.get<ShortcutData>();

    std::optional<std::wstring> desktopFolder;
    std::optional<std::wstring> startMenuFolder;

    if (s.isAdmin) {
        desktopFolder = shortcutCreator.GetPublicDesktopFolder();
        startMenuFolder = shortcutCreator.GetAllUsersStartMenuFolder();
    } else {
        desktopFolder = shortcutCreator.GetLocalDesktopFolder();
        startMenuFolder = shortcutCreator.GetLocalStartMenuFolder();
    }

    bool createdDesktop = false;

    if (desktopFolder) {
        createdDesktop = shortcutCreator.CreateShortcuts(s.desktopShortcuts, s.iconLocation, *desktopFolder);
    }

    bool createdStartMenu = false;

    if (startMenuFolder) {
        createdStartMenu = shortcutCreator.CreateShortcuts(s.startMenuShortcuts, s.iconLocation, *startMenuFolder);
    }

    if (!createdDesktop || !createdStartMenu) {
        return WPILIB_CREATION_FAILURE;
    }

    return WPILIB_SUCCESS;
}
