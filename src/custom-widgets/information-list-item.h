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

#ifndef INFORMATIONLISTITEM_H
#define INFORMATIONLISTITEM_H

#include <QWidget>

namespace Ui
{
class InformationListItem;
}

class InformationListItem : public QWidget
{
    Q_OBJECT

public:
    explicit InformationListItem(QWidget *parent = 0);
    ~InformationListItem();

protected:
    void paintEvent(QPaintEvent *event);

public:
    void setItemText(const QString &text);
    void setItemIcon(const QString &iconPath);
    void setItemArrow(bool isSelected);
    virtual QSize sizeHint() const;

private:
    Ui::InformationListItem *ui;
};

#endif  // InformationListItem_H
