#ifndef QTCDEAL_H
#define QTCDEAL_H

#include <QString>

QString GetQtcShortcut(QString key);
void CloseCurrentProject();
void ModifyProjectConfig(QString projectDir);
void ModifyProjectConfig_BD(const QString &projectDir, QString &sLine, int idxKey, const QString &ssBd);
void ReopenProject(QString projectDir);

#endif // QTCDEAL_H
