/**
 * Copyright (c) 2020 ~ 2021 KylinSec Co., Ltd.
 * kiran-cpanel-system is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 *
 * Author:     yuanxing <yuanxing@kylinos.com.cn>
 */
#include "interface.h"
#include "pages/hardware-information/hardware-information-widget.h"
#include "pages/system-information/system-information-widget.h"
#include "system-info-dbus.h"
#include "config/config.h"
#include <kiranwidgets-qt5/kiran-message-box.h>
#include <kiran-log/qt5-log-i.h>
#include <QLocale>
#include <QTranslator>
#include <QCoreApplication>
#include <QFile>
#include <iostream>
#include <QMessageBox>
#define TRANSLATION_DIR TRANSLATIONS_FILE_DIR

KcpInterface::KcpInterface()
{
}

bool KcpInterface::haveUnsavedOptions()
{
    return false;
}

QStringList KcpInterface::visibleSubItems()
{
    QStringList subItems;
    subItems << "SystemInformationWidget" << "HardwareInformationWidget" ;
    return subItems;
}

QWidget *KcpInterface::getSubItemWidget(QString id)
{
    QWidget* widget = nullptr;
    if (id == "SystemInformationWidget")
    {
        widget = new SystemInformationWidget();
    }
    else if (id == "HardwareInformationWidget")
    {
        widget = new HardwareInformationWidget();
    }
    m_currentWidget = widget;

    ///加载qss样式表
    QFile file(":/qss/style.qss");
    if( file.open(QFile::ReadOnly))
    {
        QString styleSheet = QLatin1String(file.readAll());

        m_currentWidget->setStyleSheet(styleSheet);
        file.close();
    }
    else
    {
        QMessageBox::warning(NULL, "warning", "load qss file failed", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    return m_currentWidget;
}

void KcpInterface::uninit()
{
    if( m_translator )
    {
        QCoreApplication::removeTranslator(m_translator);
        delete m_translator;
        m_translator = nullptr;
    }
}

int KcpInterface::init()
{
    QString systemInfo;
    if(!InfoDbus::SystemInfo::getSystemInfo(1,systemInfo) || !InfoDbus::SystemInfo::getSystemInfo(0,systemInfo))
    {
        KLOG_DEBUG() << "Connect dbus service failed! ";
        return -1;
    }
    //加载翻译文件
    if (m_translator != nullptr)
    {
        QCoreApplication::removeTranslator(m_translator);
        delete m_translator;
        m_translator = nullptr;
    }

    m_translator = new QTranslator;
    if (!m_translator->load(QLocale(),
                            "kiran-cpanel-system",
                            ".",
                             TRANSLATION_DIR,
                            ".qm"))
    {
        KLOG_DEBUG() << "Kiran cpanel system load translation failed" ;
        m_translator->deleteLater();
        m_translator = nullptr;
    }
    else
    {
        QCoreApplication::installTranslator(m_translator);
    }
    return 0;
}

