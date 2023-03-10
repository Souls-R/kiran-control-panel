/**
 * Copyright (c) 2020 ~ 2023 KylinSec Co., Ltd.
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

#pragma once
#include "enroll-progressbar.h"

class QLabel;
class PixmapPreview;
class FingerEnrollProgressBar : public EnrollProgressBar
{
    Q_OBJECT
public:
    explicit FingerEnrollProgressBar(QWidget* parent = nullptr);
    virtual ~FingerEnrollProgressBar();

    virtual void setProgress(uint progress);

private:
    void init();

private:
    static const std::list<std::tuple<uint, QString>> m_progressRangePixmapList;
    PixmapPreview* m_fingerWidget = nullptr;
};