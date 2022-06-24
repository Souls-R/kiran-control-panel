/**
 * Copyright (c) 2022 KylinSec Co., Ltd.
 * kiran-cpanel-network is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 *
 * Author:     luoqing <luoqing@kylinos.com.cn>
 */

#include <kiran-application.h>
#include <qt5-log-i.h>
#include <QApplication>
#include "cpanel-network-widget.h"
#include "manager-tray.h"
#include "input-dialog.h"

int main(int argc, char *argv[]) {
    KiranApplication a(argc, argv);
    klog_qt5_init("", "kylinsec-session", "kiran-cpanel-network", "kiran-cpanel-network");
    CPanelNetworkWidget networkWidget;
    networkWidget.show();
    InputDialog inputDialog;
    inputDialog.show();
    ManagerTray tray;
    return QApplication::exec();
}
