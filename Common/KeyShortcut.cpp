#include "KeyShortcut.h"

#include <QtGlobal>
#include <QStringList>
#include <QVector>
#include <QDebug>

#ifdef Q_OS_WIN

#include <windows.h>

void SendShortCutToFocusWin(QString keySeq)
{
    QStringList keyList = keySeq.split("+");
    int keySize = keyList.size();

    QList<int> kvs;
    for (int i = 0; i < keySize; i++) {
        if (keyList.at(i) == "Ctrl") {
            kvs << VK_LCONTROL;
        } else if (keyList.at(i) == "Shift") {
            kvs << VK_LSHIFT;
        } else if (keyList.at(i) == "Alt") {
            kvs << VK_LMENU;
        } else if (keyList.at(i) == "Win") {
            kvs << VK_LWIN;
        } else {
            kvs << keyList.at(i).toLatin1().at(0);
        }
    }

    // press: A+B+C -> release:C+B+A
    QVector<INPUT> keys_INPUT;
    for (int i = 0; i < keySize; i++) {
        INPUT input;
        ZeroMemory(&input, sizeof(INPUT));
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = kvs.at(i);
        keys_INPUT << input;
    }
    for (int i = keySize * 2 - 1; i >= keySize; i--) {
        INPUT input;
        ZeroMemory(&input, sizeof(INPUT));
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = kvs.at(i % keySize);
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        keys_INPUT << input;
    }

    // for debug
    // QStringList SendInputKeys;
    // for (auto inp : keys_INPUT) {
    //     SendInputKeys<< QString("key=%1,up=%2").arg(inp.ki.wVk).arg(inp.ki.dwFlags);
    // }
    // qInfo()<< "SendInput keys:"<< SendInputKeys.join(" # ");

    int keySeqSize = keys_INPUT.size();
    UINT uSent = SendInput(keySeqSize, &keys_INPUT[0], sizeof(INPUT));
    if (uSent != keySeqSize) {
        qCritical()<< QString("SendInput failed: ret=%1/%2 err=0x%3").arg(uSent).arg(keySeqSize).arg(HRESULT_FROM_WIN32(GetLastError()), 0, 16);
    }
}

#endif