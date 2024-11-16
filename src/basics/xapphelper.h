// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2024-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef XAPPHELPER_H
#define XAPPHELPER_H

#include <QString>
#include <QSharedMemory>
#include <QSystemSemaphore>

constexpr const char* szUniqueKey = "MyAppUniqueKey"; // 使用唯一的标识符来创建共享内存和系统信号量
bool detectionSingApp(QSharedMemory& sharedMemory, QSystemSemaphore& systemSemaphore, const QString &uniqueKey = szUniqueKey);

#ifdef _MSC_VER  // 在 MSVC 编译器下的代码
#include <Windows.h>
#include <DbgHelp.h>
#include <QDebug>
#include <QDir>
#include <QDateTime>
#include <QMessageBox>
#include <QCoreApplication>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "dbghelp.lib")

void GetExceptionDescription(DWORD errCode,QString& err);
LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException);   // 程式异常捕获-保存数据代码-创建 Dump 文件
#endif


#endif // XAPPHELPER_H
