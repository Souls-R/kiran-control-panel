/**
 * Copyright (c) 2022 KylinSec Co., Ltd.
 * kiran-control-panel is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 *
 * Author:     luoqing <luoqing@kylinsec.com.cn>
 */

#ifndef KIRAN_CPANEL_NETWORK_WIRELESS_WIDGET_H
#define KIRAN_CPANEL_NETWORK_WIRELESS_WIDGET_H

#include <NetworkManagerQt/WirelessSetting>
#include <QWidget>
QT_BEGIN_NAMESPACE
namespace Ui
{
class WirelessWidget;
}
QT_END_NAMESPACE

class KiranSwitchButton;
class WirelessWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WirelessWidget(QWidget *parent = nullptr);
    ~WirelessWidget() override;

    void initUI();
    void initConnection();
    void setWirelessSetting(const NetworkManager::WirelessSetting::Ptr &wirelessSetting);
public slots:
    void saveSettings();
    void setOtherWirelessSetting();
    void showSettings();
    void resetSettings();
    void clearPtr();
    void handleCustomMTUChanged(bool checked);
    void initMacComboBox();
private:
    Ui::WirelessWidget *ui;
    NetworkManager::WirelessSetting::Ptr m_wirelessSetting;
    KiranSwitchButton *m_mtuButton;
};

#endif  //KIRAN_CPANEL_NETWORK_WIRELESS_WIDGET_H
