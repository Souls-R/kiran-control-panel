/**
 * Copyright (c) 2020 ~ 2022 KylinSec Co., Ltd.
 * kiran-control-panel is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 *
 * Author:     liuxinhao <liuxinhao@kylinsec.com.cn>
 */

#ifndef KIRAN_CONTROL_PANEL_KYLINSEC_LOGO_H
#define KIRAN_CONTROL_PANEL_KYLINSEC_LOGO_H

#include <QWidget>
#include <QPaintEvent>

class KylinsecLogo : public QWidget
{
    Q_OBJECT
public:
    KylinsecLogo(QWidget* parent = nullptr);
    ~KylinsecLogo();

private:
    void paintEvent(QPaintEvent* event);

};

#endif  // KIRAN_CONTROL_PANEL_KYLINSEC_LOGO_H
