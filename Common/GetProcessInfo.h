#ifndef GETPROCESSINFO_H
#define GETPROCESSINFO_H

#include <QString>

uint GetParentProcessID();
uint GetParentProcessID(uint pid);

QString GetParentProcessExePath();
QString GetProcessExePath(uint pid);

#endif // GETPROCESSINFO_H
