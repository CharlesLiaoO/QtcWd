#include "GetProcessInfo.h"

#include <QtGlobal>
#include <QDir>
#include <QProcess>

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

#ifdef Q_OS_LINUX

#include <iostream>
#include <unistd.h> // for getppid()
#include <limits.h> // for PATH_MAX
#include <string>
#include <cstdlib> // for realpath

std::string get_parent_exe_path()
{
    pid_t parent_pid = getppid(); // 获取父进程的PID
    char proc_path[PATH_MAX];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/exe", parent_pid);

    // 使用 realpath 来解析符号链接到实际的可执行文件路径
    char resolved_path[PATH_MAX];
    if (realpath(proc_path, resolved_path) == nullptr) {
        perror("realpath");
        return "";
    }

    return std::string(resolved_path);
}


uint GetParentProcessID()
{
    return getppid();
}

uint GetParentProcessID(uint pid)
{
    // getppid(pid);  // no api
    QString cmd = QString("ps -o ppid= -p %1").arg(pid);
    QProcess prc;
    prc.start(cmd);
    prc.waitForFinished(1000);
    QByteArray output = prc.readAll().trimmed();
    return output.toUInt();
}

QString GetParentProcessExePath()
{
    return GetProcessExePath(getppid());
}

QString GetProcessExePath(uint pid)
{
    char proc_path[PATH_MAX];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/exe", pid);
    // QString r = QString("/proc/%1/exe").arg(pid);

    // 使用 realpath 来解析符号链接到实际的可执行文件路径
    char resolved_path[PATH_MAX];
    if (realpath(proc_path, resolved_path) == nullptr) {
        perror("realpath");
        return "";
    }

    return QString::fromUtf8(resolved_path);
}

#endif
