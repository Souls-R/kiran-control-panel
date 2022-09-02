 /**
  * @Copyright (C) 2020 ~ 2021 KylinSec Co., Ltd.
  *
  * Author:     liuxinhao <liuxinhao@kylinos.com.cn>
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
 
#include "hover-tips.h"

#include <QDebug>
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QStyleOption>
#include <qt5-log-i.h>

HoverTips::HoverTips(QWidget *parent)
    : QWidget(parent)
{
    initUI();
    setVisible(false);
    this->setFixedHeight(36);
}

HoverTips::~HoverTips()
{
}

void HoverTips::show(HoverTipsTypeEnum typeEnum, const QString &msg)
{
    auto iter = m_tipsTypeIconMap.find(typeEnum);
    if (iter == m_tipsTypeIconMap.end())
    {
        KLOG_WARNING() << "invalid type enum";
        return;
    }

    if (isVisible())
    {
        hide();
    }

    QString pixmapPath = iter.value();
    m_iconLabel->setPixmap(pixmapPath);
    m_textLabel->setText(msg);

    QWidget::show();

    startHideTimer();
}

void HoverTips::hide()
{
    QWidget::hide();
}

void HoverTips::updatePostion()
{
    if (parentWidget() == nullptr)
    {
        KLOG_WARNING() << "hover tips parnetwidget is null";
        return;
    }
    this->move((parentWidget()->width() - width()) / 2,
               (parentWidget()->height() - height()) / 2);
}

bool HoverTips::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == parentWidget())
    {
        switch (event->type())
        {
        case QEvent::Resize:
            updatePostion();
            break;
        case QEvent::Move:
            updatePostion();
            break;
        default:
            break;
        }
    }
    return QObject::eventFilter(watched, event);
}

void HoverTips::setIcon(HoverTips::HoverTipsTypeEnum typeEnum, const QString &icon)
{
    QPixmap pixmap;
    if (!pixmap.load(icon) || pixmap.isNull())
    {
        KLOG_WARNING() << "load icon failed.";
        return;
    }
    m_tipsTypeIconMap[typeEnum] = icon;
}

void HoverTips::initUI()
{
    auto *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(6);
    mainLayout->setContentsMargins(12, -1, 12, -1);

    m_iconLabel = new QLabel(this);
    m_iconLabel->setObjectName("label_icon");
    m_iconLabel->setFixedSize(16, 16);
    mainLayout->addWidget(m_iconLabel);

    m_textLabel = new QLabel(this);
    m_textLabel->setObjectName("label_text");
    m_textLabel->setScaledContents(false);
    mainLayout->addWidget(m_textLabel);
}

void HoverTips::paintEvent(QPaintEvent *event)
{
    QStyleOption styleOption;
    QPainter painter(this);
    styleOption.init(this);
    style()->drawPrimitive(QStyle::PE_Widget,
                           &styleOption,
                           &painter,
                           this);
}

bool HoverTips::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::ShowToParent:
    {
        adjustSize();
        updatePostion();
        break;
    }
    case QEvent::Timer:
    {
        auto timerEvent = dynamic_cast<QTimerEvent *>(event);
        if (timerEvent->timerId() == m_hideTimerID)
        {
            QWidget::hide();
            stopHideTimer();
        }
        break;
    }
    case QEvent::MouseButtonPress:
    {
        QWidget::hide();
        stopHideTimer();
        break;
    }
    default:
        break;
    }
    return QWidget::event(event);
}

void HoverTips::setTimeout(quint32 ms)
{
    if (m_hideTimeout == ms)
    {
        return;
    }

    if (ms == 0)
    {
        stopHideTimer();
        return;
    }

    m_hideTimeout = ms;
}

void HoverTips::startHideTimer()
{
    if (m_hideTimeout == 0)
    {
        return;
    }
    stopHideTimer();
    m_hideTimerID = startTimer(m_hideTimeout);
}

void HoverTips::stopHideTimer()
{
    if (m_hideTimerID == -1)
    {
        return;
        ;
    }
    killTimer(m_hideTimerID);
    m_hideTimerID = -1;
}