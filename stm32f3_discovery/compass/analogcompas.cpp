/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>

#include "analogcompas.h"

//! [0] //! [1]
AnalogCompas::AnalogCompas(QWidget *parent)
//! [0] //! [2]
    : QWidget(parent)
//! [2] //! [3]
{

    setWindowTitle(tr("Analog Compas F3 Discovery"));
    resize(400, 400);
//! [7]
}
//! [1] //! [7]

void AnalogCompas::setAngle(const float angle){
    this->angle = angle;
}

//! [8] //! [9]
void AnalogCompas::paintEvent(QPaintEvent *)
//! [8] //! [10]
{
    static const QPoint compasHandNorth[3] = {
        QPoint(7, 0),
        QPoint(-7, 0),
        QPoint(0, -70)
    };
    static const QPoint compasHandSouth[3] = {
        QPoint(7, 0),
        QPoint(-7, 0),
        QPoint(0, 70)
    };

    QColor southColor(127, 22, 12);
    QColor northColor(20, 16, 127, 191);

    int side = qMin(width(), height());
//! [10]

//! [11]
    QPainter painter(this);
//! [11] //! [12]
    painter.setRenderHint(QPainter::Antialiasing);
//! [12] //! [13]
    painter.translate(width() / 2, height() / 2);
//! [13] //! [14]
    painter.scale(side / 200.0, side / 200.0);
//! [9] //! [14]

//! [15]
    painter.setPen(Qt::NoPen);
//! [15] //! [16]
    painter.setBrush(southColor);
//! [16]

//! [17] //! [18]
    painter.save();

    painter.restore();
//! [18] //! [19]

//! [20]
    painter.setPen(southColor);
//! [20] //! [21]

    for (int i = 0; i < 12; ++i) {
        painter.drawLine(88, 0, 96, 0);
        painter.rotate(30.0);
    }
//! [21]

//! [22]
    painter.setPen(Qt::NoPen);
//! [22] //! [23]
    painter.setBrush(northColor);
//! [24]
    painter.save();
    painter.rotate(angle);
    painter.drawConvexPolygon(compasHandNorth, 3);
    painter.setBrush(southColor);
    painter.drawConvexPolygon(compasHandSouth, 3);
    painter.restore();
//! [23] //! [24]

//! [25]
    painter.setPen(northColor);
//! [25] //! [26]

//! [27]
    for (int j = 0; j < 60; ++j) {
        if ((j % 5) != 0)
            painter.drawLine(92, 0, 96, 0);
        painter.rotate(6.0);
    }
//! [27]
}
//! [26]
