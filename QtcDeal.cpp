#include "QtcDeal.h"

#include <Common/KeyShortcut.h>
#include <Common/GetProcessInfo.h>
#include <QStandardPaths>
#include <QSettings>
#include <QDebug>
#include <QThread>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QRegularExpression>

void CloseCurrentProject()
{
    qInfo("Start Close Current Project");
    QString closePrjShortcut = GetQtcShortcut("ProjectExplorer.UnloadCM");
    // QThread::msleep(1000);
    SendShortCutToFocusWin(closePrjShortcut);
    // SendShortCutToFocusWin("Shift+Alt+K");
    qInfo("End Close Current Project");
}

QString GetQtcShortcut(QString key)
{
    QString qtcIniPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/QtProject/QtCreator.ini";
    QSettings qtcCfg(qtcIniPath, QSettings::IniFormat);

    qtcCfg.beginGroup("KeyboardShortcuts");
    QString keyVal = qtcCfg.value(key).toString();
    if (!keyVal.isEmpty())
        return keyVal;
    qtcCfg.endGroup();

    qtcCfg.beginGroup("KeyboardShortcutsV2");
    keyVal = qtcCfg.value(key).toString();
    return keyVal;
}

void ModifyProjectConfig(QString projectDir)
{
    QDir dir(projectDir);
    dir.mkpath(projectDir + "/bin");

    QStringList sFileNames = dir.entryList({"*.pro.user", "CMakeList.txt.user"});
    static QRegularExpression regWdd(R"(\s*<value type="QString" key="RunConfiguration.WorkingDirectory.default">)");

    for (auto &sFileName : sFileNames) {
        QFile fCfg(dir.filePath(sFileName));
        if (!fCfg.open(QIODevice::ReadWrite)) {
            qCritical()<< __FUNCTION__<< "Open file err:"<< fCfg.errorString();
            continue;
        }

        QString sFileContent, sLine;
        while (!fCfg.atEnd()) {
            sLine = fCfg.readLine();
            if (sLine.indexOf(regWdd) == 0) {
                QString addLine;
                addLine += R"(<value type="QString" key="RunConfiguration.WorkingDirectory">)";
                addLine += projectDir + "/bin";
                addLine += "</value>";
                sFileContent += addLine + "\n";
            }
            sFileContent += sLine;
        }

        fCfg.seek(0);
        fCfg.write(sFileContent.toUtf8());
    }
}

void ReopenProject(QString projectDir)
{
    // C:\Qt\qtcreator-13.0.2\bin\qtcreator.exe -pid 4948 D:\01_EmbeddedSystem\RemoteControl
    int ppid = GetParentProcessID();
    QString qtcPath = GetProcessExePath(ppid);
    // QString qtcPath = R"(C:\Qt\qtcreator-13.0.2\bin\qtcreator.exe)";
    QString qtcPid = QString("-pid %1").arg(ppid);
    // QString projectDir = "D:/01_EmbeddedSystem/t_parentProcess";

    QStringList cmdArgs;
    cmdArgs<< qtcPath<< qtcPid<< projectDir;
    QString cmd = cmdArgs.join(" ");
    qInfo()<< __FUNCTION__<< "cmd: "<< cmd;
    system(cmd.toStdString().c_str());
}