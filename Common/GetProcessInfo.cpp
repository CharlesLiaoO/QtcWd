#include "GetProcessInfo.h"

#include <QtGlobal>
#include <QDir>

#ifdef Q_OS_WIN

#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>

uint GetParentProcessID()
{
    return GetParentProcessID(GetCurrentProcessId());
}

uint GetParentProcessID(uint pid)
{
    DWORD ppid = -1;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 pe;
        pe.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hSnapshot, &pe)) {
            do {
                if (pe.th32ProcessID == pid) {
                    ppid = pe.th32ParentProcessID;
                    break;
                }
            } while (Process32Next(hSnapshot, &pe));
        }
        CloseHandle(hSnapshot);
    }
    return ppid;
}


QString GetParentProcessExePath()
{
    return GetProcessExePath(GetParentProcessID(GetCurrentProcessId()));
}

QString GetProcessExePath(uint pid)
{
    QString exePath;
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (hProcess != NULL) {
        wchar_t path[MAX_PATH];
        if (GetModuleFileNameEx(hProcess, NULL, path, MAX_PATH)) {
            exePath = QString::fromWCharArray(path);
        }
        CloseHandle(hProcess);
    }
    return QDir::fromNativeSeparators(exePath);
}

#endif

