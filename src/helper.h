// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef HELPER_H
#define HELPER_H

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>
#include <QApplication>
#include "algo.h"

#include <QGraphicsScene>

#include <QGraphicsView>

#include <QGraphicsRectItem>

#include "math.h"

//! [0]
class Helper
{
public:
    Helper();

public:
    void paint(QGraphicsPathItem *m_pathItem,algo::Tour pt,QGraphicsItem** p2p,qreal offset);
    void paint(QGraphicsPathItem *m_pathItem,QPointF from,QPointF to,qreal offset);

private:
    QBrush background;
    QBrush circleBrush;
    QFont textFont;
    QPen circlePen;
    QPen textPen;

    QPen edgePen;
};
//! [0]

#endif
