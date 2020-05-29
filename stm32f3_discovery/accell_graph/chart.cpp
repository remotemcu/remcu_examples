/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "chart.h"
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QtCore/QRandomGenerator>
#include <QtCore/QDebug>

static const size_t maxTickCount = 10;

Chart::Chart(QGraphicsItem *parent, Qt::WindowFlags wFlags):
    QChart(QChart::ChartTypeCartesian, parent, wFlags),
    m_series_0(0),m_series_1(0),
    m_axisX(new QValueAxis()),
    m_axisY(new QValueAxis()),
    m_x(0),
    m_y(1)
{
    m_series_0 = new QSplineSeries(this);
    m_series_1 = new QSplineSeries(this);
    m_series_2 = new QSplineSeries(this);
    QPen green(Qt::green);
    QPen red(Qt::red);
    QPen blue(Qt::blue);
    green.setWidth(2);
    red.setWidth(2);
    blue.setWidth(2);
    m_series_0->setPen(green);
    m_series_0->append(m_x, m_y);
    m_series_0->setName(QString("Accel X"));
    m_series_1->setPen(red);
    m_series_1->append(m_x, m_y);
    m_series_1->setName(QString("Accel Y"));
    m_series_2->setPen(blue);
    m_series_2->append(m_x, m_y);
    m_series_2->setName(QString("Accel Z"));


    addSeries(m_series_0);
    addSeries(m_series_1);
    addSeries(m_series_2);

    addAxis(m_axisX,Qt::AlignBottom);
    addAxis(m_axisY,Qt::AlignLeft);
    m_series_0->attachAxis(m_axisX);
    m_series_0->attachAxis(m_axisY);
    m_series_1->attachAxis(m_axisX);
    m_series_1->attachAxis(m_axisY);
    m_series_2->attachAxis(m_axisX);
    m_series_2->attachAxis(m_axisY);

    m_axisX->setTickCount(maxTickCount);
    m_axisX->setRange(0, maxTickCount);
    m_axisY->setRange(-700, 700);

    this->legend()->setVisible(true);
    this->legend()->setAlignment(Qt::AlignBottom);
}

Chart::~Chart()
{

}

void Chart::printData(accel_data_t data){
    qDebug() << data.accX << data.accY << data.accZ;

    qreal x = plotArea().width() / m_axisX->tickCount();
    m_x += 1;
    m_series_0->append(m_x, data.accX);
    m_series_1->append(m_x, data.accY);
    m_series_2->append(m_x, data.accZ);
    if(m_x > maxTickCount){
        scroll(x, 0);
    }
}
