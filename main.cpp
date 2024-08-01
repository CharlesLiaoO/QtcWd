#include <QCoreApplication>

#include <QDir>
#include <QDebug>
#include <QThread>
#include <QtcDeal.h>

// when debug, set 'Run configuration'->'Command line arguments' to Other project's path

int main(int argc, char *argv[])
{
    qInfo("\n---- Start QtcWd ----");

    if (argc < 2) {
        qCritical()<< "Missing argument: project directory path";
        return -1;
    }

#ifndef QT_DEBUG
    CloseCurrentProject();
#endif
    QString projectDir = QDir::fromNativeSeparators(argv[1]);
    QThread::msleep(1000);  // wait for qtc to save the current project config
    ModifyProjectConfig(projectDir);
    ReopenProject(projectDir);  // the parent proceess is debugger when debugging

    return 0;
}
