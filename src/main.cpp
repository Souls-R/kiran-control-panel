/**
 * @file main.cpp
 * @brief  程序入口
 * @author yuanxing <yuanxing@kylinos.com.cn>
 * @copyright (c) 2020 KylinSec. All rights reserved.
 */
#include "kiran-cpanel-mouse.h"
#include "tools/log.h"
#include "dbus-interface/mouse-interface.h"
#include "dbus-interface/touchpad-interface.h"
#include <QApplication>
#include <QLoggingCategory>
#include <QDebug>
#include <QTranslator>
#include <QFile>
#include <iostream>
#include <zlog_ex.h>
#include <kiranwidgets-qt5/kiran-message-box.h>
#include <kiranwidgets-qt5/kiran-single-application.h>
#include <kiranwidgets-qt5/kiran-application.h>
#include "config/config.h"

//TODO:use config.h
#define TRANSLATION_DIR TRANSLATIONS_FILE_DIR

bool init_zlog = false;

int main(int argc, char *argv[])
{
    //设置日志输出
    if (dzlog_init_ex(NULL, "kylinsec-session", "kiran-cpanel-mouse", "kiran-cpanel-mouse") < 0){
        std::cout << "init zlog error" << std::endl;
    }
    else{
        init_zlog = true;
    }
    qInstallMessageHandler(Log::customMessageHandler);
    QLoggingCategory::defaultCategory()->setEnabled(QtMsgType::QtDebugMsg,true);
    qInfo("******New Output*********\n");

    //只允许单个程序运行
    KiranSingleApplication a(argc, argv);

    //加载翻译文件
    QTranslator *qtTranslator = new QTranslator(qApp);
    if(qtTranslator->load(QLocale(),"kiran-cpanel-mouse",".",TRANSLATION_DIR,".qm")){
        a.installTranslator(qtTranslator);
    }
    else
    {
        qDebug("Load Translator File failed : %s\n", TRANSLATION_DIR);
    }

    KiranCPanelMouse w;
    if(!w.m_cpanelMouseWidget->initUI())
    {
        KiranMessageBox::message(nullptr,QObject::tr("Faild"),
                                 QObject::tr("Connect Mouse or TouchPad Dbus Failed!"),
                                 KiranMessageBox::Ok);
        exit(-1);
    }

    w.resize(w.sizeHint());
    w.show();

    return a.exec();
}
