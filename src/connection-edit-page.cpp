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

#include "connection-edit-page.h"
#include "ui_connection-edit-page.h"
#include <kiran-switch-button.h>

ConnectionEditPage::ConnectionEditPage(QWidget *parent) : QWidget(parent), ui(new Ui::ConnectionEditPage)
{
    ui->setupUi(this);

    initUI();

    connect(ui->createConnectionButton,&QPushButton::clicked,[=](){
        emit requestCreatConnection();
    });
}

ConnectionEditPage::~ConnectionEditPage()
{
    delete ui;
}
void ConnectionEditPage::setTitle(QString title)
{
    ui->title->setText(title);
}

void ConnectionEditPage::initUI()
{
    m_switchButton = new KiranSwitchButton(this);
    ui->titleLayout->addWidget(m_switchButton);
}

void ConnectionEditPage::setSwitchButtonVisible(bool visible)
{
    m_switchButton->setVisible(visible);
}


