﻿// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2024-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef LICENSEUI_H
#define LICENSEUI_H

#include "qapplication.h"
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include <QGuiApplication>
#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QApplication>

namespace Ui {
class LicenseUI;
}

class LicenseUI : public QWidget
{
    Q_OBJECT

public:
    explicit LicenseUI(QWidget *parent = nullptr);
    ~LicenseUI();

private slots:
    void on_pushButton_released();

private:
    struct LicenseBlock
    {
        QString libraryName;
        QString repositoryURL;
        QString licenseName;
        QString projectIntroduction;
        QString fileContent;
    };

    void readLicenseBlocks();
    void displayLicenses();

private:
    Ui::LicenseUI *ui;

    QStringList m_readmeBlocks;
    QList<LicenseBlock> m_licenseBlocks;
};

#endif // LICENSEUI_H
