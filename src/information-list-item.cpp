/**
 * @Copyright (C) 2020 ~ 2021 KylinSec Co., Ltd.
 *
 * Author:     yuanxing <yuanxing@kylinos.com.cn>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see <http: //www.gnu.org/licenses/>.
 */

#include "information-list-item.h"
#include "ui_information-list-item.h"
#include <QStyleOption>
#include <QPainter>

InformationListItem::InformationListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InformationListItem)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyleSheet);
}

InformationListItem::~InformationListItem()
{
    delete ui;
}

void InformationListItem::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

/**
 * @brief setItemText 设置左边列表项的文字
 * @param text 要设置的文字
 */
void InformationListItem::setItemText(const QString &text)
{
    ui->label_list_text->setText(text);
}

/**
 * @brief setItemIcon 设置左边列表项的图标
 * @param iconPath 图标路径
 */
void InformationListItem::setItemIcon(const QString &iconPath)
{
    ui->label_icon_info->setFixedSize(16,16);
    ui->label_icon_info->setStyleSheet(QString("QLabel#label_icon_info{border-image:url(%1);}").arg(iconPath));
}

void InformationListItem::setItemArrow(bool isSelected)
{
    if(isSelected)
        ui->label_icon_arrow->setStyleSheet("QLabel#label_icon_arrow{border-image:url(:/images/arrow.svg);}");
    else
        ui->label_icon_arrow->setStyleSheet("QLabel#label_icon_arrow{border-image:none;}");
}

QSize InformationListItem::sizeHint() const
{
    QSize sizeHint = QSize(246,60);
    return sizeHint;
}
