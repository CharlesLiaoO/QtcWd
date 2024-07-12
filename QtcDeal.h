#ifndef QTCDEAL_H
#define QTCDEAL_H

#include <QString>

QString GetQtcShortcut(QString key);
void CloseCurrentProject();
void ModifyProjectConfig(QString projectDir);
void ReopenProject(QString projectDir);

#endif // QTCDEAL_H
