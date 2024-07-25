// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "helper.h"

#include <QPainter>
#include <QPaintEvent>
#include <QWidget>

#include <iostream>
#include <cstdio>

//#include <QDebug>

//! [0]
Helper::Helper()
{
    QLinearGradient gradient(QPointF(50, -20), QPointF(80, 20));
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(1.0, QColor(0xa6, 0xce, 0x39));

    background = QBrush(QColor(64, 32, 64));
    circleBrush = QBrush(gradient);
    circlePen = QPen(Qt::red);
    circlePen.setWidth(5);
    edgePen = QPen(Qt::blue);
    edgePen.setWidth(5);
    textPen = QPen(Qt::white);
    textFont.setPixelSize(50);
}
//! [0]

//! [1]
void Helper::paint(QGraphicsPathItem *m_pathItem,algo::Tour pt,QGraphicsItem** p2p,qreal offset)
{
    //QGraphicsPathItem *m_pathItem = new QGraphicsPathItem();
    QPainterPath path;

    //qDebug() <<pt.size();

    //printf("r=%d\n",pt.r(0));

    path.moveTo(p2p[pt.r(0)]->scenePos().x()+offset,p2p[pt.r(0)]->scenePos().y()+offset);



    int sz = pt.size();
    for (int i = 0; i < sz; i ++){
        printf("lineto%lf,%lf\n",p2p[pt.s(i)]->scenePos().x(),p2p[pt.s(i)]->scenePos().y());
        path.lineTo(p2p[pt.s(i)]->scenePos().x()+offset,p2p[pt.s(i)]->scenePos().y()+offset);
    }

    path.closeSubpath();

    m_pathItem->setPath(path);

    m_pathItem->setPen(circlePen);

    m_pathItem->setFlag(QGraphicsItem::ItemIsMovable);


    //painter->fillRect(event->rect(), background);
    //painter->translate(100, 100);
//! [1]

/*! [2]
    painter->save();
    painter->setBrush(circleBrush);
    painter->setPen(circlePen);
    painter->rotate(elapsed * 0.030);

    qreal r = elapsed / 1000.0;
    int n = 30;
    for (int i = 0; i < n; ++i) {
        painter->rotate(30);
        qreal factor = (i + r) / n;
        qreal radius = 0 + 120.0 * factor;
        qreal circleRadius = 1 + factor * 20;
        painter->drawEllipse(QRectF(radius, -circleRadius,
                                    circleRadius * 2, circleRadius * 2));
    }
    painter->restore();
//! [2]

//! [3]
    painter->setPen(textPen);
    painter->setFont(textFont);
    painter->drawText(QRect(-50, -50, 100, 100), Qt::AlignCenter, QStringLiteral("Qt"));*/
}
//! [3]
//!
//!
void Helper::paint(QGraphicsPathItem *m_pathItem,QPointF from,QPointF to,qreal offset)
{
    //QGraphicsPathItem *m_pathItem = new QGraphicsPathItem();
    QPainterPath path;

    //qDebug() <<pt.size();

    //printf("r=%d\n",pt.r(0));

    from.setX(from.x()+offset);
    from.setY(from.y()+offset);
    to.setX(to.x()+offset);
    to.setY(to.y()+offset);

    path.moveTo(from);

    path.lineTo(to);

    path.closeSubpath();

    m_pathItem->setPath(path);

    m_pathItem->setPen(edgePen);

    m_pathItem->setFlag(QGraphicsItem::ItemIsMovable);
}
